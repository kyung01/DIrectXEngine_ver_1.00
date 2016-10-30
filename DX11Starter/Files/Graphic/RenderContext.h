#pragma once
#include <string>

namespace Graphic {
	class GraphicMain;
	namespace NScene {
		class Scene;
	}
	struct RenderContext {
		std::string name; // id to describe the scene
		std::string description; // describe what this scene is
		Graphic::GraphicMain *main;
		Graphic::NScene::Scene *scene;
	};
}