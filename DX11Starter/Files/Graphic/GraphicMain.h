#pragma once
#include <map>
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "ShaderInformation.h"
#include "SimpleShader.h"
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>


namespace Graphic {
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them

	class GraphicMain {
	private:

		void rendering(Scene scene);
		void processObject(DumbObject obj);
		

		bool loadShaders(
			ID3D11Device* device, ID3D11DeviceContext *context,
			std::map<ShaderID, std::unique_ptr<SimpleFragmentShader>> &shadersFrag, 
			std::map<ShaderID, std::unique_ptr<SimpleVertexShader>> &shadersVert,
			ShaderInformation data[],  int dataSize);
		bool initShaders(ID3D11Device* device, ID3D11DeviceContext *context);
	protected:
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(Camera cam);// = 0;
		void beginRendering();// = 0;
		void endRendering();// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
	public:
		std::map<ShaderID, std::unique_ptr<SimpleFragmentShader*>> shadersFrag;
		std::map<ShaderID, std::unique_ptr<SimpleVertexShader*>> shadersVert;

		float width, height;
		std::map<int, Model*> models;
		std::map<int, Shader*> shaders;
		bool init(ID3D11Device *device, ID3D11DeviceContext *context);
		void render(Scene scene);
	};
}