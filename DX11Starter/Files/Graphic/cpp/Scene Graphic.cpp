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
	int contentSize = 4;
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
	for (int i = -contentSize+1; i < contentSize; i++) for (int j = -contentSize+1; j < contentSize; j++)for (int k = -contentSize+1; k < contentSize; k++) {
		if (abs(i) + abs(j) + abs(k) < 4)
			continue;
		count++;
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = (MESH_ID)(count % 6);//MESH_ID::PLANE
		//obj->m_meshType = MESH_ID::PLANE;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(i, j, k) * 2);
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}
	Vector3 lightPositions[]{ Vector3(0,0,-10) };//, Vector3(0, 0, 5), Vector3(5, 0, 0), Vector3(-5, 0, 0)};
	for (int i = 0; i < 1; i++)  {
		auto obj = std::shared_ptr<Object>(new Camera());
		obj->m_ObjectType = NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL;
		obj->setPos(lightPositions[i]);
		this->objects.push_back(obj);

	}
	this->m_camMain.setPos(Vector3(0, 0, 0));
}
