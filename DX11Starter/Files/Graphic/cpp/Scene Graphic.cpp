#include "Graphic/Scene.h"
using namespace Graphic::NScene;
Scene::Scene()
{
}

void Scene::loadExample00()
{
	int count = 0;
	for (int i = -5; i < 5; i++) for(int j = -5 ; j < 5; j++)for(int k = -5; k < 5; k++){
		if (abs(i) + abs(j) + abs(k) < 1) 
			continue;
		Object obj;
		obj.m_meshType = (MESH_TYPE)(count++ % 6);
		obj.setPos(Vector3(i, j, k)*2);
		this->objects.push_back(obj);
	}
	//this->m_camMain.
}
