#pragma once

#pragma once
#include "Camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {

	namespace NScene {
		class Light : public Camera {
		protected:
		public:
			Light();
			Vector4 m_lightColor;
			float m_lightPower;
			float m_lightAngle;
		};
	}
}