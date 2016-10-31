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
	for (int i = -2; i < 3; i++) for(int j = -2 ; j < 3; j++)for(int k = -2; k < 3; k++){
		if (abs(i) + abs(j) + abs(k) < 1) 
			continue;
		Object obj;
		obj.m_meshType = (MESH_TYPE)(count++ % 6);
		//obj.m_meshType = MESH_TYPE::SPHERE;
		obj.setPos(Vector3(i, j, k)*2);
		this->objects.push_back(obj);
	}
	//this->m_camMain.
}
