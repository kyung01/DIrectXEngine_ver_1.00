#pragma once
#include <map>
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "ShaderInformation.h"
#include "RenderType.h"
#include "SimpleShader.h"
#include "RenderTexture.h"
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>


namespace Graphic {
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them

	class GraphicMain {
	private:
		int width, height;
		void rendering(Scene scene);
		void processObject(Object obj);
		

		bool loadShaders(
			ID3D11Device* device, ID3D11DeviceContext *context,
			std::map<RENDER_TYPE, std::unique_ptr<SimpleFragmentShader>> &shadersFrag,
			std::map<RENDER_TYPE, std::unique_ptr<SimpleVertexShader>> &shadersVert,
			ShaderInformation data[],  int dataSize);
		bool initTextures(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height);
		bool initShaders(ID3D11Device* device, ID3D11DeviceContext *context);
	protected:
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(Camera cam);// = 0;
		void beginRendering();// = 0;
		void endRendering();// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
	public:
		std::map<RENDER_TYPE, RenderTexture> textures;
		std::map<RENDER_TYPE, std::unique_ptr<SimpleFragmentShader*>> shadersFrag;
		std::map<RENDER_TYPE, std::unique_ptr<SimpleVertexShader*>> shadersVert;

		std::map<int, Model*> models;
		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height);
		void render(ID3D11DepthStencilView *depth, Scene scene);
	};
}