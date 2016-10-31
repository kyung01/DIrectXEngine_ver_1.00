#pragma once
#include <list>
#include "Camera.h"
#include "Object.h"
namespace Graphic {
	namespace NScene {
		class Scene {
		public:
			Vector3 size;
			Camera m_camMain; // the main camera
			std::list<Camera> cams; // for this scene you need to render these cameras
			std::list<Object> objects; //things to be rendered on the screen
			Scene();
			Scene(float x, float y, float z);
			void loadExample00();
		};
	}
}