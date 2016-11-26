#pragma once
#include <list>
#include <memory>
#include "Camera.h"
#include <Graphic\Object.h>
#include <Graphic\Light.h>
namespace NGraphic {
	namespace NScene {
		class Scene {

		public:
			Vector3 size;
			Camera m_camMain; // the main camera
			std::list<std::shared_ptr<Object>> objs_solid; //things to be rendered on the screen
			std::list<std::shared_ptr<Object>> objs_ui; //things to be rendered on the screen
			std::list<std::shared_ptr<Light>> objs_lights; //things to be rendered on the screen
			Scene();
			Scene(float x, float y, float z);
			void addObject(std::shared_ptr<Object> obj);
			void loadExample00();

			std::shared_ptr<Object> getObjSolid();
			std::shared_ptr<Object> getObjUI();
			std::shared_ptr<Light>	getObjLight();
		};
	}
}