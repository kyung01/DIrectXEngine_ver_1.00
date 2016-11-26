#pragma once
namespace NGraphic {
	enum KEnum {
		OBJ_TYPE_SOLID,
		OBJ_TYPE_UI,
		OBJ_TYPE_LIGHT,
		MESH_ID_CONE,
		MESH_ID_CYLINDER,
		MESH_ID_CUBE,
		MESH_ID_HELIX,
		MESH_ID_SPHERE,
		MESH_ID_TORUS,
		MESH_ID_PLANE,
		MESH_ID_TERRAIN_00,
		MESH_ID_STONEHENGE_00,
		MESH_ID_STONEHENGE_01,
		MESH_ID_STONEHENGE_02,
		MESH_ID_STONEHENGE_03,
		MESH_ID_STONEHENGE_04,
		MESH_ID_STONEHENGE_05,
		MESH_ID_STONEHENGE_06,

		RENDER_TYPE_DEFAULT, RENDER_TYPE_OTHER,
		RENDER_TYPE_DEPTH,
		RENDER_TYPE_DEFFERED,
		RENDER_TYPE_DEFFERED_DIFFUSE, RENDER_TYPE_DEFFERED_NORMAL, RENDER_TYPE_DEFFERED_WORLD,
		RENDER_TYPE_DEFFERED_LIGHT_SPOTLIGHT,
		RENDER_TYPE_DEFFERED_LIGHT_POINT,
		RENDER_TYPE_DEFFERED_FINAL,
		RENDER_TYPE_UI,


		TEXTURE_ID_DEFAULT,
		TEXTURE_ID_NORMAL_DEFAULT,
		TEXTURE_ID_NORMAL_COUCH,
		TEXTURE_ID_NORMAL_DIRT,
		TEXTURE_ID_NORMAL_BRICK,
		TEXTURE_ID_NORMAL_WOOD,
		TEXTURE_ID_NORMAL_ROCK,
		TEXTURE_ID_TEST_00,
		TEXTURE_ID_TEST_01,
		TEXTURE_ID_TEST_02,
		TEXTURE_ID_TEST_03,
		TEXTURE_ID_TEST_04,
		TEXTURE_ID_TEST_05,
		TEXTURE_ID_TEST_06,
		TEXTURE_ID_ICN_LIGHT,
		TEXTURE_ID_WHITE,

		TEXTURE_TYPE_DEFAULT,
		TEXTURE_TYPE_DIFFUSE,
		TEXTURE_TYPE_NORMAL,
		TEXTURE_TYPE_DISPLACEMENT,
		TEXTURE_TYPE_SPECULAR,
		
		SAMPLER_ID_WRAP, SAMPLER_ID_BORDER_ONE,

		SHADER_TYPE_VERTEX, SHADER_TYPE_FRAGMENT
		
	};
}