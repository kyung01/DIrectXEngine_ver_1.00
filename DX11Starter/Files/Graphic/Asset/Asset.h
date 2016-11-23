#pragma once
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <list>
#include <map>
#include <memory>
#include <Graphic\Asset\TextureID.h>
#include <Graphic\Asset\SamplerID.h>
#include <Graphic\Asset\Mesh.h>
#include <Graphic\RenderType.h>
#include <Graphic\SimpleShader.h>
#include <Graphic\GraphicEnum.h>


namespace Graphic {
	struct LoadInfoMesh {
		GEnum id;
		char* path;
	};
	struct LoadInfoShader {
		RENDER_TYPE type;
		LPCWSTR path;
	};
	struct LoadInfoTexture {
		TEXTURE_ID id;
		LPCWSTR path;
	};
	class Asset {
	private:

		std::list<LoadInfoMesh>		getLoadListMesh();
		std::list<LoadInfoShader>	getLoadListShaderVert();
		std::list<LoadInfoShader>	getLoadListShaderFrag();
		std::list<LoadInfoTexture>	getLoadListTexture();

	public:
		std::map<RENDER_TYPE, std::shared_ptr<SimpleFragmentShader>> m_shadersFrag;
		std::map<RENDER_TYPE, std::shared_ptr<SimpleVertexShader>> m_shadersVert;
		std::map<GEnum, std::unique_ptr<Mesh*>> m_meshes;
		std::map<TEXTURE_ID, ID3D11ShaderResourceView*> m_textures;
		std::map<SAMPLER_ID, ID3D11SamplerState*> m_samplers;
		bool init(ID3D11Device* device, ID3D11DeviceContext *context);

	};
 }