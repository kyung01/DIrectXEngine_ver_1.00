#pragma once
#include <d3d11.h>
#include <list>
#include <Graphic\MeshID.h>
#include <Graphic\TextureID.h>
#include <Graphic\RenderType.h>

namespace Graphic {
	struct LoadInfoMesh {
		MESH_ID type;
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
	class Assets {
	private:

		std::list<LoadInfoMesh>		getLoadListMesh();
		std::list<LoadInfoShader>	getLoadListShaderVert();
		std::list<LoadInfoShader>	getLoadListShaderFrag();
		std::list<LoadInfoTexture>	getLoadListTexture();

	public:
		void load(ID3D11Device* device, ID3D11DeviceContext *context);

	};
 }