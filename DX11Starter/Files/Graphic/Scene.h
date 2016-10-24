#pragma once
#include <list>
#include "Camera.h"
#include "Object.h"
namespace Graphic {
	class Scene {
	public:
		std::list<Graphic::Camera> cams; // for this scene you need to render these cameras
		std::list<Graphic::DumbObject> objects; //things to be rendered on the screen
	};
}