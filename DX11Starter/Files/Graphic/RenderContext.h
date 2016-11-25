#pragma once
#include <string>

namespace NGraphic {
	class GraphicMain;
	namespace NScene {
		class Scene;
	}
	struct RenderContext {
		std::string name; // id to describe the scene
		std::string description; // describe what this scene is
		NGraphic::GraphicMain *main;
		NGraphic::NScene::Scene *scene;
	};
}