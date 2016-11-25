#pragma once
#include <list>
#include <Game\Entity.h>
namespace Graphic {
	namespace NScene {
		class Scene;
	}
}
namespace NGame {
	class Context {
		Graphic::NScene::Scene *m_scene; // is going to ask the scene to get graphical objects it needs 
		std::list<Entity> m_entities;
	public:
		Context(Graphic::NScene::Scene *scene);

		
		
	};
}