#pragma once
#include <list>
#include "Object.h"
#include "Camera.h"

/*
	WorldMain class is a main for a world.
	World contains all the informations that are mainly visual. 
	It does not process game elements such as physics.
	A
*/
namespace World {
	class WorldMain {
	public:
		Camera cam;
		std::list<Object> objs;
		WorldMain();
		void update(float timeElapsed);
	};
}