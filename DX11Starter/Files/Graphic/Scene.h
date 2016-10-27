#pragma once
#include <list>
#include "Camera.h"
#include "Object.h"
namespace Graphic {
	class Scene {
	public:
		Graphic::Camera m_camMain; // the main camera
		std::list<Graphic::Camera> cams; // for this scene you need to render these cameras
		std::list<Graphic::Object> objects; //things to be rendered on the screen
		Scene();
		void loadExample00();
	};
}