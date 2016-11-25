#pragma once

#pragma once
#include "Camera.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace Graphic {

	namespace NScene {
		class Light : public Camera {
		protected:
		public:
			Vector4 m_lightColor;
			float m_lightPower;
			float m_lightAngle;
		};
	}
}