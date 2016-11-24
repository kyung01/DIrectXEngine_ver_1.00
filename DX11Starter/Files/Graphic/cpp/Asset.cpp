#include <Graphic\Asset\Asset.h>
using namespace Graphic;

std::list<LoadInfoMesh> Asset::getLoadListMesh()
{
	std::list<LoadInfoMesh> lst({
		{ KEnum::MESH_ID_CONE, "Resource/Mesh/cone.obj" },
		{ KEnum::MESH_ID_CUBE, "Resource/Mesh/cube.obj" },
		{ KEnum::MESH_ID_CYLINDER, "Resource/Mesh/cylinder.obj" },
		{ KEnum::MESH_ID_HELIX, "Resource/Mesh/helix.obj" },
		{ KEnum::MESH_ID_SPHERE, "Resource/Mesh/sphere.obj" },
		{ KEnum::MESH_ID_TORUS, "Resource/Mesh/torus.obj" },
		{ KEnum::MESH_ID_PLANE, "Resource/Mesh/plane.obj" }
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderVert()
{
	std::list<LoadInfoShader> lst({
		{ RENDER_TYPE_DEFAULT,			L"Resource/Shader/default_vert.hlsl" },
		{ RENDER_TYPE_DEFFERED,			L"Resource/Shader/deffered_vert.hlsl" },
		{ RENDER_TYPE_DEPTH,			L"Resource/Shader/depth_vert.hlsl" },
		{ RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT,		L"Resource/Shader/deffered_light_spot_vert.hlsl" },
		{ RENDER_TYPE_UI,		L"Resource/Shader/simple_texture_vert.hlsl" }
	});
	return lst;
}

std::list<LoadInfoShader> Asset::getLoadListShaderFrag()
{
	std::list<LoadInfoShader> lst({
		{ RENDER_TYPE_DEFAULT,			L"Resource/Shader/default_frag.hlsl" },
		{ RENDER_TYPE_DEFFERED,		L"Resource/Shader/deffered_frag.hlsl" },
		{ RENDER_TYPE_DEPTH,			L"Resource/Shader/depth_frag.hlsl" },
		{ RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT,		L"Resource/Shader/deffered_light_spot_frag.hlsl" },
		{ RENDER_TYPE_UI,		L"Resource/Shader/simple_texture_frag.hlsl" }
	});
	return lst;
}

std::list<LoadInfoTexture> Asset::getLoadListTexture()
{
	std::list<LoadInfoTexture> lst({
		{ TEXTURE_ID_DEFAULT,		L"Resource/Texture/textureTest00.jpg" },
		{ TEXTURE_ID_NORMAL_DEFAULT,	L"Resource/Texture/normal_default.jpg" },
		{ TEXTURE_ID_NORMAL_DIRT,		L"Resource/Texture/normal_dirt.jpg" },
		{ TEXTURE_ID_NORMAL_WOOD,		L"Resource/Texture/normal_wood.jpg" },
		{ TEXTURE_ID_NORMAL_BRICK,		L"Resource/Texture/normal_brick.jpg" },
		{ TEXTURE_ID_NORMAL_ROCK,		L"Resource/Texture/normal_rock.jpg" },//normal_couch.bmp
		{ TEXTURE_ID_NORMAL_COUCH,		L"Resource/Texture/normal_couch.bmp" },//normal_couch.bmp
		{ TEXTURE_ID_TEST_00,			L"Resource/Texture/texture_test_00.jpg" } ,
		{ TEXTURE_ID_TEST_01,			L"Resource/Texture/texture_test_01.jpg" } ,
		{ TEXTURE_ID_TEST_02,			L"Resource/Texture/texture_test_02.jpg" },
		{ TEXTURE_ID_TEST_03,			L"Resource/Texture/texture_test_03.jpg" },
		{ TEXTURE_ID_TEST_04,			L"Resource/Texture/texture_test_04.jpg" },
		{ TEXTURE_ID_TEST_05,			L"Resource/Texture/texture_test_05.jpg" },
		{ TEXTURE_ID_TEST_06,			L"Resource/Texture/texture_test_06.jpg" },
		{ TEXTURE_ID_ICN_LIGHT,		L"Resource/Texture/icn_light.png" }
	});
	return lst;
}

bool Asset::init(ID3D11Device * device, ID3D11DeviceContext * context)
{
	auto dataVert = getLoadListShaderVert();
	auto dataFrag = getLoadListShaderFrag();
	auto dataMesh = getLoadListMesh();
	auto dataTexture = getLoadListTexture();

	for (auto it = dataFrag.begin(); it != dataFrag.end(); it++) {
		m_shadersFrag[it->type] = std::shared_ptr<SimpleFragmentShader>(new Graphic::SimpleFragmentShader(device, context));
		if (!m_shadersFrag[it->type]->LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
	}
	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		m_shadersVert[it->type] = std::shared_ptr<SimpleVertexShader>(new Graphic::SimpleVertexShader(device, context));
		if (!m_shadersVert[it->type]->LoadShaderFileHLSL(it->path, "vs_5_0")) return false;
	}
	for (auto it = dataMesh.begin(); it != dataMesh.end(); it++) {
		auto mesh = new Mesh(device, it->path);
		m_meshes[it->id] = std::make_unique<Mesh*>(mesh);
	}
	for (auto it = dataTexture.begin(); it != dataTexture.end(); it++) {
		ID3D11ShaderResourceView *texture;
		//DirectX::CreateWICTextureFromFileEx(device,)

		DirectX::CreateWICTextureFromFile(device, context, it->path, 0, &texture);
		m_textures[it->id] = texture;
	}

	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // Could be anisotropic
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	D3D11_SAMPLER_DESC samplerDescLight = {};
	samplerDescLight.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescLight.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLight.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLight.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLight.BorderColor[0] = 1.0f;
	samplerDescLight.BorderColor[1] = 1.0f;
	samplerDescLight.BorderColor[2] = 1.0f;
	samplerDescLight.BorderColor[3] = 1.0f;
	samplerDescLight.MaxAnisotropy = 16;
	samplerDescLight.MinLOD = 0;
	samplerDescLight.MaxLOD = D3D11_FLOAT32_MAX;
	ID3D11SamplerState * samplerDefault, *samplerBorderOne;
	device->CreateSamplerState(&samplerDesc, &samplerDefault);
	device->CreateSamplerState(&samplerDescLight, &samplerBorderOne);
	m_samplers[SAMPLER_ID::SAMPLER_WRAP]		= samplerDefault;
	m_samplers[SAMPLER_ID::SAMPLER_BORDER_ONE]	= samplerBorderOne;

}
