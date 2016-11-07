#pragma once
#include <map>
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "ShaderInformation.h"
#include "TextureID.h"
#include "RenderType.h"
#include "SimpleShader.h"
#include "RenderTexture.h"
#include "DepthTexture.h"
#include "MeshID.h"
#include "Graphic\Light.h"
#include <list>
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>
#include <WICTextureLoader.h>



#include "Graphic/Mesh.h" // TODO delete this

namespace Graphic {
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	
	struct MeshLoadInformation {
		MESH_ID type;
		char* path;
	};
	struct ShaderLoadInformation {
		RENDER_TYPE type;
		LPCWSTR path;
	};
	struct TextureLoadInformation {
		TEXTURE_ID id;
		LPCWSTR path;
	};

	class GraphicMain {
	private:
		int m_width, m_height;
		
		void rendering(NScene::Scene scene);
		void processObject(NScene::Object obj);

		std::list<MeshLoadInformation>		getLoadListMesh();
		std::list<ShaderLoadInformation>	getLoadListShaderVert();
		std::list<ShaderLoadInformation>	getLoadListShaderFrag();
		std::list<TextureLoadInformation>	getLoadListTexture();

		bool initTextures		(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height);
		bool initShaders		(ID3D11Device* device, ID3D11DeviceContext *context);
		
		void renderPreDeffered(	ID3D11DeviceContext* context, NScene::Scene &scene,
								SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
								RenderTexture& texture_diffuse, RenderTexture& texture_normal, DepthTexture& textureDepth
			);
		void renderLights(ID3D11Device* device,	ID3D11DeviceContext* context, NScene::Scene &scene,
						SimpleVertexShader& shaderVertDepthOnly,
						SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag, RenderTexture& target, DepthTexture& targetDepth,
						RenderTexture& textureDiffuse, RenderTexture& textureNormal, DepthTexture& textureDepth
			);
		
	protected:
		ID3D11BlendState *m_blendStateNoBlack;
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(Graphic::NScene::Camera cam);// = 0;
		void beginRendering();// = 0;
		void endRendering();// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
	public:
		Graphic::Mesh * mesh00;

		std::map<TEXTURE_ID, ID3D11ShaderResourceView*> m_textures;

		std::map<RENDER_TYPE, std::shared_ptr<RenderTexture>>	m_renderTextures;
		std::map<RENDER_TYPE, std::shared_ptr<DepthTexture>>	m_depthTextures;
		std::map<int, DepthTexture*> m_lightDepthTextures;
		std::map<RENDER_TYPE, std::shared_ptr<SimpleFragmentShader>> m_shadersFrag;
		std::map<RENDER_TYPE, std::shared_ptr<SimpleVertexShader>> m_shadersVert;
		std::map<MESH_ID, std::unique_ptr<Mesh*>> m_meshes;
		ID3D11SamplerState *m_samplerDefault,*m_samplerLight;

		std::map<int, Model*> models;
		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height);
		void render(ID3D11Device * device , ID3D11DeviceContext* context, NScene::Scene scene);
	};
}