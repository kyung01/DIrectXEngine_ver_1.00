#include "Graphic\GraphicMain.h"
#include <string>
#include "Graphic\TextureType.h"

using namespace Graphic;

static int SIZE_LIGHT_TEXTURE = 512;
void Graphic::GraphicMain::processObject(NScene::Object obj) {
}

std::list<MeshLoadInformation> Graphic::GraphicMain::getLoadListMesh()
{
	std::list<MeshLoadInformation> lst({
		{ MESH_TYPE::CONE, "Resource/Mesh/cone.obj" },
		{ MESH_TYPE::CUBE, "Resource/Mesh/cube.obj" },
		{ MESH_TYPE::CYLINDER, "Resource/Mesh/cylinder.obj" },
		{ MESH_TYPE::HELIX, "Resource/Mesh/helix.obj" },
		{ MESH_TYPE::SPHERE, "Resource/Mesh/sphere.obj" },
		{ MESH_TYPE::TORUS, "Resource/Mesh/torus.obj" }
	});
	return lst;
}

std::list<ShaderLoadInformation> Graphic::GraphicMain::getLoadListShaderFrag()
{
	std::list<ShaderLoadInformation> lst({
		{ RENDER_TYPE::DEFAULT,			L"Resource/Shader/default_frag.hlsl" },
		{ RENDER_TYPE::DEFFERED,		L"Resource/Shader/deffered_frag.hlsl" },
		{ RENDER_TYPE::DEPTH,			L"Resource/Shader/depth_frag.hlsl" }
	});
	return lst;
}

std::list<ShaderLoadInformation> Graphic::GraphicMain::getLoadListShaderVert()
{
	std::list<ShaderLoadInformation> lst({	
		{RENDER_TYPE::DEFAULT,			L"Resource/Shader/default_vert.hlsl" },
		{RENDER_TYPE::DEFFERED,			L"Resource/Shader/deffered_vert.hlsl" },
		{RENDER_TYPE::DEPTH,			L"Resource/Shader/depth_vert.hlsl" }
	});
	return lst;
}

std::list<TextureLoadInformation> Graphic::GraphicMain::getLoadListTexture()
{
	std::list<TextureLoadInformation> lst({
		{ TEXTURE_ID::TEXTURE_DEFAULT,		L"Resource/Texture/textureTest00.jpg" },
		{ TEXTURE_ID::TEXTURE_NORMAL_DEFAULT,	L"Resource/Texture/normal_default.jpg" },
		{ TEXTURE_ID::TEXTURE_NORMAL_DIRT,		L"Resource/Texture/normal_dirt.jpg" },
		{ TEXTURE_ID::TEXTURE_NORMAL_WOOD,		L"Resource/Texture/normal_wood.jpg" },
		{ TEXTURE_ID::TEXTURE_NORMAL_BRICK,		L"Resource/Texture/normal_brick.jpg" },
		{ TEXTURE_ID::TEXTURE_NORMAL_ROCK,		L"Resource/Texture/normal_rock.jpg" },//normal_couch.bmp
		{ TEXTURE_ID::TEXTURE_NORMAL_COUCH,		L"Resource/Texture/normal_couch.bmp" },//normal_couch.bmp
		{ TEXTURE_ID::TEXTURE_TEST_00,			L"Resource/Texture/texture_test_00.jpg" } ,
		{ TEXTURE_ID::TEXTURE_TEST_01,			L"Resource/Texture/texture_test_01.jpg" } ,
		{ TEXTURE_ID::TEXTURE_TEST_02,			L"Resource/Texture/texture_test_02.jpg" },
		{ TEXTURE_ID::TEXTURE_TEST_03,			L"Resource/Texture/texture_test_03.jpg" },
		{ TEXTURE_ID::TEXTURE_TEST_04,			L"Resource/Texture/texture_test_04.jpg" },
		{ TEXTURE_ID::TEXTURE_TEST_05,			L"Resource/Texture/texture_test_05.jpg" },
		{ TEXTURE_ID::TEXTURE_TEST_06,			L"Resource/Texture/texture_test_06.jpg" }
	});
	return lst;
}




void Graphic::GraphicMain::processCamera(Graphic::NScene::Camera cam)
{
}

void Graphic::GraphicMain::beginRendering()
{
}

void Graphic::GraphicMain::endRendering()
{
}

void Graphic::GraphicMain::getScreenWidth(int & w, int & h)
{
}


void Graphic::GraphicMain::rendering(NScene::Scene scene)
{

	
}
bool Graphic::GraphicMain::initTextures(ID3D11Device * device, ID3D11DeviceContext * context,int width, int height)
{
	std::cout << "initTextures";

	//auto lightTexture = std::shared_ptr<DepthTexture>(new DepthTexture());
	this->m_depthTextures[RENDER_TYPE::DEFFERED] = std::shared_ptr<DepthTexture>(new DepthTexture());
	this->m_depthTextures[RENDER_TYPE::DEFFERED_FINAL] = std::shared_ptr<DepthTexture>(new DepthTexture());
	this->m_depthTextures[RENDER_TYPE::DEFFERED]->init(device, width, height);
	this->m_depthTextures[RENDER_TYPE::DEFFERED_FINAL]->init(device, width, height);

	this->m_renderTextures[RENDER_TYPE::DEFFERED_DIFFUSE]	= std::shared_ptr<RenderTexture>(new RenderTexture());
	this->m_renderTextures[RENDER_TYPE::DEFFERED]		= std::shared_ptr<RenderTexture>(new RenderTexture());
	this->m_renderTextures[RENDER_TYPE::DEFFERED_NORMAL]	= std::shared_ptr<RenderTexture>(new RenderTexture());
	this->m_renderTextures[RENDER_TYPE::DEFFERED_WORLD]		= std::shared_ptr<RenderTexture>(new  RenderTexture());
	this->m_renderTextures[RENDER_TYPE::DEFFERED_FINAL]		= std::shared_ptr<RenderTexture>(new  RenderTexture());


	this->m_renderTextures[RENDER_TYPE::DEFFERED_DIFFUSE]	->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFFERED]	->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFFERED_NORMAL]	->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFFERED_WORLD]->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFFERED_FINAL]->Initialize(device, width, height);
	return true;
}
bool GraphicMain::initShaders(ID3D11Device* device, ID3D11DeviceContext *context) {
	auto dataVert = getLoadListShaderVert();
	auto dataFrag = getLoadListShaderFrag();
	auto dataMesh = getLoadListMesh();
	auto dataTexture = getLoadListTexture();

	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		m_shadersVert[it->type] = std::shared_ptr<SimpleVertexShader>(new Graphic::SimpleVertexShader(device, context));
		if(!m_shadersVert[it->type]->LoadShaderFileHLSL(it->path, "vs_5_0")) return false;
	}
	for (auto it = dataFrag.begin(); it != dataFrag.end(); it++) {
		m_shadersFrag[it->type] = std::shared_ptr<SimpleFragmentShader>(new Graphic::SimpleFragmentShader(device, context));
		if (!m_shadersFrag[it->type]->LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
	}
	for (auto it = dataMesh.begin(); it != dataMesh.end(); it++) {
		auto mesh = new Mesh(device, it->path);
		m_meshes[it->type] = std::make_unique<Mesh*>(mesh);
	}
	for (auto it = dataTexture.begin(); it != dataTexture.end(); it++) {
		ID3D11ShaderResourceView *texture;
		//DirectX::CreateWICTextureFromFileEx(device,)
		
		DirectX::CreateWICTextureFromFile(device, context,it->path, 0, &texture);
		m_textures[it->id] = texture;
	}

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	//samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	// Ask the device to create a state
	device->CreateSamplerState(&samplerDesc, &m_sampler);

	return true;

}
void Graphic::GraphicMain::render(ID3D11DeviceContext * context, ID3D11DepthStencilView * depth, NScene::Object object)
{
}
Graphic::GraphicMain::GraphicMain()
{
}
bool Graphic::GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height)
{
	this->m_width = width;
	this->m_height = height;
	if (!initShaders(device, context)||
		!initTextures(device,context,width,height)
		
		) return false;

	return true;
}
float temp_angle = 0;
void Graphic::GraphicMain::renderPreDeffered(
	ID3D11DeviceContext * context, NScene::Scene scene, 
	SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
	RenderTexture& texture_diffuse, RenderTexture& texture_normal, DepthTexture& textureDepth)
{
	ID3D11RenderTargetView *renderTargets[]{
		texture_diffuse.getRenderTargetView(),
		texture_normal.getRenderTargetView()
	};


	DirectX::XMFLOAT4X4 world, view, projection;

	DirectX::XMStoreFloat4x4(&world,		XMMatrixTranspose(scene.m_camMain.getModelMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view,			XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection,	XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(m_width, m_height, 0.1f, 100.0f))); // Transpose for HLSL!

	context->OMSetRenderTargets(2, renderTargets, textureDepth.getDepthStencilView());

	textureDepth	.clear(context);
	texture_diffuse	.ClearRenderTarget(context, 0, 0.0, 0, 1);
	texture_normal	.ClearRenderTarget(context, 0, 0, 0, 1);

	shader_vert.SetFloat3("worldSize", scene.size);
	shader_vert.SetMatrix4x4("world", world);
	shader_vert.SetMatrix4x4("view", view);
	shader_vert.SetMatrix4x4("projection", projection);
	shader_frag.SetSamplerState("sampler_default", m_sampler);

	shader_vert.CopyAllBufferData();
	shader_frag.CopyAllBufferData();
	shader_vert.SetShader();
	shader_frag.SetShader();
	int count = 0; //TODO why am I using this?

	for (auto it = scene.objects.begin(); it != scene.objects.end(); it++, count++) {
		if ((*it)->m_ObjectType != NScene::OBJECT_TYPE::SOLID)
			continue;
		auto mesh = *m_meshes[(*it)->m_meshType];
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*it)->getModelMatrix())); // Transpose for HLSL!

		shader_vert.SetMatrix4x4("world", world);
		shader_frag.SetShaderResourceView("texture_diffuse", m_textures[(*it)->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE]]);
		shader_frag.SetShaderResourceView("texture_normal", m_textures[(*it)->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL]]);
		shader_frag.SetShaderResourceView("texture_specular", m_textures[(*it)->m_textures[TEXTURE_TYPE::TEXTURE_SPECULAR]]);
		shader_frag.SetShaderResourceView("texture_displacement", m_textures[(*it)->m_textures[TEXTURE_TYPE::TEXTURE_DISPLACEMENT]]);
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
}
void Graphic::GraphicMain::renderLightDepth(ID3D11Device * device, ID3D11DeviceContext * context, NScene::Scene scene)
{
	auto vertexShader	= *m_shadersVert[RENDER_TYPE::DEPTH];
	vertexShader.SetShader();
	context->PSSetShader(NULL, NULL, 0); //set pixel writing stage to none
	DirectX::XMFLOAT4X4 world, view, projection;

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)512;
	viewport.Height = (float)512;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	for (auto it = scene.objects.begin(); it != scene.objects.end(); it++) {
		if ((*it)->m_ObjectType != NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL)
			continue;

		auto cam = dynamic_cast<NScene::Camera*>(&(**it));
		cam->setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), temp_angle));

		auto depth = m_lightDepthTextures[(*it)->m_id];
		if (depth == 0){//TODO meh...it is going to take less than one second to check this it is dirty though. Think of better ways of doing it if you can.
			m_lightDepthTextures[(*it)->m_id] = new DepthTexture();
			m_lightDepthTextures[(*it)->m_id]->init(device, SIZE_LIGHT_TEXTURE, SIZE_LIGHT_TEXTURE);
			depth = m_lightDepthTextures[(*it)->m_id];
		}
		context->OMSetRenderTargets(0, 0, depth->getDepthStencilView());
		depth->clear(context);

		Vector3 target = (cam)->m_pos + (Vector3)XMVector3Rotate(Vector3::Forward, (cam)->m_rotation);// , 0.0f);
		
		//DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*it)->getModelMatrix())); // Transpose for HLSL!
		//auto wtf = cam->getViewMatrix();
		if (temp_angle > 1) {
			int e = 3;
		}
		//std::cout << "VIEW MATRIX HERE \n" << cam->m_pos.x << " " << cam->m_pos.y << cam->m_pos.z << " and " << cam->m_rotation.x << " " << cam->m_rotation.y << "  " << cam->m_rotation.z << " " << cam->m_rotation.w << "\n";
		//std::cout << "CAM VIEW MATRIX DIRTY" << cam->m_isDirty_matView << "\n";
		/*
		DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(cam->getViewMatrix()));
		DirectX::XMStoreFloat4x4(&view, XMMatrixTranspose(
			DirectX::XMMatrixLookAtLH((*it)->m_pos, target, Vector3::Up))); // Transpose for HLSL!
		*/
		float viewSize = 1+temp_angle;
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(cam->getModelMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&view,  XMMatrixTranspose(cam->getViewMatrix())); // Transpose for HLSL!
		DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(
			DirectX::XMMatrixOrthographicLH(viewSize,viewSize,0.1,100))); // Transpose for HLSL!
		
		//DirectX::XMStoreFloat4x4(&projection, XMMatrixTranspose(cam->getProjectionMatrix(512,512, 0.1f, 100.0f))); // Transpose for HLSL!


		vertexShader.SetMatrix4x4("world", world);
		vertexShader.SetMatrix4x4("view", view);
		vertexShader.SetMatrix4x4("projection", projection);
		vertexShader.CopyAllBufferData();


		for (auto obj = scene.objects.begin(); obj != scene.objects.end(); obj++) {
			if ((*obj)->m_ObjectType != NScene::OBJECT_TYPE::SOLID) continue;
			auto mesh = *m_meshes[(*obj)->m_meshType];
			DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose((*obj)->getModelMatrix())); // Transpose for HLSL!

			vertexShader.SetMatrix4x4("world", world);
			vertexShader.CopyAllBufferData();
			UINT stride = sizeof(Vertex);
			UINT offset = 0;
			context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
			context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

			context->DrawIndexed(
				mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
				0,     // Offset to the first index we want to use
				0);    // Offset to add to each index when looking up vertices
		}




	}
}
void Graphic::GraphicMain::renderDirectionalLight(
	ID3D11Device * device, ID3D11DeviceContext * context, 
	RenderTexture& textureDiffuse, RenderTexture& textureNormal,
	RenderTexture& textureWorld, RenderTexture& textureCameraDepth, RenderTexture& textureFinalScene)
{
	auto shaderVert = *m_shadersVert[RENDER_TYPE::DEFFERED_LIGHT_DIRECTIONAL];
	auto shaderfrag = *m_shadersFrag[RENDER_TYPE::DEFFERED_LIGHT_DIRECTIONAL];


	shaderfrag.SetShaderResourceView("texture_diffuse",	textureDiffuse		.GetShaderResourceView());
	shaderfrag.SetShaderResourceView("texture_normal",		textureNormal		.GetShaderResourceView());
	shaderfrag.SetShaderResourceView("texture_world",		textureWorld		.GetShaderResourceView());
	shaderfrag.SetShaderResourceView("texture_camera",		textureCameraDepth	.GetShaderResourceView());
}
void Graphic::GraphicMain::render(ID3D11Device * device, ID3D11DeviceContext *context, NScene::Scene scene)
{
	temp_angle += .051f;
	UINT viewportNum = 1;
	D3D11_VIEWPORT viewport;
	context->RSGetViewports(&viewportNum, &viewport);
	scene.m_camMain.setPos(Vector3(0,0, temp_angle*.1));
	scene.m_camMain.setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), temp_angle));
	beginRendering();



	//renderPreDeffered(context, scene);
	
	renderPreDeffered(context, scene,
		*m_shadersVert[RENDER_TYPE::DEFFERED], *m_shadersFrag[RENDER_TYPE::DEFFERED],
		*m_renderTextures[RENDER_TYPE::DEFFERED_DIFFUSE], *m_renderTextures[RENDER_TYPE::DEFFERED_NORMAL],*m_depthTextures[RENDER_TYPE::DEFFERED]

		);
	
	//renderLightDepth(device, context, scene);

	endRendering();
	context->RSSetViewports(1, &viewport);
}