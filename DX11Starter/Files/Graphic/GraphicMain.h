#pragma once
#include <map>
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"

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
#include <Graphic\Enum.h>



namespace NGraphic {
	//TODO hlsl files are stroed in debug folder once they are built with extention .cso You need grasp them
	struct ReflectiveShadowMap{
		std::shared_ptr<RenderTexture>	flux;
		std::shared_ptr<RenderTexture>	fluxEye;
		std::shared_ptr<RenderTexture>	normal;
		std::shared_ptr<DepthTexture>	depth;

	};
	struct MeshLoadInformation {
		KEnum id;
		char* path;
	};
	struct TextureLoadInformation {
		KEnum id;
		LPCWSTR path;
	};

	class GraphicMain {
	private:
		int m_rsm_flux_eye_perspective_width, m_rsm_flux_eye_perspective_height;
		
		void rendering(NScene::Scene scene);
		void processObject(NScene::Object obj);


		bool initTextures		(ID3D11Device* device, ID3D11DeviceContext *context, int width, int height, int textureIndirectLightWidth, int textureIndirectLightHeight);
		bool initShaders		(ID3D11Device* device, ID3D11DeviceContext *context);

		void renderSolidObjects(
			ID3D11DeviceContext* context, NScene::Scene &scene,
			SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
			std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures);

		void renderPreDeffered(	ID3D11DeviceContext* context, NScene::Scene &scene, 
								SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
								RenderTexture& texture_diffuse, RenderTexture& texture_normal, RenderTexture& texture_specular,
								DepthTexture& textureDepth,
								std::map<KEnum, std::unique_ptr<Mesh*>> &meshes,std::map<KEnum, ID3D11ShaderResourceView*> &textures,
								ID3D11SamplerState * sampler
				
			);
		void renderUI(
			ID3D11DeviceContext* context, NScene::Scene &scene,
			SimpleVertexShader& shader_vert, SimpleFragmentShader& shader_frag,
			RenderTexture& texture_final, DepthTexture& textureDepth,
			std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> *textures,
			ID3D11SamplerState * sampler);
		void renderLights(
			ID3D11Device* device, ID3D11DeviceContext* context,
			NScene::Scene &scene,
			SimpleVertexShader& shaderVertLight, SimpleFragmentShader & shaderFragLight,
			SimpleVertexShader& shaderVertIndirectLight, SimpleFragmentShader & shaderFragIndirectLight,
			SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag,

			RenderTexture& target, DepthTexture& targetDepth,
			RenderTexture& targetIndirectLight, DepthTexture& targetIndirectDepth,
			RenderTexture& textureDiffuse, RenderTexture& textureNormal, RenderTexture & textureSpecular,
			DepthTexture& textureDepth, DepthTexture& textureLightIndirectDpeth,
			std::map<KEnum, std::unique_ptr<Mesh*>> &meshes, std::map<KEnum, ID3D11ShaderResourceView*> &textures,
			ID3D11SamplerState * samplerDefault, ID3D11SamplerState * samplerLightDepth, ID3D11SamplerState * samplerLightRSM
		);
		void renderIndirectLightBlur(
			ID3D11Device* device, ID3D11DeviceContext* context, NScene::Scene & scene,
			SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag,

			RenderTexture& target, DepthTexture& targetDepth,
			RenderTexture & textureIndirectLight,
			RenderTexture& textureNormal, RenderTexture&textureSpecular, DepthTexture& textureDepth,
			std::unique_ptr<Mesh*> &meshePlane,
			ID3D11SamplerState * samplerDefault,
			ID3D11SamplerState * samplerLinear
		);
		void renderApplyDirectAndIndirectLights(
			ID3D11Device* device, ID3D11DeviceContext* context, NScene::Scene & scene,
			SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag,

			RenderTexture& target, DepthTexture& targetDepth,
			RenderTexture & textureDirectLight, RenderTexture & textureIndirectLight,
			RenderTexture& textureNormal, RenderTexture&textureSpecular, DepthTexture& textureDepth,
			std::unique_ptr<Mesh*> &meshePlane,
			ID3D11SamplerState * samplerDefault,
			ID3D11SamplerState * samplerLinear
		);
		//void renderApplyRSMs(
		//	ID3D11Device* device, ID3D11DeviceContext* context, NScene::Scene & scene,
		//	SimpleVertexShader& shaderVert, SimpleFragmentShader& shaderFrag,
		//
		//	RenderTexture& target, DepthTexture& targetDepth,
		//	RenderTexture& textureLightDirect, RenderTexture& textureLightIndirect, 
		//	RenderTexture& textureNormal, RenderTexture&textureSpecular,DepthTexture& textureDepth,
		//	std::unique_ptr<Mesh*> &meshePlane,
		//	ID3D11SamplerState * samplerDefault,
		//	ID3D11SamplerState * samplerLinear
		//);
		
	protected:
		ID3D11BlendState 
			*m_blendStateNoBlack,
			*m_blendStateTransparent;
		//glm::mat4 matProjection, matView, matModel;
		void processCamera(NScene::Camera cam);// = 0;
		void beginRendering();// = 0;
		void endRendering();// = 0;
		void getScreenWidth(int &w, int &h);// = 0;
	public:
		int m_width, m_height;
		Mesh * mesh00;


		std::map<KEnum, std::shared_ptr<RenderTexture>>	m_renderTextures;
		std::map<KEnum, std::shared_ptr<DepthTexture>>	m_depthTextures;
		std::map<int, DepthTexture*>					m_lightDepthTextures;
		std::map<int, ReflectiveShadowMap>				m_RSM;

		std::map<int, Shader*> shaders;
		// Width and hieght is for the resolution in wihich this graphic main will adjust to render things onto
		GraphicMain();
		bool init(ID3D11Device *device, ID3D11DeviceContext *context, int textureWidth, int textureHeight, int textureIndirectLightWidth, int textureIndirectLightHeight);
		void render(ID3D11Device * device , ID3D11DeviceContext* context,  Asset* asset, NScene::Scene scene);
	};
}