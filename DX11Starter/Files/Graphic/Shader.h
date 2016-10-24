#pragma once
#include <iostream>
#include "glm\glm.hpp"
namespace Graphic {
	class Shader {
	private:
	protected:
	public:
		enum KType{DEFAULT=0,DEFERRED=1};
		KType type;
		virtual void use(glm::mat4 projection, glm::mat4 view) = 0;
		virtual void input(std::string id, float* value) = 0;
		virtual void input(std::string id, int* value) = 0;
	};
}