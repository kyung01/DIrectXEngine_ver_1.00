#pragma once

#include <DirectXMath.h>
#include "Object.h"
using namespace DirectX;
namespace World {
	class Camera: public Object {
	protected:
	public:
		XMFLOAT4X4 projection;
		Camera();
	};
}