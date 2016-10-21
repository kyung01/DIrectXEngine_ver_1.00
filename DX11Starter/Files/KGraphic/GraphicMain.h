#pragma once
#include <map>
#include "glm\glm.hpp"
#include "Scene.h"
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
namespace Graphic {
	class GraphicMain {
	private:
		void rendering(Scene scene);
		void processObject(DumbObject obj);
	protected:
		glm::mat4 matProjection, matView, matModel;
		virtual void processCamera(Camera cam) = 0;
		virtual void beginRendering() = 0;
		virtual void endRendering() = 0;
		virtual void getScreenWidth(int &w, int &h) = 0;
	public:
		float width, height;
		std::map<int, Model*> models;
		std::map<int, Shader*> shaders;
		void render(Scene scene);
	};
}