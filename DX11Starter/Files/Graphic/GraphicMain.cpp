#include "GraphicMain.h"
#include <string>
using namespace Graphic;
void Graphic::GraphicMain::processObject(DumbObject obj) {
	if (obj.shader != NULL) {

		//Graphic::Shader *shader = shaders[obj.shader->type];
		//shader->use(this->matProjection, this->matView);
	}
}


bool Graphic::GraphicMain::loadShaders(
	ID3D11Device* device, ID3D11DeviceContext *context,
	std::map<ShaderID, std::unique_ptr<SimpleFragmentShader>>& shadersFrag,
	std::map<ShaderID, std::unique_ptr<SimpleVertexShader>>& shadersVert,
	ShaderInformation data[],int dataSize)
{

	for (int i =0; i < dataSize;i++){
		//std::string alternatePath = "Debug/";
		//alternatePath += data[i].path;
		std::string s = data[i].path;
		std::wstring stemp = std::wstring(s.begin(), s.end());
		LPCWSTR path = stemp.c_str();

		if (data[i].type == ShaderType::FRAGMENT_SHADER) {
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


void Graphic::GraphicMain::rendering(Scene scene)
{
	for (auto cam = scene.cams.begin(); cam != scene.cams.end(); cam++) {
		//processCamera(*cam);
		for (auto it = scene.objects.begin(); it != scene.objects.end(); it++) {
			//processShader(it->shader);
			//processObject(*it);
			//draw the object
		}
	}
	
}

void Graphic::GraphicMain::init(ID3D11Device *device, ID3D11DeviceContext *context)
{
	auto vertexShader = new Graphic::SimpleVertexShader(device, context);
	vertexShader->LoadShaderFileHLSL(L"Resource/default_vert.hlsl","vs_5_0");


	auto pixelShader = new Graphic::SimpleFragmentShader(device, context);
	pixelShader->LoadShaderFileHLSL(L"Resource/default_frag.hlsl", "ps_5_0");
	//if (!vertexShader->LoadShaderFile(L"Debug/VertexShader.cso"))
	//	vertexShader->LoadShaderFile(L"VertexShader.cso");

	//auto pixelShader = new Graphic::SimpleFragmentShader(device, context);
	//if (!pixelShader->LoadShaderFile(L"Debug/PixelShader.cso"))
	//	pixelShader->LoadShaderFile(L"PixelShader.cso");
	this->shadersVert[ShaderID::DEFAULT] = std::make_unique<SimpleVertexShader*>(vertexShader);
	this->shadersFrag[ShaderID::DEFAULT] = std::make_unique<SimpleFragmentShader*>(pixelShader);
	return;
	/*
	ShaderInformation shaderInfos[] = { 
		{ShaderID::DEFAULT,ShaderType::FRAGMENT_SHADER,"Debug/default_frag.cso"},
		{ShaderID::DEFAULT,ShaderType::VERTEX_SHADER,"Debug/default_vert.cso" } 
	};
	if (!loadShaders(device, context, this->shadersFrag, shadersVert, shaderInfos, 1 * 2)) {
		std::cout << "ShaderLoadingFailed" << std::endl;
	}
	*/
}

void Graphic::GraphicMain::render(Graphic::Scene scene)
{
	beginRendering();
	rendering(scene);
	endRendering();
}