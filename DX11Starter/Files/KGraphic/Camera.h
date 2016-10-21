#pragma once
#include "glm\glm.hpp"
using namespace glm;
namespace Graphic {
	class Camera {
	public:
		enum CAMERA_TYPE { PERSPECTIVE, ORTHOGONAL };
		CAMERA_TYPE viewType;
		mat4 position, rotation;
		vec3 up;

		float fov;
		float zNear,zFar;

		Camera();
		mat4 getProjViewMatrix();
	};
}