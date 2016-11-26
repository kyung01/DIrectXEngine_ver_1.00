#include "Graphic\Scene.h"
#include "Graphic\Light.h"
using namespace NGraphic;
using namespace NGraphic::NScene;
Scene::Scene():
	size(10,10,10)
{
}

Scene::Scene(float x, float y, float z):size(x,y,z)
{
}

void NScene::Scene::addObject(std::shared_ptr<Object> obj)
{
}

void Scene::loadExample00()
{
	return;
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
	
	Vector3 lightPositions[]{ Vector3(0,0,0),Vector3(0.0,0.0,-1.0) };//, Vector3(0, 0, 5), Vector3(5, 0, 0), Vector3(-5, 0, 0)};
	Vector3 lightRotation[]{ Vector3(0,3.14,0) ,Vector3(0,-3.14,0) };
	Vector4 lightColors[]{ Vector4(1,1,1,5.5) ,Vector4(1.0,0.0,0.0,150.5f) };
	

	
	{
		//ground
		auto obj = getObjSolid();
		obj->m_meshId = KEnum::MESH_ID_CUBE;
		obj->setPos(Vector3(0, 0, 7.5));
		obj->setScale(Vector3(20, 20, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = normalTexture[(count * 73) % 5];
	}

	{
		auto obj = getObjSolid();
		obj->m_meshId = KEnum::MESH_ID_CUBE;
		obj->setPos(Vector3(0, 0, 5));
		obj->setScale(Vector3(0.3, 0.3, 0.3));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = TEXTURE_ID_WHITE;
		obj->m_textures[TEXTURE_TYPE_NORMAL] = normalTexture[(count * 73) % 5];
	}
	

	{
		auto obj = getObjSolid();
		obj->m_meshId = KEnum::MESH_ID_HELIX;
		obj->setPos(Vector3(1.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
	}
	{
		auto obj = getObjSolid();
		obj->m_meshId = KEnum::MESH_ID_HELIX;
		obj->setPos(Vector3(-1.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
	}
	{
		auto obj = getObjSolid();
		obj->m_meshId = KEnum::MESH_ID_CYLINDER;
		obj->setPos(Vector3(3.0, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
	}
	{
		auto obj = getObjSolid();
		obj->m_meshId = KEnum::MESH_ID_TORUS;
		obj->setPos(Vector3(4.5, 1, 5));
		obj->setScale(Vector3(1, 1, 1));
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
	}

	{
		auto obj = getObjLight();
		obj->setPos(Vector3(0, 2.0f, 0.5));
		obj->m_lightColor = Vector4(1, 0.0, 0.0, 5);
	}
	{
		auto obj = getObjLight();
		obj->setPos(Vector3(1, 3.0f, 0.5));
		obj->m_lightColor = Vector4(1.0, 1.0, 1.0, 1);
	}
	//{
	//	auto obj = getObjLight();
	//	obj->setPos(Vector3(4, 3.0f, 0.5));
	//	obj->m_lightColor = Vector4(0, 0.0, 1.0, 5);
	//}
	{
		auto obj = getObjUI();
		obj->m_meshId = KEnum::MESH_ID_PLANE;
		obj->m_textures[TEXTURE_TYPE_DEFAULT] = TEXTURE_ID_ICN_LIGHT;
		obj->m_textures[TEXTURE_TYPE_DIFFUSE] = diffuseTextures[(count * 77) % 5];
		obj->m_textures[TEXTURE_TYPE_NORMAL] = TEXTURE_ID_NORMAL_WOOD;
		obj->setPos(Vector3(3.0, 1, 3.5));
		obj->setScale(Vector3(2, 2, 2));
		obj->setRotation(Quaternion());
	}


	
	this->m_camMain.setPos(Vector3(0, 0, 0));
}

std::shared_ptr<Object> NGraphic::NScene::Scene::getObjSolid()
{
	auto obj = std::shared_ptr<Object>(new Object());
	objs_solid.push_back(obj);
	return obj;
}

std::shared_ptr<Object> NGraphic::NScene::Scene::getObjUI()
{
	auto obj = std::shared_ptr<Object>(new Object());
	obj->m_ObjectType = KEnum::OBJ_TYPE_UI;
	objs_ui.push_back(obj);
	return obj;
}

std::shared_ptr<Light> NGraphic::NScene::Scene::getObjLight()
{
	auto obj = std::shared_ptr<Light>(new Light());
	objs_lights.push_back(obj);
	return obj;
}
