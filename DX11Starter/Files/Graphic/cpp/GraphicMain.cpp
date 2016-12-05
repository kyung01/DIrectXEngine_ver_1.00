#include "Graphic\GraphicMain.h"
#include <string>

using namespace NGraphic;

static int SIZE_LIGHT_TEXTURE = 512*2;
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
bool GraphicMain::initTextures(ID3D11Device * device, ID3D11DeviceContext * context,
	int width, int height,
	int textureIndirectLightWidth, int textureIndirectLightHeight)
{
#define HPR_ADD_DEPTH_TEXTURE(key,defWidth, defHeight) \
m_depthTextures[key] = std::shared_ptr<DepthTexture>(new DepthTexture());\
m_depthTextures[key]->init(device, defWidth, defHeight);
#define HPR_ADD_RENDER_TEXTURE(key,defWidth, defHeight) \
this->m_renderTextures[key]	= std::shared_ptr<RenderTexture>(new RenderTexture());\
this->m_renderTextures[key]	->Initialize(device, defWidth, defHeight);
	std::cout << "initTextures";

	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED, width, height);
	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_FINAL, width, height);
	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_DIRECT, width, height);
	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT, textureIndirectLightWidth, textureIndirectLightHeight);
	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY, width, height);
	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR, width, height);
	HPR_ADD_DEPTH_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_BLUR, textureIndirectLightWidth, textureIndirectLightHeight);

	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_DIRECT, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_DIFFUSE, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_NORMAL, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_WORLD, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_FINAL, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_SPECULAR, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT, textureIndirectLightWidth, textureIndirectLightHeight);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR, width, height);
	HPR_ADD_RENDER_TEXTURE(KEnum::RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_BLUR, textureIndirectLightWidth, textureIndirectLightHeight);

	return true;
}
bool GraphicMain::initShaders(ID3D11Device* device, ID3D11DeviceContext *context) {
	


	return true;

}
GraphicMain::GraphicMain()
{
}
bool GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight)
{
	this->m_width = width;
	this->m_height = height;
	m_rsm_flux_eye_perspective_width = textureIndirectLightWidth;
	m_rsm_flux_eye_perspective_height = textureIndirectLightHeight;

	if (!initShaders(device, context)||
		!initTextures(device,context,width,height, textureIndirectLightWidth, textureIndirectLightHeight)
		
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

void NGraphic::GraphicMain::renderSolidObjects(
	ID3D11DeviceContext * context, NScene::Scene & scene, 
	SimpleVertexShader & shaderVert, SimpleFragmentShader & shaderFrag,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures)
{

	DirectX::XMFLOAT4X4 world;

	for (auto it = scene.objs_solid.begin(); it != scene.objs_solid.end(); it++) {
		if ((*it)->m_ObjectType != OBJ_TYPE_SOLID)
			continue;
		auto mesh = *meshes[(*it)->m_meshId];
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*it)->getModelMatrix())); // Transpose for HLSL!

		shaderVert.SetMatrix4x4("world", world);
		shaderFrag.SetShaderResourceView("texture_diffuse", textures[(*it)->m_textures[TEXTURE_TYPE_DIFFUSE]]);
		shaderFrag.SetShaderResourceView("texture_normal", textures[(*it)->m_textures[TEXTURE_TYPE_NORMAL]]);
		shaderFrag.SetShaderResourceView("texture_specular", textures[(*it)->m_textures[TEXTURE_TYPE_SPECULAR]]);
		shaderFrag.SetShaderResourceView("texture_displacement", textures[(*it)->m_textures[TEXTURE_TYPE_DISPLACEMENT]]);
		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}

	shaderFrag.SetShaderResourceView("texture_diffuse", 0);
	shaderFrag.SetShaderResourceView("texture_normal", 0);
	shaderFrag.SetShaderResourceView("texture_specular", 0);
	shaderFrag.SetShaderResourceView("texture_displacement", 0);
}

float temp_angle = 0;
void GraphicMain::renderPreDeffered(
	ID3D11DeviceContext * context, NScene::Scene& scene,
	SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
	RenderTexture& texture_diffuse, RenderTexture& texture_normal, RenderTexture& texture_specular, DepthTexture& textureDepth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures, ID3D11SamplerState * sampler)
{
	ID3D11RenderTargetView *renderTargets[]{
		texture_diffuse.getRenderTargetView(),
		texture_normal.getRenderTargetView(),
		texture_specular.getRenderTargetView()
	};


	DirectX::XMFLOAT4X4 world, view, projection;

	DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(.25f *3.14, m_width, m_height, 0.01f, 100.0f))); // Transpose for HLSL!

	context->RSSetViewports(1, &texture_diffuse.viewport);
	context->OMSetRenderTargets(3, renderTargets, textureDepth.getDepthStencilView());

	textureDepth.clear(context);

	texture_diffuse.ClearRenderTarget(context, 0, 0.0, 0, 1);
	texture_normal.ClearRenderTarget(context, 0, 0, 0, 1);
	texture_specular.ClearRenderTarget(context, 0, 0, 0, 1);

	shader_vert.SetFloat3("worldSize", scene.size);
	shader_vert.SetMatrix4x4("world", world);
	shader_vert.SetMatrix4x4("view", view);
	shader_vert.SetMatrix4x4("projection", projection);
	shader_frag.SetSamplerState("samplerWrap", sampler);

	shader_vert.CopyAllBufferData();
	shader_frag.CopyAllBufferData();
	shader_vert.SetShader();
	shader_frag.SetShader();
	int count = 0; //TODO why am I using this?

	renderSolidObjects(context, scene, shader_vert, shader_frag, meshes, textures);

}

void GraphicMain::renderPreDefferedFake(
	ID3D11DeviceContext * context, NScene::Scene& scene,
	SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
	RenderTexture& texture_diffuse, RenderTexture& texture_normal, RenderTexture& texture_specular, DepthTexture& textureDepth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures, ID3D11SamplerState * sampler)
{
	ID3D11RenderTargetView *renderTargets[]{
		texture_diffuse.getRenderTargetView(),
		texture_normal.getRenderTargetView(),
		texture_specular.getRenderTargetView()
	};


	DirectX::XMFLOAT4X4 world, view, projection;

	DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(.25f *3.14, m_width, m_height, 0.01f, 100.0f))); // Transpose for HLSL!

	context->RSSetViewports(1, &texture_diffuse.viewport);
	context->OMSetRenderTargets(3, renderTargets, textureDepth.getDepthStencilView());



	shader_vert.SetFloat3("worldSize", scene.size);
	shader_vert.SetMatrix4x4("world", world);
	shader_vert.SetMatrix4x4("view", view);
	shader_vert.SetMatrix4x4("projection", projection);
	shader_frag.SetSamplerState("samplerWrap", sampler);

	shader_vert.CopyAllBufferData();
	shader_frag.CopyAllBufferData();
	shader_vert.SetShader();
	shader_frag.SetShader();

	//textureDepth.clear(context);
	//texture_diffuse.ClearRenderTarget(context, 0, 0.0, 0, 1);
	//texture_normal.ClearRenderTarget(context, 0, 0, 0, 1);
	//texture_specular.ClearRenderTarget(context, 0, 0, 0, 1);
	//renderSolidObjects(context, scene, shader_vert, shader_frag, meshes, textures);

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


void GraphicMain::renderLights_directLight(
	ID3D11Device * device, ID3D11DeviceContext * context,
	NScene::Scene &scene,
	SimpleVertexShader & shaderVert, SimpleFragmentShader & shaderFrag,
	RenderTexture& targetDirectLight, DepthTexture& targetDirectDepth,
	RenderTexture & textureDiffuse, RenderTexture & textureNormal, RenderTexture & textureSpecular,
	DepthTexture & textureDepth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures,
	ID3D11SamplerState * samplerDefault, ID3D11SamplerState * samplerLightDepth)
{
	
	DirectX::XMFLOAT4X4 world, view, projection;
	auto sceneReverseProjectionView = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(scene.m_camMain.getViewMatrix(), scene.m_camMain.getProjectionMatrix()));
	//auto lightProjectionMatrix = DirectX::XMMatrixOrthographicLH(15, 15, -10.0, 20);
	auto matScreenOrtho = DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);
	targetDirectLight.ClearRenderTarget(context, 0, 0, 0, 1);
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)SIZE_LIGHT_TEXTURE;
	viewport.Height = (float)SIZE_LIGHT_TEXTURE;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {
		if ((*it)->m_ObjectType != OBJ_TYPE_LIGHT)continue;
		auto &p = **it;
		auto lightCamera = dynamic_cast<NScene::Light*>(&(p));
		if (m_RSM.find((**it).m_id) == m_RSM.end()) {
			m_RSM[(**it).m_id] = {
				std::shared_ptr<RenderTexture>(new RenderTexture()),
				std::shared_ptr<RenderTexture>(new RenderTexture()),
				std::shared_ptr<RenderTexture>(new RenderTexture()),
				std::shared_ptr<DepthTexture>(new DepthTexture()) };
			m_RSM[(**it).m_id].normal.get()->Initialize(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			m_RSM[(**it).m_id].flux.get()->Initialize(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			m_RSM[(**it).m_id].fluxEye.get()->Initialize(device, m_rsm_flux_eye_perspective_width, m_rsm_flux_eye_perspective_height);
			m_RSM[(**it).m_id].depth.get()->init(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
		}
		auto rsm = m_RSM[(**it).m_id]; //Reflective shadow map
		ID3D11RenderTargetView *renderTargets[]{
			rsm.normal.get()->getRenderTargetView(),
			rsm.flux.get()->getRenderTargetView()
		};
		//context->PSSetShader(NULL, NULL, 0); //set pixel writing stage to none
		auto lightProjectionMatrix = lightCamera->getProjectionMatrix(.55*3.14f, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE, 0.1, 50);
		auto lightProjectionView = DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix);
		auto lightProjectionViewInverse = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix));

		//orthogonal display here
		targetDirectLight.SetRenderTarget(context, targetDirectDepth.getDepthStencilView());
		targetDirectDepth.clear(context);

		//orthogonal render
		auto pos = Vector3(0, 0, -10);
		auto dir = Vector3(0, 0, 1);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixLookToLH(pos, dir, Vector3::Up), matScreenOrtho))); // Transpose for HLSL!

		shaderVert.SetMatrix4x4("matViewProjection", projection);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(sceneReverseProjectionView)); // Transpose for HLSL!
		shaderFrag.SetMatrix4x4("matProjViewInverse", projection);
		//ok so far
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionView)); // Transpose for HLSL!
		shaderFrag.SetMatrix4x4("matLightProjView", projection);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionViewInverse)); // Transpose for HLSL!
		shaderFrag.SetMatrix4x4("matLightProjViewInverse", projection);
		shaderFrag.SetFloat3("lightPos", it->get()->m_pos);
		shaderFrag.SetFloat3("lightDir", it->get()->m_dirLook);
		shaderFrag.SetFloat4("lightColor", lightCamera->m_lightColor);//	t()->m_pos * Vector3(0, 0, 1));

																	  //matProjInverse

		shaderFrag.SetShaderResourceView("textureDiffuse", textureDiffuse.getShaderResourceView());
		shaderFrag.SetShaderResourceView("textureNormal", textureNormal.getShaderResourceView());
		shaderFrag.SetShaderResourceView("textureDepth", textureDepth.getShaderResourceView());
		shaderFrag.SetShaderResourceView("textureSpecular", textureSpecular.getShaderResourceView());
		shaderFrag.SetShaderResourceView("textureLightRSM", rsm.flux->getShaderResourceView());
		shaderFrag.SetShaderResourceView("textureLightNormal", rsm.normal->getShaderResourceView());
		shaderFrag.SetShaderResourceView("textureLightDepth", rsm.depth->getShaderResourceView());
		shaderFrag.SetSamplerState("samplerDefault", samplerDefault);
		shaderFrag.SetSamplerState("samplerLight", samplerLightDepth);



		shaderVert.SetShader();
		shaderFrag.SetShader();
		shaderVert.CopyAllBufferData();
		shaderFrag.CopyAllBufferData();

		context->OMSetBlendState(m_blendStateNoBlack, 0, 0xffffffff);
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
		shaderFrag.SetShaderResourceView("textureSpecular", 0);

		shaderFrag.SetShaderResourceView("textureLightRSM",0);
		shaderFrag.SetShaderResourceView("textureLightNormal",0);
		shaderFrag.SetShaderResourceView("textureLightDepth", 0);
	}

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}


void GraphicMain::renderLights_indirect(
	ID3D11Device * device, ID3D11DeviceContext * context,
	NScene::Scene &scene,
	SimpleVertexShader& shaderVertIndirectLight, SimpleFragmentShader & shaderFragIndirectLight,
	RenderTexture& targetIndirectLight, DepthTexture& targetIndirectDepth,
	RenderTexture & textureDiffuse, RenderTexture & textureNormal, RenderTexture & textureSpecular,
	DepthTexture & textureDepth,DepthTexture& textureLightIndirectDpeth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures,
	ID3D11SamplerState * samplerDefault, ID3D11SamplerState * samplerLightDepth, ID3D11SamplerState * samplerLightRSM)
{
	int count = 0; //TODO delete this
	DirectX::XMFLOAT4X4 world, view, projection;
	auto sceneReverseProjectionView = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(scene.m_camMain.getViewMatrix(), scene.m_camMain.getProjectionMatrix()));
	auto matScreenOrtho = DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);
	

	targetIndirectLight.SetRenderTarget(context, targetIndirectDepth.getDepthStencilView());
	targetIndirectLight.ClearRenderTarget(context, 1, 1, 0, 0);
	context->OMSetBlendState(m_blendStateNoBlack, 0, 0xffffffff);
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {

		auto lightCamera = dynamic_cast<NScene::Light*>(&(**it));
	
		auto rsm = m_RSM[(**it).m_id]; //Reflective shadow map
		auto lightProjectionMatrix = lightCamera->getProjectionMatrix(.55*3.14f, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE, 0.1, 50);
		auto lightProjectionView = DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix);
		auto lightProjectionViewInverse = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix));
		
		targetIndirectDepth.clear(context);

		//orthogonal render
		auto pos = Vector3(0, 0, -10);
		auto dir = Vector3(0, 0, 1);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixLookToLH(pos, dir, Vector3::Up), matScreenOrtho))); // Transpose for HLSL!

		shaderVertIndirectLight.SetMatrix4x4("matViewProjection", projection);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(sceneReverseProjectionView)); // Transpose for HLSL!
		shaderFragIndirectLight.SetMatrix4x4("matProjViewInverse", projection);
		
		//ok so far
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionView)); // Transpose for HLSL!
		if (!shaderFragIndirectLight.SetMatrix4x4("matLightProjView", projection)) {
			std::cout << "ERER";
		}
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionViewInverse)); // Transpose for HLSL!
		if (!shaderFragIndirectLight.SetMatrix4x4("matLightProjViewInverse", projection)) {
			std::cout << "ERER";
		}
		//DirectX::XMStoreFloat3(&storeVector3, DirectX::XMVector3Rotate(Vector3(0, 0, 1), it->get()->m_rotation));

		shaderFragIndirectLight.SetFloat3("lightPos", it->get()->m_pos);
		if (!shaderFragIndirectLight.SetFloat3("lightDir", it->get()->m_dirLook)) {
			//std::cout << "lightDir FAIL FAIL FAIL";
		}
		//DirectX::XMStoreFloat4(&storeVector4, lightCamera->m_lightColor);
		//else std::cout << "succeess " << it->get()->m_dirLook.x  << " " << it->get()->m_dirLook.y << " " << it->get()->m_dirLook.z<<"\n";
		shaderFragIndirectLight.SetFloat4("lightColor", lightCamera->m_lightColor);//	t()->m_pos * Vector3(0, 0, 1));

																	  //matProjInverse

		shaderFragIndirectLight.SetShaderResourceView("textureDiffuse", textureDiffuse.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureNormal", textureNormal.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureDepth", textureDepth.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureSpecular", textureSpecular.getShaderResourceView());
		if (!shaderFragIndirectLight.SetShaderResourceView("textureLightDepth", rsm.depth->getShaderResourceView())) {
			std::cout << "FAIL";
			//textureLightDepth
		}
		shaderFragIndirectLight.SetShaderResourceView("textureLightRSM", rsm.flux->getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureLightNormal", rsm.normal->getShaderResourceView());

		shaderFragIndirectLight.SetSamplerState("samplerDefault", samplerDefault);
		shaderFragIndirectLight.SetSamplerState("samplerLight", samplerLightDepth);
		shaderFragIndirectLight.SetSamplerState("samplerLightRSM", samplerLightRSM);



		shaderVertIndirectLight.SetShader();
		shaderFragIndirectLight.SetShader();
		shaderVertIndirectLight.CopyAllBufferData();
		shaderFragIndirectLight.CopyAllBufferData();

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

		shaderFragIndirectLight.SetShaderResourceView("textureDiffuse", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureNormal", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureDepth", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureSpecular", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureLightDepth", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureLightRSM", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureLightNormal",0);
	}

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}

void NGraphic::GraphicMain::renderLightsFailCases(
	ID3D11Device* device, ID3D11DeviceContext* context,
	NScene::Scene &scene,
	//SimpleVertexShader& shaderVertLight, SimpleFragmentShader & shaderFragLight,
	SimpleVertexShader& shaderVertIndirectLight, SimpleFragmentShader & shaderFragIndirectLight,
	//SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag,

	//RenderTexture& target, DepthTexture& targetDepth,
	RenderTexture& targetIndirectLight, DepthTexture& targetIndirectDepth,
	RenderTexture & textureError, RenderTexture & textureDirectLight,
	RenderTexture& textureDiffuse, RenderTexture& textureNormal, RenderTexture & textureSpecular,
	DepthTexture& textureDepth, DepthTexture& textureLightIndirectDpeth,
	std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures,
	ID3D11SamplerState * samplerDefault, ID3D11SamplerState * samplerLightDepth, ID3D11SamplerState * samplerLightRSM
)
{
	int count = 0; //TODO delete this
	DirectX::XMFLOAT4X4 world, view, projection;
	auto sceneReverseProjectionView = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(scene.m_camMain.getViewMatrix(), scene.m_camMain.getProjectionMatrix()));
	auto matScreenOrtho = DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);


	targetIndirectLight.SetRenderTarget(context, targetIndirectDepth.getDepthStencilView());
	//targetIndirectLight.ClearRenderTarget(context, 0, 0, 0, 0);
	context->OMSetBlendState(m_blendStateNoBlack, 0, 0xffffffff);
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {

		auto lightCamera = dynamic_cast<NScene::Light*>(&(**it));

		auto rsm = m_RSM[(**it).m_id]; //Reflective shadow map
		auto lightProjectionMatrix = lightCamera->getProjectionMatrix(.55*3.14f, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE, 0.1, 50);
		auto lightProjectionView = DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix);
		auto lightProjectionViewInverse = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(lightCamera->getViewMatrix(), lightProjectionMatrix));

		targetIndirectDepth.clear(context);

		//orthogonal render
		auto pos = Vector3(0, 0, -10);
		auto dir = Vector3(0, 0, 1);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(DirectX::XMMatrixMultiply(
			DirectX::XMMatrixLookToLH(pos, dir, Vector3::Up), matScreenOrtho))); // Transpose for HLSL!

		shaderVertIndirectLight.SetMatrix4x4("matViewProjection", projection);
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(sceneReverseProjectionView)); // Transpose for HLSL!
		shaderFragIndirectLight.SetMatrix4x4("matProjViewInverse", projection);

		//ok so far
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionView)); // Transpose for HLSL!
		if (!shaderFragIndirectLight.SetMatrix4x4("matLightProjView", projection)) {
			std::cout << "ERER";
		}
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(lightProjectionViewInverse)); // Transpose for HLSL!
		if (!shaderFragIndirectLight.SetMatrix4x4("matLightProjViewInverse", projection)) {
			std::cout << "ERER";
		}
		//DirectX::XMStoreFloat3(&storeVector3, DirectX::XMVector3Rotate(Vector3(0, 0, 1), it->get()->m_rotation));

		shaderFragIndirectLight.SetFloat3("lightPos", it->get()->m_pos);
		if (!shaderFragIndirectLight.SetFloat3("lightDir", it->get()->m_dirLook)) {
			//std::cout << "lightDir FAIL FAIL FAIL";
		}
		//DirectX::XMStoreFloat4(&storeVector4, lightCamera->m_lightColor);
		//else std::cout << "succeess " << it->get()->m_dirLook.x  << " " << it->get()->m_dirLook.y << " " << it->get()->m_dirLook.z<<"\n";
		shaderFragIndirectLight.SetFloat4("lightColor", lightCamera->m_lightColor);//	t()->m_pos * Vector3(0, 0, 1));

																				   //matProjInverse

		shaderFragIndirectLight.SetShaderResourceView("textureError", textureError.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureDirectLight", textureDirectLight.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureDiffuse", textureDiffuse.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureNormal", textureNormal.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureDepth", textureDepth.getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureSpecular", textureSpecular.getShaderResourceView());
		if (!shaderFragIndirectLight.SetShaderResourceView("textureLightDepth", rsm.depth->getShaderResourceView())) {
			std::cout << "FAIL";
			//textureLightDepth
		}
		shaderFragIndirectLight.SetShaderResourceView("textureLightRSM", rsm.flux->getShaderResourceView());
		shaderFragIndirectLight.SetShaderResourceView("textureLightNormal", rsm.normal->getShaderResourceView());

		shaderFragIndirectLight.SetSamplerState("samplerDefault", samplerDefault);
		shaderFragIndirectLight.SetSamplerState("samplerLight", samplerLightDepth);
		shaderFragIndirectLight.SetSamplerState("samplerLightRSM", samplerLightRSM);



		shaderVertIndirectLight.SetShader();
		shaderFragIndirectLight.SetShader();
		shaderVertIndirectLight.CopyAllBufferData();
		shaderFragIndirectLight.CopyAllBufferData();

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

		shaderFragIndirectLight.SetShaderResourceView("textureError", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureDirectLight", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureDiffuse", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureNormal", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureDepth", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureSpecular", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureLightDepth", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureLightRSM", 0);
		shaderFragIndirectLight.SetShaderResourceView("textureLightNormal", 0);
	}

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}

void GraphicMain::renderIndirectTextureBlur(
	ID3D11Device* device, ID3D11DeviceContext* context, NScene::Scene & scene,
	SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag,

	RenderTexture& target, DepthTexture& targetDepth,
	RenderTexture & textureBlurred,
	RenderTexture& textureNormal, RenderTexture & textureSpecular, DepthTexture& textureDepth,
	std::unique_ptr<Mesh*> &meshePlane,
	ID3D11SamplerState * samplerDefault,
	ID3D11SamplerState * samplerLinear
	) {

	DirectX::XMFLOAT4X4 world, view, projection;
	auto sceneReverseProjectionView = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(scene.m_camMain.getViewMatrix(), scene.m_camMain.getProjectionMatrix()));
	auto matScreenOrtho = DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);
	context->OMSetBlendState(m_blendStateNoBlack, 0, 0xffffffff);

	//orthogonal display here
	target.SetRenderTarget(context, targetDepth.getDepthStencilView());
	target.ClearRenderTarget(context, 0, 0, 0, 0);
	targetDepth.clear(context);

	//orthogonal render
	auto pos = Vector3(0, 0, -10);
	auto dir = Vector3(0, 0, 1);
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(DirectX::XMMatrixMultiply(
		DirectX::XMMatrixLookToLH(pos, dir, Vector3::Up), matScreenOrtho))); // Transpose for HLSL!

	shaderVert.SetMatrix4x4("matViewProjection", projection);
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(sceneReverseProjectionView)); // Transpose for HLSL!
	shaderFrag.SetMatrix4x4("matProjViewInverse", projection);


	shaderFrag.SetFloat("PIXEL_DISTANCE", 1.0 / m_rsm_flux_eye_perspective_width);
	shaderFrag.SetShaderResourceView("textureTarget", textureBlurred.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureNormal", textureNormal.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureSpecular", textureSpecular.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureDepth", textureDepth.getShaderResourceView());

	shaderFrag.SetSamplerState("samplerDefault", samplerDefault);
	shaderFrag.SetSamplerState("samplerIndirectLight", samplerLinear);




	shaderVert.SetShader();
	shaderFrag.SetShader();
	shaderVert.CopyAllBufferData();
	shaderFrag.CopyAllBufferData();

	{
		auto mesh = *meshePlane;
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
	//TODO you gotta do it at the end
	//shaderFrag.SetShaderResourceView("textureDiffuse", 0);
	//shaderFrag.SetShaderResourceView("textureNormal", 0);
	//shaderFrag.SetShaderResourceView("textureDepth", 0);
	//shaderFrag.SetShaderResourceView("textureLightDepth", 0);
	//shaderFrag.SetShaderResourceView("textureSpecular", 0);

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}
void NGraphic::GraphicMain::renderApplyDirectAndIndirectLights(
	ID3D11Device * device, ID3D11DeviceContext * context, NScene::Scene & scene,
	SimpleVertexShader & shaderVert, SimpleFragmentShader & shaderFrag, 
	RenderTexture & target, RenderTexture & targetIndirectLightError, DepthTexture & targetDepth,
	RenderTexture & textureDirectLight, RenderTexture & textureIndirectLight, 
	RenderTexture & textureNormal, RenderTexture & textureSpecular, 
	DepthTexture & textureDepth,
	std::unique_ptr<Mesh*>& meshePlane, 
	ID3D11SamplerState * samplerDefault,ID3D11SamplerState * samplerLinear)
{
	ID3D11RenderTargetView *renderTargets[]{
		target.getRenderTargetView(),
		targetIndirectLightError.getRenderTargetView()
	};


	context->RSSetViewports(1, &target.viewport);
	context->OMSetRenderTargets(2, renderTargets, targetDepth.getDepthStencilView());
	target.ClearRenderTarget(context, 0, 0, 0, 0);
	targetIndirectLightError.ClearRenderTarget(context, 0, 0, 0, 0);
	targetDepth.clear(context);

	DirectX::XMFLOAT4X4 world, view, projection;
	auto sceneReverseProjectionView = DirectX::XMMatrixInverse(0, DirectX::XMMatrixMultiply(scene.m_camMain.getViewMatrix(), scene.m_camMain.getProjectionMatrix()));
	auto matScreenOrtho = DirectX::XMMatrixOrthographicLH(1, 1, 0.0, 100);
	context->OMSetBlendState(m_blendStateNoBlack, 0, 0xffffffff);

	//orthogonal display here
	//target.SetRenderTarget(context, targetDepth.getDepthStencilView());
	

	//orthogonal render
	auto pos = Vector3(0, 0, -10);
	auto dir = Vector3(0, 0, 1);
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(DirectX::XMMatrixMultiply(
		DirectX::XMMatrixLookToLH(pos, dir, Vector3::Up), matScreenOrtho))); // Transpose for HLSL!

	
	
	
	
	shaderVert.SetMatrix4x4("matViewProjection", projection);
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(sceneReverseProjectionView)); // Transpose for HLSL!
	shaderFrag.SetMatrix4x4("matProjViewInverse", projection);

	shaderFrag.SetShaderResourceView("textureNormal", textureNormal.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureDepth", textureDepth.getShaderResourceView());

	shaderFrag.SetShaderResourceView("textureSpecular", textureSpecular.getShaderResourceView());
	shaderFrag.SetShaderResourceView("textureLightDirect", textureDirectLight.getShaderResourceView()); //TODO delete this
	shaderFrag.SetShaderResourceView("textureLightIndirect", textureIndirectLight.getShaderResourceView());

	shaderFrag.SetSamplerState("samplerDefault", samplerDefault);
	shaderFrag.SetSamplerState("samplerIndirectLight", samplerLinear);

	shaderVert.SetShader();
	shaderFrag.SetShader();
	shaderVert.CopyAllBufferData();
	shaderFrag.CopyAllBufferData();
	{
		auto mesh = *meshePlane;
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(
			mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}



	shaderFrag.SetShaderResourceView("textureNormal", 0);
	shaderFrag.SetShaderResourceView("textureDepth", 0);
	shaderFrag.SetShaderResourceView("textureSpecular", 0);
	shaderFrag.SetShaderResourceView("textureLightDirect", 0); //TODO delete this
	shaderFrag.SetShaderResourceView("textureLightIndirect", 0);

	context->OMSetBlendState(0, 0, 0xffffffff);//::NoBlack, blendFactor, 0xffffffff);
}

void NGraphic::GraphicMain::renderLightIndirect_FirstBounce(
	ID3D11Device * device, ID3D11DeviceContext * context,
	NScene::Scene & scene, NScene::Light light,
	SimpleVertexShader & shaderVert, SimpleFragmentShader & shaderFrag,
	RenderTexture & targetColor, RenderTexture & targetNormal,
	DepthTexture & depthTexture, std::map<KEnum, std::unique_ptr<Mesh*>>& meshes,
	std::map<KEnum, ID3D11ShaderResourceView*>& textures, ID3D11SamplerState * samplerClamp)
{
	DirectX::XMFLOAT4X4 world, view, projection;

	//auto lightProjectionMatrix = lightCamera->getProjectionMatrix(.55*3.14f, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE, 0.1, 50);
	DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(light.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(light.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(light.getProjectionMatrix()) ); 


	shaderVert.SetMatrix4x4("world", world);
	shaderVert.SetMatrix4x4("view", view);
	shaderVert.SetMatrix4x4("projection", projection);

	shaderFrag.SetFloat3("lightPos", light.m_pos);
	shaderFrag.SetFloat3("lightDir", light.m_dirLook);
	shaderFrag.SetFloat4("lightColor", light.m_lightColor);//	t()->m_pos * Vector3(0, 0, 1));
	shaderFrag.SetSamplerState("samplerDefault", samplerClamp);
	shaderVert.SetShader();
	shaderFrag.SetShader();


	ID3D11RenderTargetView *renderTargets[]{
		targetColor.getRenderTargetView(),
		targetNormal.getRenderTargetView()
	};

	context->OMSetRenderTargets(2, renderTargets, depthTexture.getDepthStencilView());
	context->RSSetViewports(1, &targetColor.viewport);
	targetColor.ClearRenderTarget(context, 0, 0, 0, 1);
	targetNormal.ClearRenderTarget(context, 0, 0, 0, 1);
	depthTexture.clear(context);




	context->OMSetBlendState(0, 0, 0xffffffff);
	renderSolidObjects(context, scene, shaderVert, shaderFrag, meshes, textures);

}
void GraphicMain::render(ID3D11Device * device, ID3D11DeviceContext *context, Asset* asset, NScene::Scene scene)
{
	for (auto it = scene.objs_lights.begin(); it != scene.objs_lights.end(); it++) {

		if (m_RSM.find((**it).m_id) == m_RSM.end()) {
			m_RSM[(**it).m_id] = {
				std::shared_ptr<RenderTexture>(new RenderTexture()),
				std::shared_ptr<RenderTexture>(new RenderTexture()),
				std::shared_ptr<RenderTexture>(new RenderTexture()),
				std::shared_ptr<DepthTexture>(new DepthTexture()) };
			m_RSM[(**it).m_id].normal.get()->Initialize(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			m_RSM[(**it).m_id].flux.get()->Initialize(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			m_RSM[(**it).m_id].fluxEye.get()->Initialize(device, m_rsm_flux_eye_perspective_width, m_rsm_flux_eye_perspective_height);
			m_RSM[(**it).m_id].depth.get()->init(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
		}
	}
		
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
		*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_renderTextures[RENDER_TYPE_DEFFERED_SPECULAR],
		*m_depthTextures[RENDER_TYPE_DEFFERED],
		asset->m_meshes, asset->m_textures,
		asset->m_samplers[SAMPLER_ID_WRAP]
	);
	renderLights_directLight(device, context, scene,
		*asset->m_shadersVert[RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT],
	
		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_DIRECT], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_DIRECT],
		//*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_renderTextures[RENDER_TYPE_DEFFERED_SPECULAR],
		*m_depthTextures[RENDER_TYPE_DEFFERED], //*m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		asset->m_meshes, asset->m_textures,
		asset->m_samplers[SAMPLER_ID_CLAMP], asset->m_samplers[SAMPLER_ID_BORDER_ONE]//, asset->m_samplers[SAMPLER_ID_BORDER_ZERO]
	);
	for (auto light = scene.objs_lights.begin(); light != scene.objs_lights.end(); light++) {
		auto rsm = this->m_RSM[light->get()->m_id];
		renderLightIndirect_FirstBounce(device, context, scene, **light, *asset->m_shadersVert[RENDER_TYPE_LIGHT_RSM], *asset->m_shadersFrag[RENDER_TYPE_LIGHT_RSM],
			*rsm.flux, *rsm.normal, *rsm.depth, asset->m_meshes, asset->m_textures, asset->m_samplers[SAMPLER_ID_CLAMP]
		);
	}

	renderLights_indirect(device, context, scene,
		*asset->m_shadersVert[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_renderTextures[RENDER_TYPE_DEFFERED_SPECULAR],
		*m_depthTextures[RENDER_TYPE_DEFFERED], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		asset->m_meshes, asset->m_textures,
		asset->m_samplers[SAMPLER_ID_CLAMP], asset->m_samplers[SAMPLER_ID_BORDER_ONE], asset->m_samplers[SAMPLER_ID_BORDER_ZERO]
	);

	renderApplyDirectAndIndirectLights(device, context, scene,
		*asset->m_shadersVert[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY],

		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY], *m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR], 
		*m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY],

		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_DIRECT], 
		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		
		*m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_renderTextures[RENDER_TYPE_DEFFERED_SPECULAR] ,
		*m_depthTextures[RENDER_TYPE_DEFFERED],
		asset->m_meshes[KEnum::MESH_ID_PLANE],
		asset->m_samplers[SAMPLER_ID_CLAMP],
		asset->m_samplers[SAMPLER_ID_LINEAR]
	);

	//renderLights_indirect(device, context, scene,
	//	*asset->m_shadersVert[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY],
	//	*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
	//	*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_renderTextures[RENDER_TYPE_DEFFERED_SPECULAR],
	//	*m_depthTextures[RENDER_TYPE_DEFFERED], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
	//	asset->m_meshes, asset->m_textures,
	//	asset->m_samplers[SAMPLER_ID_CLAMP], asset->m_samplers[SAMPLER_ID_BORDER_ONE], asset->m_samplers[SAMPLER_ID_BORDER_ZERO]
	//);
	//
	//*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR], *m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_DIRECT],
	renderLightsFailCases(device, context, scene,
		*asset->m_shadersVert[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR], *asset->m_shadersFrag[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR],
		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_APPLY],
		*m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT_ERROR], *m_renderTextures[RENDER_TYPE_DEFFERED_LIGHT_DIRECT],
		*m_renderTextures[RENDER_TYPE_DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE_DEFFERED_NORMAL], *m_renderTextures[RENDER_TYPE_DEFFERED_SPECULAR],
		*m_depthTextures[RENDER_TYPE_DEFFERED], *m_depthTextures[RENDER_TYPE_DEFFERED_LIGHT_INDIRECT],
		asset->m_meshes, asset->m_textures,
		asset->m_samplers[SAMPLER_ID_CLAMP], asset->m_samplers[SAMPLER_ID_BORDER_ONE], asset->m_samplers[SAMPLER_ID_BORDER_ZERO]
	);

	//renderUI(context, scene,
	//	*asset->m_shadersVert[RENDER_TYPE_UI], *asset->m_shadersFrag[RENDER_TYPE_UI],
	//	*m_renderTextures[RENDER_TYPE_DEFFERED_FINAL], *m_depthTextures[RENDER_TYPE_DEFFERED],
	//	asset->m_meshes, &asset->m_textures,
	//	asset->m_samplers[SAMPLER_ID_CLAMP]
	//);
	
	//renderLightDepth(device, context, scene);

	//context->OMSetRenderTargets(1, 0, 0);
	endRendering();
	context->RSSetViewports(1, &viewport);
}