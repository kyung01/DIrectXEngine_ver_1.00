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

void Graphic::NScene::Scene::addObject(std::shared_ptr<Object> obj)
{
}

void Scene::loadExample00()
{
	int contentSize = 4;
	int count = 0;
	KEnum normalTexture[]{
		TEXTURE_ID_NORMAL_DEFAULT,
		TEXTURE_ID_NORMAL_COUCH,
		TEXTURE_ID_NORMAL_BRICK ,
		TEXTURE_ID_NORMAL_DIRT ,
		TEXTURE_ID_NORMAL_ROCK,
		TEXTURE_ID_NORMAL_WOOD };
	KEnum diffuseTextures[]{
		TEXTURE_ID_TEST_00,
		TEXTURE_ID_TEST_01,
		TEXTURE_ID_TEST_02 ,
		TEXTURE_ID_TEST_03 ,
		TEXTURE_ID_TEST_04,
		TEXTURE_ID_TEST_05,
		TEXTURE_ID_TEST_06 };
	for (int i = -contentSize+1; i < contentSize; i++) for (int j = -contentSize+1; j < contentSize; j++)for (int k = -contentSize+1; k < contentSize; k++) {
		if (abs(i) + abs(j) + abs(k) < 4)
			continue;
		continue;
		count++;
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_PLANE;//(MESH_ID)(count % 6);//MESH_ID::PLANE
		//obj->m_meshType = MESH_ID::PLANE;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(i, j, k) * 2);
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}
	Vector3 lightPositions[]{ Vector3(0,0,0),Vector3(0.0,0.0,-1.0) };//, Vector3(0, 0, 5), Vector3(5, 0, 0), Vector3(-5, 0, 0)};
	Vector3 lightRotation[]{ Vector3(0,3.14,0) ,Vector3(0,-3.14,0) };
	Vector4 lightColors[]{ Vector4(1,1,1,5.5) ,Vector4(1.0,0.0,0.0,150.5f) };
	for (int i = 0; i < 2; i++)  {
		continue;
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = KEnum::OBJ_TYPE_LIGHT;
		obj->setPos(lightPositions[i]);
		obj->setRotation(Quaternion(lightRotation[i]));
		dynamic_cast<Light*>(obj.get())->m_lightColor = lightColors[i];
		this->objects.push_back(obj);

	}

	{
		//ground
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_CUBE;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(0, -1, 0));
		obj->setScale(Vector3(20, 1, 20));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}
	{
		//ground
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_CUBE;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(0, 0, 7.5));
		obj->setScale(Vector3(20, 20, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}

	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_SPHERE;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(0, 0, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = normalTexture[(count * 73) % 5];
		this->objects.push_back(obj);
	}
	

	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_HELIX;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(1.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_HELIX;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(-1.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_CYLINDER;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(3.0, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_meshId = KEnum::MESH_ID_TORUS;
		obj->m_ObjectType = KEnum::OBJ_TYPE_SOLID;
		obj->setPos(Vector3(4.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
		this->objects.push_back(obj);
	}

	{
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = KEnum::OBJ_TYPE_LIGHT;
		obj->setPos(Vector3(0, 2.0f, 0.5));
		obj->setRotation(Quaternion());
		dynamic_cast<Light*>(obj.get())->m_lightColor = Vector4(1, 0.0, 0.0, 5);
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = KEnum::OBJ_TYPE_LIGHT;
		obj->setPos(Vector3(1, 3.0f, 0.5));
		obj->setRotation(Quaternion());
		dynamic_cast<Light*>(obj.get())->m_lightColor = Vector4(0, 1.0, 0.0, 5);
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Light());
		obj->m_ObjectType = KEnum::OBJ_TYPE_LIGHT;
		obj->setPos(Vector3(4, 3.0f, 0.5));
		obj->setRotation(Quaternion());
		dynamic_cast<Light*>(obj.get())->m_lightColor = Vector4(0, 0.0, 1.0, 5);
		this->objects.push_back(obj);
	}
	{
		auto obj = std::shared_ptr<Object>(new Object());
		obj->m_ObjectType = KEnum::OBJ_TYPE_UI;
		obj->m_meshId = KEnum::MESH_ID_PLANE;
		obj->m_textures[TEXTURE_TYPE_DEFAULT] = TEXTURE_ID_ICN_LIGHT;
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
		obj->setPos(Vector3(3.0, 1, 3.5));
		obj->setScale(Vector3(2, 2, 2));
		obj->setRotation(Quaternion());
		this->objects.push_back(obj);
	}


	
	this->m_camMain.setPos(Vector3(0, 0, 0));
}
