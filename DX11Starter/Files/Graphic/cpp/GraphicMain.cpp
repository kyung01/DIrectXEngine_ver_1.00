#include "Graphic\GraphicMain.h"
#include <string>
#include "Graphic\TextureType.h"

using namespace Graphic;

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

std::list<ShaderLoadInformation> Graphic::GraphicMain::getLoadListShaderVert()
{
	std::list<ShaderLoadInformation> lst(
	{	{ RENDER_TYPE::DEFAULT,			L"Resource/default_vert.hlsl" },
		{ RENDER_TYPE::DEFFERED_DIFFUSE,L"Resource/default_vert.hlsl" } ,
		{ RENDER_TYPE::DEFFERED_NORMAL,	L"Resource/default_vert.hlsl" } ,
		{ RENDER_TYPE::DEFFERED_WORLD,	L"Resource/default_vert.hlsl" },
		{ RENDER_TYPE::DEFERRED_DEPTH,	L"Resource/default_vert.hlsl" },
		{ RENDER_TYPE::OTHER,			L"Resource/default_vert.hlsl" }
	});
	return lst;
}

std::list<ShaderLoadInformation> Graphic::GraphicMain::getLoadListShaderFrag()
{
	std::list<ShaderLoadInformation> lst(
	{ { RENDER_TYPE::DEFAULT,			L"Resource/default_frag.hlsl" },
	{ RENDER_TYPE::DEFFERED_DIFFUSE,	L"Resource/default_frag.hlsl" } ,
	{ RENDER_TYPE::DEFFERED_NORMAL,		L"Resource/default_frag.hlsl" } ,
	{ RENDER_TYPE::DEFFERED_WORLD,		L"Resource/default_frag.hlsl" },
	{ RENDER_TYPE::DEFERRED_DEPTH,		L"Resource/default_frag.hlsl" },
	{ RENDER_TYPE::OTHER,				L"Resource/default_frag.hlsl" }
	});
	return lst;
}

std::list<TextureLoadInformation> Graphic::GraphicMain::getLoadListTexture()
{
	std::list<TextureLoadInformation> lst({ 
		{ TEXTURE_ID::TEXTURE_DEFAULT,		L"Resource/Texture/textureTest00.jpg" },
		{ TEXTURE_ID::TEXTURE_A,			L"Resource/Texture/normalTexture00.png" } ,
		{ TEXTURE_ID::TEXTURE_B,			L"Resource/Texture/normalTexture00.png" } ,
		{ TEXTURE_ID::TEXTURE_C,			L"Resource/Texture/textureTest00.jpg" },
		{ TEXTURE_ID::TEXTURE_D,			L"Resource/Texture/textureTest00.jpg" }
	});
	return lst;
}




void Graphic::GraphicMain::processCamera(Camera cam)
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
	this->m_renderTextures[RENDER_TYPE::DEFFERED_DIFFUSE]	= new RenderTexture();
	this->m_renderTextures[RENDER_TYPE::DEFERRED_DEPTH]	= new RenderTexture();
	this->m_renderTextures[RENDER_TYPE::DEFFERED_NORMAL]	= new RenderTexture();
	this->m_renderTextures[RENDER_TYPE::DEFFERED_WORLD]	= new  RenderTexture();

	this->m_renderTextures[RENDER_TYPE::DEFFERED_DIFFUSE]	->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFERRED_DEPTH]	->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFFERED_NORMAL]	->Initialize(device, width, height);
	this->m_renderTextures[RENDER_TYPE::DEFFERED_WORLD]	->Initialize(device, width, height);

	return true;
}
bool GraphicMain::initShaders(ID3D11Device* device, ID3D11DeviceContext *context) {
	auto dataVert = getLoadListShaderVert();
	auto dataFrag = getLoadListShaderFrag();
	auto dataMesh = getLoadListMesh();
	auto dataTexture = getLoadListTexture();

	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		auto shader = new Graphic::SimpleVertexShader(device, context);
		if(!shader->LoadShaderFileHLSL(it->path, "vs_5_0")) return false;
		m_shadersVert[it->type] = std::make_unique<SimpleVertexShader*>(shader);
	}
	for (auto it = dataFrag.begin(); it != dataFrag.end(); it++) {
		auto shader = new Graphic::SimpleFragmentShader(device, context);
		if (!shader->LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
		m_shadersFrag[it->type] = std::make_unique<SimpleFragmentShader*>(shader);
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
void Graphic::GraphicMain::render(ID3D11DeviceContext *context, ID3D11DepthStencilView *depth, NScene::Scene scene)
{
	temp_angle += .01f;
	UINT viewportNum = 1;
	D3D11_VIEWPORT viewport;
	context->RSGetViewports(&viewportNum, &viewport);
	scene.m_camMain.setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), temp_angle) );
	beginRendering();

	auto renderTarget_diffuse	= m_renderTextures[RENDER_TYPE::DEFFERED_DIFFUSE];
	auto renderTarget_normal	= m_renderTextures[RENDER_TYPE::DEFFERED_NORMAL];
	auto renderTarget_world		= m_renderTextures[RENDER_TYPE::DEFFERED_WORLD];
	auto renderTarget_depth = m_renderTextures[RENDER_TYPE::DEFERRED_DEPTH];
	ID3D11RenderTargetView *renderTargets[]{ 
		renderTarget_diffuse->m_renderTargetView,
		renderTarget_normal->m_renderTargetView ,
		renderTarget_world->m_renderTargetView,
		renderTarget_depth->m_renderTargetView
	};

	auto vertexShader = *m_shadersVert.begin()->second;
	auto pixelShader = *m_shadersFrag.begin()->second;


	DirectX::XMFLOAT4X4 world,view,projection;

	DirectX::XMStoreFloat4x4(&world,		XMMatrixTranspose(scene.m_camMain.getModelMatrix() )); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view,			XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection,	XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(m_width, m_height, 0.1f, 100.0f))); // Transpose for HLSL!

	renderTarget_diffuse->SetRenderTarget(context, depth);
	context->OMSetRenderTargets(4, renderTargets, depth);
	//deviceContext->OMSetRenderTargets(1, &m_renderTargetView, depthStencilView);
	renderTarget_diffuse->ClearRenderTarget(context, depth, 255, 0.0, 0, 1);
	renderTarget_normal->ClearRenderTarget(context, depth, 0, 255, 0, 1);
	renderTarget_world->ClearRenderTarget(context, depth, 0, 0.0, 255, 1);
	renderTarget_depth->ClearRenderTarget(context, depth, 0, 0.0, 0, 255);

	vertexShader->SetFloat3("worldSize", scene.size);
	vertexShader->SetMatrix4x4("world", world);
	vertexShader->SetMatrix4x4("view", view);
	vertexShader->SetMatrix4x4("projection", projection);
	pixelShader->SetSamplerState("sampler_default", m_sampler);

	vertexShader->CopyAllBufferData();
	pixelShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
	int count = 0;
	
	for (auto it = scene.objects.begin(); it != scene.objects.end(); it++, count++) {
		render(context,depth,*it);
		//XMStoreFloat4x4(&worldMatrix_temp, XMMatrixTranspose(XMLoadFloat4x4(&it->pos))); // Transpose for HLSL!
		auto mesh = *m_meshes[it->m_meshType];
		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(it->getModelMatrix())); // Transpose for HLSL!

		vertexShader->SetMatrix4x4("world", world);
		pixelShader->SetShaderResourceView("texture_diffuse", m_textures[it->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE]]);
		pixelShader->SetShaderResourceView("texture_normal", m_textures[it->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL]]);
		pixelShader->SetShaderResourceView("texture_specular", m_textures[it->m_textures[TEXTURE_TYPE::TEXTURE_SPECULAR]]);
		pixelShader->SetShaderResourceView("texture_displacement", m_textures[it->m_textures[TEXTURE_TYPE::TEXTURE_DISPLACEMENT]]);
		//pixelShader->SetShaderResourceView("texture_diffuse", m_textures[it->m_textures[0]]);
		vertexShader->CopyAllBufferData();
		pixelShader->CopyAllBufferData();
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			mesh->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices


	}
	// std::cout << "H\n" << count;
	/*

	for (auto cam = scene.cams.begin(); cam != scene.cams.end(); cam++) {
		//processCamera(*cam);
		for (auto it = scene.objects.begin(); it != scene.objects.end(); it++) {
			//processShader(it->shader);
			//processObject(*it);
			//draw the object
			XMStoreFloat4x4(&worldMatrix_temp, XMMatrixTranspose(XMLoadFloat4x4(&it->pos))); // Transpose for HLSL!
			vertexShader->SetMatrix4x4("world", worldMatrix_temp);
			vertexShader->CopyAllBufferData();
			switch (count++) {
			default:
			case 0:
				renderMesh(square.get()); break;
			case 1:
				renderMesh(diamond.get()); break;
			case 2:
				renderMesh(triangle.get()); break;
			}
		}
	}
	*/
	endRendering();
	context->RSSetViewports(1, &viewport);
}