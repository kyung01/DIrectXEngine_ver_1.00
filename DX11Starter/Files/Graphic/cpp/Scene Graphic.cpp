#include "Graphic\Scene.h"
#include "Graphic\Light.h"
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
	Vector3 lightPositions[]{ Vector3(0,0,0),Vector3(0.0,0.0,-1.0) };//, Vector3(0, 0, 5), Vector3(5, 0, 0), Vector3(-5, 0, 0)};
	Vector3 lightRotation[]{ Vector3(0,3.14,0) ,Vector3(0,-3.14,0) };
	Vector4 lightColors[]{ Vector4(1,1,1,5.5) ,Vector4(1.0,0.0,0.0,150.5f) };
	for (int i = 0; i < 2; i++)  {
		continue;
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL;
		obj->setPos(lightPositions[i]);
		obj->setRotation(Quaternion(lightRotation[i]));
		dynamic_cast<Light*>(obj.get())->m_lightColor = lightColors[i];
		this->objects.push_back(obj);

	}

	{
		//ground
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = MESH_ID::CUBE;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(0, -1, 0));
		obj->setScale(Vector3(20, 1, 20));
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}

	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = MESH_ID::SPHERE;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(0, 0, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}
	

	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = MESH_ID::HELIX;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(1.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = TEXTURE_NORMAL_WOOD;
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = MESH_ID::HELIX;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(-1.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = TEXTURE_NORMAL_WOOD;
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = MESH_ID::CYLINDER;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(3.0, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = TEXTURE_NORMAL_WOOD;
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshType = MESH_ID::TORUS;
		obj->m_ObjectType = NScene::OBJECT_TYPE::SOLID;
		obj->setPos(Vector3(4.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = TEXTURE_NORMAL_WOOD;
		this->objects.push_back(obj);
	}

	{
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL;
		obj->setPos(Vector3(0, 3.5f, 0));
		obj->setRotation(Quaternion());
		dynamic_cast<Light*>(obj.get())->m_lightColor = Vector4(1, 0.5, 0.5, 15);
		this->objects.push_back(obj);
	}

	{
		//auto obj = std::shared_ptr<Object>(new Light());
		//obj->m_ObjectType = NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL;
		//obj->setPos(Vector3(.5, 1.5f, 0));
		//obj->setRotation(Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), 3.14 / 180 * 45));
		//dynamic_cast<Light*>(obj.get())->m_lightColor = Vector4(0, 1, 0, 15);
		//this->objects.push_back(obj);
	}
	/*
	{
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = NScene::OBJECT_TYPE::LIGHT_DIRECTIONAL;
		obj->setPos(Vector3(1.5, 0, 4));
		obj->setRotation(Quaternion());
		dynamic_cast<Light*>(obj.get())->m_lightColor = Vector4(0, 1, 0, 1);
		this->objects.push_back(obj);
	}
	*/

	
	this->m_camMain.setPos(Vector3(0, 0, 0));
}
