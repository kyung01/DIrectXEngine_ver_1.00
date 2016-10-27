#include "GraphicMain.h"
#include <string>

using namespace Graphic;
void Graphic::GraphicMain::processObject(Object obj) {
}


bool Graphic::GraphicMain::loadShaders(
	ID3D11Device* device, ID3D11DeviceContext *context,
	std::map<RENDER_TYPE, std::unique_ptr<SimpleFragmentShader>>& shadersFrag,
	std::map<RENDER_TYPE, std::unique_ptr<SimpleVertexShader>>& shadersVert,
	ShaderInformation data[],int dataSize)
{
	/*
	for (int i =0; i < dataSize;i++){
		//std::string alternatePath = "Debug/";
		//alternatePath += data[i].path;
		std::string s = data[i].path;
		std::wstring stemp = std::wstring(s.begin(), s.end());
		LPCWSTR path = stemp.c_str();

		if (data[i].type == SHADER_TYPE::FRAGMENT_SHADER) {
			SimpleFragmentShader shader(device, context);
			
			if (!shader.LoadShaderFile(path))
				//&& !shader.LoadShaderFile((LPCWSTR)alternatePath.c_str()))
				return false;
			shadersFrag[data[i].id] = std::make_unique<SimpleFragmentShader>(shader);
		}
		else {
			SimpleVertexShader shader(device, context);
			if (!shader.LoadShaderFile(path) )
				//&&!shader.LoadShaderFile((LPCWSTR)alternatePath.c_str()))
				return false;
			shadersVert[data[i].id] = std::make_unique<SimpleVertexShader>(shader);
		}
		
	}
	*/
	return true;
	
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


void Graphic::GraphicMain::rendering( Scene scene)
{

	
}
bool Graphic::GraphicMain::initTextures(ID3D11Device * device, ID3D11DeviceContext * context,int width, int height)
{
	this->m_textures[RENDER_TYPE::DEFFERED_DIFFUSE]	= new RenderTexture();
	this->m_textures[RENDER_TYPE::DEFERRED_DEPTH]	= new RenderTexture();
	this->m_textures[RENDER_TYPE::DEFFERED_NORMAL]	= new RenderTexture();
	this->m_textures[RENDER_TYPE::DEFFERED_WORLD]	= new  RenderTexture();

	this->m_textures[RENDER_TYPE::DEFFERED_DIFFUSE]	->Initialize(device, width, height);
	this->m_textures[RENDER_TYPE::DEFERRED_DEPTH]	->Initialize(device, width, height);
	this->m_textures[RENDER_TYPE::DEFFERED_NORMAL]	->Initialize(device, width, height);
	this->m_textures[RENDER_TYPE::DEFFERED_WORLD]	->Initialize(device, width, height);

	return true;
}
bool GraphicMain::initShaders(ID3D11Device* device, ID3D11DeviceContext *context) {
	auto vertexShader = new Graphic::SimpleVertexShader(device, context);
	auto pixelShader = new Graphic::SimpleFragmentShader(device, context);
	if (!vertexShader->LoadShaderFileHLSL(L"Resource/default_vert.hlsl", "vs_5_0") ||
		!pixelShader->LoadShaderFileHLSL(L"Resource/default_frag.hlsl", "ps_5_0"))
		return false;

	this->m_shadersVert[RENDER_TYPE::DEFAULT] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->m_shadersFrag[RENDER_TYPE::DEFAULT] = std::make_unique<SimpleFragmentShader*>(pixelShader);

	this->m_shadersVert[RENDER_TYPE::DEFFERED_DIFFUSE] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->m_shadersFrag[RENDER_TYPE::DEFFERED_DIFFUSE] = std::make_unique<SimpleFragmentShader*>(pixelShader);

	this->m_shadersVert[RENDER_TYPE::DEFFERED_NORMAL] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->m_shadersFrag[RENDER_TYPE::DEFFERED_NORMAL] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	//this->shadersVert[SHADER_ID::DEFFERED_NORMAL] = std::make_unique<SimpleVertexShader*>(vertexShader);
	//this->shadersFrag[SHADER_ID::DEFFERED_NORMAL] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	return true;

}
Graphic::GraphicMain::GraphicMain()
{
}
bool Graphic::GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height)
{
	this->width = width;
	this->height = height;
	if (!initShaders(device, context)||
		!initTextures(device,context,width,height)
		) return false;
	return true;
}
float temp_angle = 0;
void Graphic::GraphicMain::render(ID3D11DeviceContext *context, ID3D11DepthStencilView *depth, Graphic::Scene scene)
{
	scene.m_camMain.setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), temp_angle) );
	temp_angle += .01f;
	beginRendering();

	auto renderTexture = m_textures[RENDER_TYPE::DEFFERED_DIFFUSE];
	auto vertexShader = *m_shadersVert.begin()->second;
	auto pixelShader = *m_shadersFrag.begin()->second;


	DirectX::XMFLOAT4X4 world,view,projection;

	DirectX::XMStoreFloat4x4(&world,		XMMatrixTranspose(scene.m_camMain.getModelMatrix() )); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&view,			XMMatrixTranspose(scene.m_camMain.getViewMatrix())); // Transpose for HLSL!
	DirectX::XMStoreFloat4x4(&projection,	XMMatrixTranspose(scene.m_camMain.getProjectionMatrix(width, height, 0.1f, 100.0f))); // Transpose for HLSL!

	renderTexture->SetRenderTarget(context, depth);
	renderTexture->ClearRenderTarget(context, depth, 1 ,.5f, 0, 1);
	vertexShader->SetMatrix4x4("world", world);
	vertexShader->SetMatrix4x4("view", view);
	vertexShader->SetMatrix4x4("projection", projection);
	//vertexShader->SetMatrix4x4("world", scene.m_camMain.getModelMatrix());
	//vertexShader->SetMatrix4x4("view", scene.m_camMain.getViewMatrix());
	//vertexShader->SetMatrix4x4("projection", scene.m_camMain.getProjectionMatrix(width,height,0.1f,100.0f));
	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
	int count = 0;
	
	for (auto it = scene.objects.begin(); it != scene.objects.end(); it++, count++) {
		//XMStoreFloat4x4(&worldMatrix_temp, XMMatrixTranspose(XMLoadFloat4x4(&it->pos))); // Transpose for HLSL!

		DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(it->getModelMatrix())); // Transpose for HLSL!

		vertexShader->SetMatrix4x4("world", world);
		vertexShader->CopyAllBufferData();


		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &mesh00->getBufferVertexRef(), &stride, &offset);
		context->IASetIndexBuffer(mesh00->getBufferIndex(), DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			mesh00->getBufferIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
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
}