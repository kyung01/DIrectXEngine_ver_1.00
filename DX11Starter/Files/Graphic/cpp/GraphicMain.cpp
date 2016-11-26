#include "Graphic\GraphicMain.h"
#include <string>

using namespace NGraphic;

static int SIZE_LIGHT_TEXTURE = 512;
void GraphicMain::processObject(NScene::Object obj) {
}


void GraphicMain::processCamera(NScene::Camera cam)
{
}

void GraphicMain::beginRendering()
{
}

void GraphicMain::endRendering()
{
}

void GraphicMain::getScreenWidth(int & w, int & h)
{
}


void GraphicMain::rendering(NScene::Scene scene)
{

	
}
bool GraphicMain::initTextures(ID3D11Device * device, ID3D11DeviceContext * context,int width, int height)
{
	std::cout << "initTextures";

	//auto lightTexture = std::shared_ptr<DepthTexture>(new DepthTexture());
	this->m_depthTextures[KEnum::RENDER_TYPE_DEFFERED] = std::shared_ptr<DepthTexture>(new DepthTexture());
	this->m_depthTextures[KEnum::RENDER_TYPE_DEFFERED_FINAL] = std::shared_ptr<DepthTexture>(new DepthTexture());
	this->m_depthTextures[KEnum::RENDER_TYPE_DEFFERED]->init(device, width, height);
	this->m_depthTextures[KEnum::RENDER_TYPE_DEFFERED_FINAL]->init(device, width, height);

	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_DIFFUSE]	= std::shared_ptr<RenderTexture>(new RenderTexture());
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED]		= std::shared_ptr<RenderTexture>(new RenderTexture());
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_NORMAL]	= std::shared_ptr<RenderTexture>(new RenderTexture());
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_WORLD]		= std::shared_ptr<RenderTexture>(new  RenderTexture());
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_FINAL]		= std::shared_ptr<RenderTexture>(new  RenderTexture());


	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_DIFFUSE]	->Initialize(device, width, height);
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED]	->Initialize(device, width, height);
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_NORMAL]	->Initialize(device, width, height);
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_WORLD]->Initialize(device, width, height);
	this->m_renderTextures[KEnum::RENDER_TYPE_DEFFERED_FINAL]->Initialize(device, width, height);
	return true;
}
bool GraphicMain::initShaders(ID3D11Device* device, ID3D11DeviceContext *context) {
	


	return true;

}
GraphicMain::GraphicMain()
{
}
bool GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	if (!initShaders(device, context)||
		!initTextures(device,context,width,height)
		
		) return false;
	D3D11_BLEND_DESC noBlack = {};
	noBlack.RenderTarget[0].BlendEnable = true;
	noBlack.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	noBlack.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
	noBlack.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	noBlack.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	noBlack.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	noBlack.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	noBlack.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&noBlack, &m_blendStateNoBlack);

	D3D11_BLEND_DESC transparent = {};
	transparent.RenderTarget[0].BlendEnable = true;
	transparent.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparent.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparent.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparent.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	transparent.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//D3D11_BLEND_ZERO
	transparent.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparent.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&transparent, &m_blendStateTransparent);
	return true;
}
float temp_angle = 0;
void GraphicMain::renderPreDeffered(
	ID3D11DeviceContext * context, NScene::Scene& scene,
	SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
	RenderTexture& texture_diffuse, RenderTexture& texture_normal, DepthTexture& textureDepth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum,ID3D11ShaderResourceView*> *textures, ID3D11SamplerState * sampler)
{
	ID3D11RenderTargetView *renderTargets[]{
		texture_diffuse.getRenderTargetView(),
		texture_normal.getRenderTargetView()
	};


	DirectX::XMFLOAT4X4 world, view, projection;

	DirectX::XMStoreFloat4x4(&world,		XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view,			XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection,	XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(.25f *3.14,m_width, m_height, 0.01f, 100.0f))); // Transpose for HLSL!

	context->RSSetViewports(1, &texture_diffuse.viewport);
	context->OMSetRenderTargets(2, renderTargets, textureDepth.getDepthStencilView());

	textureDepth	.clear(context);
	
	texture_diffuse	.ClearRenderTarget(context, 0, 0.0, 0, 1);
	texture_normal	.ClearRenderTarget(context, 0, 0, 0, 1);

	shader_vert.SetFloat3("worldSize", scene.size);
	shader_vert.SetMatrix4x4("world", world);
	shader_vert.SetMatrix4x4("view", view);
	shader_vert.SetMatrix4x4("projection", projection);
	shader_frag.SetSamplerState("sampler_default", sampler);

	shader_vert.CopyAllBufferData();
	shader_frag.CopyAllBufferData();
	shader_vert.SetShader();
	shader_frag.SetShader();
	int count = 0; //TODO why am I using this?

	for (auto it = scene.objs_solid.begin(); it != scene.objs_solid.end(); it++, count++) {
		if ((*it)->m_ObjectType != OBJ_TYPE_SOLID)
			continue;
		auto mesh = *meshes[(*it)->m_meshId];
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*it)->getModelMatrix())); // Transpose for HLSL!

		shader_vert.SetMatrix4x4("world", world);
		shader_frag.SetShaderResourceView("texture_diffuse",		(*textures)[(*it)->m_textures[TEXTURE_TYPE_DIFFUSE]]);
		shader_frag.SetShaderResourceView("texture_normal",			(*textures)[(*it)->m_textures[TEXTURE_TYPE_NORMAL]]);
		shader_frag.SetShaderResourceView("texture_specular",		(*textures)[(*it)->m_textures[TEXTURE_TYPE_SPECULAR]]);
		shader_frag.SetShaderResourceView("texture_displacement",	(*textures)[(*it)->m_textures[TEXTURE_TYPE_DISPLACEMENT]]);
		shader_vert.CopyAllBufferData();
		shader_frag.CopyAllBufferData();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	shader_frag.SetShaderResourceView("texture_diffuse", 0);
	shader_frag.SetShaderResourceView("texture_normal",0);
	shader_frag.SetShaderResourceView("texture_specular", 0);
	shader_frag.SetShaderResourceView("texture_displacement",0);
}

void GraphicMain::renderUI(
	ID3D11DeviceContext * context, NScene::Scene & scene, 
	SimpleVertexShader & shader_vert, SimpleFragmentShader & shader_frag, 
	RenderTexture & texture_final, DepthTexture& textureDepth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*>* textures, ID3D11SamplerState * sampler)
{
	context->OMSetBlendState(m_blendStateTransparent, 0, 0xffffffff);
	DirectX::XMFLOAT4X4 world, view, projection;

	DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(.25f *3.14, m_width, m_height, 0.01f, 100.0f))); // Transpose for HLSL!
	
	texture_final.SetRenderTarget(context, textureDepth.getDepthStencilView());
	//texture_final.ClearRenderTarget(context,0,0,1,1);
	//textureDepth.clear(context);

	shader_vert.SetMatrix4x4("world", world);
	shader_vert.SetMatrix4x4("view", view);
	shader_vert.SetMatrix4x4("projection", projection);
	shader_frag.SetSamplerState("sampler_default", sampler);

	shader_vert.CopyAllBufferData();
	shader_frag.CopyAllBufferData();
	shader_vert.SetShader();
	shader_frag.SetShader();
	int count = 0;
	for (auto it = scene.objs_ui.begin(); it != scene.objs_ui.end(); it++) {
		if ((*it)->m_ObjectType != OBJ_TYPE_UI)
			continue;
		auto mesh = *meshes[(*it)->m_meshId];
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*it)->getModelMatrix())); // Transpose for HLSL!

		shader_vert.SetMatrix4x4("world", world);
		shader_frag.SetShaderResourceView("texture00", (*textures)[(*it)->m_textures[TEXTURE_TYPE_DEFAULT]]);
		shader_vert.CopyAllBufferData();
		shader_frag.CopyAllBufferData();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}
void GraphicMain::renderLights(
	ID3D11Device * device, ID3D11DeviceContext * context,
	NScene::Scene &scene, SimpleVertexShader & shaderVertDepthOnly, 
	SimpleVertexShader & shaderVert, SimpleFragmentShader & shaderFrag, RenderTexture& target, DepthTexture& targetDepth,
	RenderTexture & textureDiffuse, RenderTexture & textureNormal, DepthTexture & textureDepth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> *textures,
	ID3D11SamplerState * samplerDefault, ID3D11SamplerState * samplerLightDepth)
{
	int count = 0; //TODO delete this
	DirectX::XMFLOAT4X4 world, view, projection;
	auto sceneReverseProjectionView = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(scene.m_camMain.getViewMatrix(), scene.m_camMain.getProjectionMatrix()));
	//auto lightProjectionMatrix = DirectX::XMMatrixOrthographicLH(15, 15, -10.0, 20);
	auto matScreenOrtho = DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);
	target.ClearRenderTarget(context, 0, 0, 0, 1);
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		if ((*it)->m_ObjectType != OBJ_TYPE_LIGHT)continue;
		auto &p = **it;
		auto lightCamera = dynamic_cast<NScene::Light*>(&(p));
		shaderVertDepthOnly.SetShader();
		context->OMSetBlendState(m_blendStateNoBlack, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
		context->PSSetShader(NULL, NULL, 0); //set pixel writing stage to none
		auto lightProjectionMatrix = lightCamera->getProjectionMatrix(.55*3.14f, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE, 0.1, 50);

		float progress = temp_angle;
		

		//lightCamera->setPos(Vector3( sin(angle) * 10, 0, cos(angle)*-10 ));
		auto lightViewProjection = DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix);
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (float)SIZE_LIGHT_TEXTURE;
		viewport.Height = (float)SIZE_LIGHT_TEXTURE;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		context->RSSetViewports(1, &viewport);
		

		auto depth = m_lightDepthTextures[(*it)->m_id];
		if (depth == 0) {//TODO meh...it is going to take less than one second to check this it is dirty though. Think of better ways of doing it if you can.
			m_lightDepthTextures[(*it)->m_id] = new DepthTexture();
			m_lightDepthTextures[(*it)->m_id]->init(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			depth = m_lightDepthTextures[(*it)->m_id];
		}
		context->OMSetRenderTargets(0, 0, depth->getDepthStencilView());
		depth->clear(context);
												  //DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*it)->getModelMatrix())); // Transpose for HLSL!
																									  //auto wtf = cam->getViewMatrix();
		if (temp_angle > 1) {
			int e = 3;
		}
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(lightCamera->getModelMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(lightCamera->getViewMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionMatrix)); // Transpose for HLSL!
																			 //DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(cam->getProjectionMatrix(512,512, 0.1f, 100.0f))); // Transpose for HLSL!


		shaderVertDepthOnly.SetMatrix4x4("world", world);
		shaderVertDepthOnly.SetMatrix4x4("view", view);
		shaderVertDepthOnly.SetMatrix4x4("projection", projection);
		shaderVertDepthOnly.CopyAllBufferData();


		for (auto obj = scene.objs_solid.begin(); obj != scene.objs_solid.end(); obj++) {

			auto mesh = *meshes[(*obj)->m_meshId];
			DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*obj)->getModelMatrix())); // Transpose for HLSL!

			shaderVertDepthOnly.SetMatrix4x4("world", world);
			shaderVertDepthOnly.CopyAllBufferData();
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
			context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

			context->DrawIndexed(
				mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
				0,     // Offset to the first index we want to use
				0);    // Offset to add to each index when looking up vertices
		}
		//orthogonal display here
		target.SetRenderTarget(context, targetDepth.getDepthStencilView());
		targetDepth.clear(context);

		//orthogonal render
		auto pos = Vector3(0, 0, -10);
		auto dir = Vector3(0, 0, 1);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixLookToLH(pos, dir, Vector3::Up), matScreenOrtho))); // Transpose for HLSL!

		shaderVert.SetMatrix4x4("matViewProjection", projection);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(sceneReverseProjectionView)); // Transpose for HLSL!
		shaderFrag.SetMatrix4x4("matProjViewInverse", projection);
		//ok so far
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightViewProjection)); // Transpose for HLSL!
		if (!shaderFrag.SetMatrix4x4("matLightViewProj", projection)) {
			std::cout << "ERER";
		}
		DirectX::XMFLOAT3 storeVector3;
		DirectX::XMStoreFloat3(&storeVector3, DirectX::XMVector3Rotate(Vector3(0, 0, 1), it->get()->m_rotation));
		DirectX::XMFLOAT4 storeVector4;
		DirectX::XMStoreFloat4(&storeVector4, lightCamera->m_lightColor);
		shaderFrag.SetFloat3("lightPos", it->get()->m_pos);
		if (!shaderFrag.SetFloat3("lightDir", it->get()->m_dirLook)  ){
			//std::cout << "lightDir FAIL FAIL FAIL";
		}
		//else std::cout << "succeess " << it->get()->m_dirLook.x  << " " << it->get()->m_dirLook.y << " " << it->get()->m_dirLook.z<<"\n";
		shaderFrag.SetFloat4("lightColor", storeVector4);//	t()->m_pos * Vector3(0, 0, 1));

		//matProjInverse

		shaderFrag.SetShaderResourceView("textureDiffuse", textureDiffuse.GetShaderResourceView());
		shaderFrag.SetShaderResourceView("textureNormal", textureNormal.GetShaderResourceView());
		shaderFrag.SetShaderResourceView("textureDepth", textureDepth.getShaderResourceView());
		if (!shaderFrag.SetShaderResourceView("textureLightDepth", depth->getShaderResourceView())) {
			std::cout << "FAIL";
			//textureLightDepth
		}
		shaderFrag.SetSamplerState("samplerDefault", samplerDefault);
		shaderFrag.SetSamplerState("samplerLight", samplerLightDepth);



		shaderVert.SetShader();
		shaderFrag.SetShader();
		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();

		{
			auto mesh = *meshes[KEnum::MESH_ID_PLANE];
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
			context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(
				mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
				0,     // Offset to the first index we want to use
				0);    // Offset to add to each index when looking up vertices
		}

		shaderFrag.SetShaderResourceView("textureDiffuse", 0);
		shaderFrag.SetShaderResourceView("textureNormal", 0);
		shaderFrag.SetShaderResourceView("textureDepth", 0);
		shaderFrag.SetShaderResourceView("textureLightDepth", 0);
	}

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}
void GraphicMain::render(ID3D11Device * device, ID3D11DeviceContext *context, Asset* asset, NScene::Scene scene)
{
	temp_angle += .051f;
	UINT viewportNum = 1;
	D3D11_VIEWPORT viewport;
	context->RSGetViewports(&viewportNum, &viewport);
	//scene.m_camMain.setPos(Vector3(0,0, temp_angle*.1));
	//scene.m_camMain.setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), temp_angle*0.01f));
	beginRendering();



	//renderPreDeffered(context, scene);
	
	renderPreDeffered(context, scene,
		*asset->m_shadersVert[RENDER_TYPE_DEFFERED], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED],
		*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL],*m_depthTextures[RENDER_TYPE_DEFFERED],
		asset->m_meshes, &asset->m_textures,
		asset->m_samplers[SAMPLER_ID_WRAP]
		);
	renderLights(device,context, 
		scene,
		*asset->m_shadersVert[RENDER_TYPE_DEPTH],
		*asset->m_shadersVert[RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT],
		*m_renderTextures[RENDER_TYPE_DEFFERED_FINAL],* m_depthTextures[RENDER_TYPE_DEFFERED_FINAL],
		*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_depthTextures[RENDER_TYPE_DEFFERED],
		asset->m_meshes, &asset->m_textures,
		asset->m_samplers[SAMPLER_ID_WRAP], asset->m_samplers[SAMPLER_ID_BORDER_ONE]
		);
	renderUI(context, scene,
		*asset->m_shadersVert[RENDER_TYPE_UI], *asset->m_shadersFrag[RENDER_TYPE_UI],
		*m_renderTextures[RENDER_TYPE_DEFFERED_FINAL], *m_depthTextures[RENDER_TYPE_DEFFERED],
		asset->m_meshes, &asset->m_textures,
		asset->m_samplers[SAMPLER_ID_WRAP]
	);
	/*
	*/
	
	//renderLightDepth(device, context, scene);

	endRendering();
	context->RSSetViewports(1, &viewport);
}