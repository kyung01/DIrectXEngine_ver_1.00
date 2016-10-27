#pragma once
#include <memory>
#include <d3d11.h>
#include <SimpleMath.h>
//#include "glm\glm.hpp"
//#include "Shader.h"
using namespace DirectX::SimpleMath;

namespace Graphic {
	class Object {
	private:
		bool isDirty;
	protected:
		Vector3 m_scale;
		Vector3 m_pos;
		Quaternion m_rotation;
		Matrix m_matModel;
	public:

		Object();
		virtual void setScale(Vector3 scale);
		virtual void setPos(Vector3 pos);
		virtual void setRotation(Quaternion quaternion);
		Matrix getModelMatrix();
	};
}