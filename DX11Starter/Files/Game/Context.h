#pragma once
#include <list>
#include <memory>
#include <Game\Entity.h>
#include <Graphic\Scene.h>
namespace NGame {
	class Context {
	public:

		NGraphic::NScene::Scene *m_scene; // is going to ask the scene to get graphical objects it needs 
		std::list<std::shared_ptr<Entity>> m_entities;

		Context();
		void update(float timeElapsed);
		void init(NGraphic::NScene::Scene *scene);
		void addEntity(std::shared_ptr<Entity> entity);
		
		
	};
}