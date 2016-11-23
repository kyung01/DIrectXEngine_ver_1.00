#pragma once
#include <map>
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

#include "RenderType.h"
#include "SimpleShader.h"
#include "RenderTexture.h"
#include "DepthTexture.h"
#include "Graphic\Light.h"
#include <list>
#include <map>
#include <memory>
#include <d3d11.h>

#include <SimpleMath.h>
#include <WICTextureLoader.h>
#include <Graphic\Asset\Asset.h>
#include <Graphic\Asset\Mesh.h>// TODO delete this
#include <Graphic\Asset\TextureId.h>
#include <Graphic\GraphicEnum.h>



namespace Graphic {
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	
	struct MeshLoadInformation {
		GEnum id;
		char* path;
	};
	struct TextureLoadInformation {
		TEXTURE_ID id;
		LPCWSTR path;
	};

	class GraphicMain {
	private:
		
		void rendering(NScene::Scene scene);
		void processObject(NScene::Object obj);

		std::list<MeshLoadInformation>		getLoadListMesh();

		bool initTextures		(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height);
		bool initShaders		(ID3D11Device* device, ID3D11DeviceContext *context);
		
		void renderPreDeffered(	ID3D11DeviceContext* context, NScene::Scene &scene, 
								SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
								RenderTexture& texture_diffuse, RenderTexture& texture_normal, DepthTexture& textureDepth,
								std::map<TEXTURE_ID, ID3D11ShaderResourceView*> *textures,
								ID3D11SamplerState * sampler
				
			);
		void renderUI(
			ID3D11DeviceContext* context, NScene::Scene &scene,
			SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
			RenderTexture& texture_final, DepthTexture& textureDepth,
			std::map<TEXTURE_ID, ID3D11ShaderResourceView*> *textures,
			ID3D11SamplerState * sampler);
		void renderLights(ID3D11Device* device,	ID3D11DeviceContext* context,
						NScene::Scene &scene,
						SimpleVertexShader& shaderVertDepthOnly,
						SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag, RenderTexture& target, DepthTexture& targetDepth,
						RenderTexture& textureDiffuse, RenderTexture& textureNormal, DepthTexture& textureDepth,
						std::map<TEXTURE_ID, ID3D11ShaderResourceView*> *textures,
						ID3D11SamplerState * samplerDefault, ID3D11SamplerState * samplerLightDepth
			);
		
	protected:
		ID3D11BlendState *m_blendStateNoBlack;
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(Graphic::NScene::Camera cam);// = 0;
		void beginRendering();// = 0;
		void endRendering();// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
	public:
		int m_width, m_height;
		Graphic::Mesh * mesh00;


		std::map<RENDER_TYPE, std::shared_ptr<RenderTexture>>	m_renderTextures;
		std::map<RENDER_TYPE, std::shared_ptr<DepthTexture>>	m_depthTextures;
		std::map<int, DepthTexture*> m_lightDepthTextures;
		std::map<GEnum, std::unique_ptr<Mesh*>> m_meshes;

		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int width, int height);
		void render(ID3D11Device * device , ID3D11DeviceContext* context,  Asset* asset, NScene::Scene scene);
	};
}