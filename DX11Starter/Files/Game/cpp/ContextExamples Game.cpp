#include <Game\ContextExamples.h>
#include <Game\Script\Y_Axis_Rotate.h>
#include <Game\Script\X_Axis_Rotate.h>
using namespace NGame;

void NGame::LoadExample00(Context &context)
{
	auto ground = new Entity(); 
	
	NGraphic::KEnum meshIds[] = {
		NGraphic::MESH_ID_STONEHENGE_02,
		NGraphic::MESH_ID_STONEHENGE_06,
		NGraphic::MESH_ID_STONEHENGE_03,
		NGraphic::MESH_ID_STONEHENGE_00,
		NGraphic::MESH_ID_STONEHENGE_04,
		NGraphic::MESH_ID_STONEHENGE_05,
		NGraphic::MESH_ID_STONEHENGE_01 };
	NGraphic::KEnum meshIds2[] = {
		NGraphic::MESH_ID_CONE,
		NGraphic::MESH_ID_CUBE,
		NGraphic::MESH_ID_CYLINDER,
		NGraphic::MESH_ID_HELIX,
		NGraphic::MESH_ID_SPHERE,
		NGraphic::MESH_ID_TORUS,
		NGraphic::MESH_ID_SPHERE };
	NGraphic::KEnum normalIds[] = {
		NGraphic::TEXTURE_ID_NORMAL_BRICK,
		NGraphic::TEXTURE_ID_NORMAL_COUCH,
		NGraphic::TEXTURE_ID_NORMAL_DEFAULT,
		NGraphic::TEXTURE_ID_NORMAL_DIRT,
		NGraphic::TEXTURE_ID_NORMAL_ROCK,
		NGraphic::TEXTURE_ID_NORMAL_WOOD };
	NGraphic::KEnum reflectiveIds[] = {
		NGraphic::TEXTURE_ID_HEIGHT_DEFAULT,
		NGraphic::TEXTURE_ID_HEIGHT_BUMP,
		NGraphic::TEXTURE_ID_HEIGHT_CLOUD,
		NGraphic::TEXTURE_ID_HEIGHT_CIRCLES};

	float angle = 0;
	float distance = 3;
	{
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_TERRAIN_00;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_TEST_01;
		ground->m_graphicObjects.push_back(obj);
		ground->setPos(-25 , 0, -25);
	}
	distance = 13;
	for (int i = 0; i < 7; i++) {
		angle = 3.14 / 6 * i;
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = meshIds[i];
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(cos(angle)*distance, -0.25, sin(angle)*distance);
		e->setRotation(Quaternion::CreateFromAxisAngle(Vector3(0,1,0), 3.14/2- 3.14 / 6 * i));
	}
	distance = 3;
	for (int i = 0; i < 6; i++) {
		angle = 3.14 / 6 * i;
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_SPHERE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = reflectiveIds[i %4];
		obj.get()->setScale(Vector3(0.5, 0.5, 0.5));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(cos(angle)*distance, 0.5, sin(angle)*distance);
	}
	{
		distance = 6;
		angle =0;
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = NGraphic::MESH_ID_CUBE;
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = NGraphic::KEnum::TEXTURE_ID_NORMAL_DEFAULT;// normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = reflectiveIds[0 % 4];
		obj.get()->setScale(Vector3(1, 25, 25));
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(5+cos(angle)*distance, 0.3, sin(angle)*distance);
	}

	distance = 5;
	for (int i = 0; i < 9; i++) {
		angle = 3.14 / 8 * i;
		auto obj = context.m_scene->getObjSolid();
		obj.get()->m_meshId = meshIds2[i % 7];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_NORMAL] = normalIds[i % 6];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_SPECULAR] = reflectiveIds[i % 4];
		obj.get()->m_textures[NGraphic::TEXTURE_TYPE_DIFFUSE] = NGraphic::KEnum::TEXTURE_ID_WHITE;
		auto e = new Entity();
		context.addEntity(std::shared_ptr<Entity>(e));
		e->m_graphicObjects.push_back(obj);
		e->setPos(cos(angle)*distance, 0.25, sin(angle)*distance);
	}
	{
		auto obj = context.m_scene->getObjLight();
		auto light = new Entity();
		context.addEntity(std::shared_ptr<Entity>(light));
		obj->m_lightColor = Vector4(1.0, 1.0, 1.0, 10);
		light->m_graphicObjects.push_back(obj);
		light->setPos(0, 3.5, 0);
		light->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), 3.14*0.25f));
		light->addScript(context, std::shared_ptr<Script>(new NScript::Y_Axis_Rotate()));
	}
	{
		//auto obj = context.m_scene->getObjLight();
		//auto light = new Entity();
		//context.addEntity(std::shared_ptr<Entity>(light));
		//obj->m_lightColor = Vector4(1.0, 1.0, 1.0, 10);
		//light->m_graphicObjects.push_back(obj);
		//light->setPos(2, 5.5, 0);
		//light->setRotation(Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), 3.14*0.55f));
		//light->addScript(context, std::shared_ptr<Script>(new NScript::X_Axis_Rotate()));
	}

}
