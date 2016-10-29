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
#include "MeshID.h"
#include <list>
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>



#include "Graphic/Mesh.h" // TODO delete this

namespace Graphic {
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	struct MeshLoadInformation {
		MESH_TYPE type;
		char* path;
	};
	struct ShaderLoadInformation {
		RENDER_TYPE type;
		LPCWSTR path;
	};

	class GraphicMain {
	private:
		int width, height;
		
		void rendering(Scene scene);
		void processObject(Object obj);

		std::list<MeshLoadInformation>		getLoadListMesh();
		std::list<ShaderLoadInformation>	getLoadListShaderVert();
		std::list<ShaderLoadInformation>	getLoadListShaderFrag();

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
		Graphic::Mesh * mesh00;
		std::map<RENDER_TYPE, RenderTexture*> m_textures;
		std::map<RENDER_TYPE, std::unique_ptr<SimpleFragmentShader*>> m_shadersFrag;
		std::map<RENDER_TYPE, std::unique_ptr<SimpleVertexShader*>> m_shadersVert;
		std::map<MESH_TYPE, std::unique_ptr<Mesh*>> m_meshes;

		std::map<int, Model*> models;
		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height);
		void render(ID3D11DeviceContext* context, ID3D11DepthStencilView *depth, Scene scene);
	};
}