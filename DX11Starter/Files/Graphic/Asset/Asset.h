#pragma once
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <list>
#include <map>
#include <memory>
#include <Graphic\Asset\SamplerID.h>
#include <Graphic\Asset\Mesh.h>
#include <Graphic\SimpleShader.h>
#include <Graphic\Enum.h>


namespace Graphic {
	struct LoadInfoMesh {
		KEnum id;
		char* path;
	};
	struct LoadInfoShader {
		KEnum type;
		LPCWSTR path;
	};
	struct LoadInfoTexture {
		KEnum id;
		LPCWSTR path;
	};
	class Asset {
	private:

		std::list<LoadInfoMesh>		getLoadListMesh();
		std::list<LoadInfoShader>	getLoadListShaderVert();
		std::list<LoadInfoShader>	getLoadListShaderFrag();
		std::list<LoadInfoTexture>	getLoadListTexture();

	public:
		std::map<KEnum, std::shared_ptr<SimpleFragmentShader>> m_shadersFrag;
		std::map<KEnum, std::shared_ptr<SimpleVertexShader>> m_shadersVert;
		std::map<KEnum, std::unique_ptr<Mesh*>> m_meshes;
		std::map<KEnum, ID3D11ShaderResourceView*> m_textures;
		std::map<SAMPLER_ID, ID3D11SamplerState*> m_samplers;
		bool init(ID3D11Device* device, ID3D11DeviceContext *context);

	};
 }