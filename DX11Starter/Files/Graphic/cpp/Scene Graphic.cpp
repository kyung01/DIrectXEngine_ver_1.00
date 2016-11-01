#include "Graphic/Scene.h"
using namespace Graphic::NScene;
Scene::Scene():
	size(10,10,10)
{
}

Graphic::NScene::Scene::Scene(float x, float y, float z):size(x,y,z)
{
}

void Scene::loadExample00()
{
	int count = 0;
	TEXTURE_ID normalTexture[]{
		TEXTURE_ID::TEXTURE_NORMAL_DEFAULT,
		TEXTURE_ID::TEXTURE_NORMAL_COUCH,
		TEXTURE_ID::TEXTURE_NORMAL_BRICK ,
		TEXTURE_ID::TEXTURE_NORMAL_DIRT ,
		TEXTURE_ID::TEXTURE_NORMAL_ROCK,
		TEXTURE_ID::TEXTURE_NORMAL_WOOD };
	TEXTURE_ID diffuseTextures[]{
		TEXTURE_ID::TEXTURE_TEST_00,
		TEXTURE_ID::TEXTURE_TEST_01,
		TEXTURE_ID::TEXTURE_TEST_02 ,
		TEXTURE_ID::TEXTURE_TEST_03 ,
		TEXTURE_ID::TEXTURE_TEST_04,
		TEXTURE_ID::TEXTURE_TEST_05,
		TEXTURE_ID::TEXTURE_TEST_06 };
	for (int i = -2; i < 3; i++) for (int j = -2; j < 3; j++)for (int k = -2; k < 3; k++) {
		if (abs(i) + abs(j) + abs(k) < 2)
			continue;
		count++;
		Object* obj = new Object();
		(*obj).m_meshType = (MESH_TYPE)(count % 6);
		//obj.m_meshType = MESH_TYPE::SPHERE;
		(*obj).setPos(Vector3(i, j, k) * 2);
		(*obj).m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		(*obj).m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}
	Vector3 lightPositions[]{ Vector3(0,0,-2) };//, Vector3(0, 0, 5), Vector3(5, 0, 0), Vector3(-5, 0, 0)};
	for (int i = 0; i < 1; i++)  {
		Object* obj = new Camera();
		(*obj).m_ObjectType = NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL;
		(*obj).setPos(lightPositions[i]);

		this->objects.push_back(obj);
	}
	//this->m_camMain.
}
