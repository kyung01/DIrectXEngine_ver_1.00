#include <Graphic\Asset\Asset.h>
using namespace NGraphic;

std::list<LoadInfoMesh> Asset::getLoadListMesh()
{
	std::list<LoadInfoMesh> lst({
		{ KEnum::MESH_ID_CONE, "Resource/Mesh/cone.obj" },
		{ KEnum::MESH_ID_CUBE, "Resource/Mesh/cube.obj" },
		{ KEnum::MESH_ID_CYLINDER, "Resource/Mesh/cylinder.obj" },
		{ KEnum::MESH_ID_HELIX, "Resource/Mesh/helix.obj" },
		{ KEnum::MESH_ID_SPHERE, "Resource/Mesh/sphere.obj" },
		{ KEnum::MESH_ID_TORUS, "Resource/Mesh/torus.obj" },
		{ KEnum::MESH_ID_PLANE, "Resource/Mesh/plane.obj" },
		{ KEnum::MESH_ID_TERRAIN_00, "Resource/Mesh/Grounds/Ground_s_01.obj" },
		{ KEnum::MESH_ID_STONEHENGE_00, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_01.obj" },
		{ KEnum::MESH_ID_STONEHENGE_01, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_02.obj" },
		{ KEnum::MESH_ID_STONEHENGE_02, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_03.obj" },
		{ KEnum::MESH_ID_STONEHENGE_03, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_04.obj" },
		{ KEnum::MESH_ID_STONEHENGE_04, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_05.obj" },
		{ KEnum::MESH_ID_STONEHENGE_05, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_06.obj" },
		{ KEnum::MESH_ID_STONEHENGE_06, "Resource/Mesh/Stonehenge_Rocks/Stonehenge_07.obj" }
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
		{ RENDER_TYPE_DEFFERED_FINAL,		L"Resource/Shader/deffered_final_vert.hlsl" },
		{ RENDER_TYPE_DEFERRED_LIGHT_INDIRECT,		L"Resource/Shader/deffered_indirect_vert.hlsl" },
		{ RENDER_TYPE_DEFERRED_LIGHT_INDIRECT_APPLY,		L"Resource/Shader/deffered_indirect_apply_vert.hlsl" },
		{ RENDER_TYPE_DEFERRED_LIGHT_INDIRECT_BLUR,		L"Resource/Shader/deffered_indirect_blur_vert.hlsl" },
		{ RENDER_TYPE_UI,		L"Resource/Shader/simple_texture_vert.hlsl" },
		{ RENDER_TYPE_LIGHT_RSM,		L"Resource/Shader/light_rsm_vert.hlsl" }
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
		{ RENDER_TYPE_DEFFERED_FINAL,		L"Resource/Shader/deffered_final_frag.hlsl" },
		{ RENDER_TYPE_DEFERRED_LIGHT_INDIRECT,		L"Resource/Shader/deffered_indirect_frag.hlsl" },
		{ RENDER_TYPE_DEFERRED_LIGHT_INDIRECT_APPLY,		L"Resource/Shader/deffered_indirect_apply_frag.hlsl" },
		{ RENDER_TYPE_DEFERRED_LIGHT_INDIRECT_BLUR,		L"Resource/Shader/deffered_indirect_blur_frag.hlsl" },
		{ RENDER_TYPE_UI,		L"Resource/Shader/simple_texture_frag.hlsl" },
		{ RENDER_TYPE_LIGHT_RSM,		L"Resource/Shader/light_rsm_frag.hlsl" }
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
		{ TEXTURE_ID_ICN_LIGHT,		L"Resource/Texture/icn_light.png" },
		{ TEXTURE_ID_WHITE,			L"Resource/Texture/texture_white.png" },
		{ TEXTURE_ID_HEIGHT_DEFAULT,		L"Resource/Texture/heightMap00.png" },
		{ TEXTURE_ID_HEIGHT_BUMP,			L"Resource/Texture/heightMap01.png" },
		{ TEXTURE_ID_HEIGHT_CIRCLES,		L"Resource/Texture/heightMap02.png" },
		{ TEXTURE_ID_HEIGHT_CLOUD,			L"Resource/Texture/heightMap03.png" }
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
		m_shadersFrag[it->type] = std::shared_ptr<SimpleFragmentShader>(new SimpleFragmentShader(device, context));
		if (!m_shadersFrag[it->type]->LoadShaderFileHLSL(it->path, "ps_5_0")) return false;
	}
	for (auto it = dataVert.begin(); it != dataVert.end(); it++) {
		m_shadersVert[it->type] = std::shared_ptr<SimpleVertexShader>(new SimpleVertexShader(device, context));
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

	D3D11_SAMPLER_DESC samplerDescWrap = {};
	samplerDescWrap.Filter = D3D11_FILTER_ANISOTROPIC; // Could be anisotropic
	samplerDescWrap.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDescWrap.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescWrap.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescWrap.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescWrap.MaxAnisotropy = 16;
	samplerDescWrap.MinLOD = 0;
	samplerDescWrap.MaxLOD = D3D11_FLOAT32_MAX;
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; // Could be anisotropic
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	D3D11_SAMPLER_DESC sampler_IDontKnow = {};
	sampler_IDontKnow.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // Could be anisotropic
	sampler_IDontKnow.ComparisonFunc = D3D11_COMPARISON_LESS;
	sampler_IDontKnow.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_IDontKnow.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_IDontKnow.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_IDontKnow.MaxAnisotropy = 16;
	sampler_IDontKnow.MinLOD = 0;
	sampler_IDontKnow.MaxLOD = D3D11_FLOAT32_MAX;

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

	D3D11_SAMPLER_DESC samplerDescLightRMS = {};
	samplerDescLightRMS.Filter = D3D11_FILTER_ANISOTROPIC;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescLightRMS.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLightRMS.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLightRMS.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescLightRMS.BorderColor[0] = 0.0f;
	samplerDescLightRMS.BorderColor[1] = 0.0f;
	samplerDescLightRMS.BorderColor[2] = 0.0f;
	samplerDescLightRMS.BorderColor[3] = 0.0f;
	samplerDescLightRMS.MaxAnisotropy = 16;
	samplerDescLightRMS.MinLOD = 0;
	samplerDescLightRMS.MaxLOD = D3D11_FLOAT32_MAX;



	ID3D11SamplerState* samplerWrap,* samplerClamp, *samplerBorderOne,*samplerBorderZero,*samplerLinear;
	device->CreateSamplerState(&samplerDescWrap, &samplerWrap);
	device->CreateSamplerState(&samplerDesc, &samplerClamp);
	device->CreateSamplerState(&sampler_IDontKnow , &samplerLinear);
	device->CreateSamplerState(&samplerDescLight, &samplerBorderOne);
	device->CreateSamplerState(&samplerDescLightRMS, &samplerBorderZero);
	m_samplers[SAMPLER_ID_WRAP] = samplerWrap;
	m_samplers[SAMPLER_ID_CLAMP] = samplerClamp;
	m_samplers[SAMPLER_ID_BORDER_ONE] = samplerBorderOne;
	m_samplers[SAMPLER_ID_BORDER_ZERO] = samplerBorderZero;
	m_samplers[SAMPLER_ID_LINEAR] = samplerLinear;

}
