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
	this->textures[RENDER_TYPE::DEFFERED_DIFFUSE]	= RenderTexture();
	this->textures[RENDER_TYPE::DEFERRED_DEPTH]	= RenderTexture();
	this->textures[RENDER_TYPE::DEFFERED_NORMAL]	= RenderTexture();
	this->textures[RENDER_TYPE::DEFFERED_WORLD]	= RenderTexture();

	this->textures[RENDER_TYPE::DEFFERED_DIFFUSE]	.Initialize(device, width, height);
	this->textures[RENDER_TYPE::DEFERRED_DEPTH]	.Initialize(device, width, height);
	this->textures[RENDER_TYPE::DEFFERED_NORMAL]	.Initialize(device, width, height);
	this->textures[RENDER_TYPE::DEFFERED_WORLD]	.Initialize(device, width, height);

	return true;
}
bool GraphicMain::initShaders(ID3D11Device* device, ID3D11DeviceContext *context) {
	auto vertexShader = new Graphic::SimpleVertexShader(device, context);
	auto pixelShader = new Graphic::SimpleFragmentShader(device, context);
	if (!vertexShader->LoadShaderFileHLSL(L"Resource/default_vert.hlsl", "vs_5_0") ||
		!pixelShader->LoadShaderFileHLSL(L"Resource/default_frag.hlsl", "ps_5_0"))
		return false;

	this->shadersVert[RENDER_TYPE::DEFAULT] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->shadersFrag[RENDER_TYPE::DEFAULT] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	this->shadersVert[RENDER_TYPE::DEFFERED_DIFFUSE] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->shadersFrag[RENDER_TYPE::DEFFERED_DIFFUSE] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	this->shadersVert[RENDER_TYPE::DEFFERED_NORMAL] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->shadersFrag[RENDER_TYPE::DEFFERED_NORMAL] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	//this->shadersVert[SHADER_ID::DEFFERED_NORMAL] = std::make_unique<SimpleVertexShader*>(vertexShader);
	//this->shadersFrag[SHADER_ID::DEFFERED_NORMAL] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	return true;

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

void Graphic::GraphicMain::render(ID3D11DepthStencilView *depth, Graphic::Scene scene)
{
	beginRendering();
	/*
	renderTexture.SetRenderTarget(context, depth);
	renderTexture.ClearRenderTarget(context, depth, 0, 1, 0, 0);

	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);
	vertexShader->CopyAllBufferData();
	vertexShader->SetShader();
	pixelShader->SetShader();
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