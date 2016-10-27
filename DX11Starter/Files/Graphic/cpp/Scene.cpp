#include "Graphic/Scene.h"

Graphic::Scene::Scene()
{
}

void Graphic::Scene::loadExample00()
{
	for (int i = 0; i < 10; i++) for(int j = 0 ; j < 10; j++)for(int k = 0; k < 10; k++){
		Object obj;
		obj.setPos(Vector3(i, j, k));
		this->objects.push_back(obj);
	}
	//this->m_camMain.
}
