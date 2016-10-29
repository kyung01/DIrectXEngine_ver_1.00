#pragma once
#include "Object.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace Graphic {
	enum CAMERA_TYPE { PERSPECTIVE, ORTHOGONAL };
	class Camera : public NScene::Object{
	private:
		float 
			m_screenWidth, m_screenHeight,
			m_clipNear, m_clipFar;
	protected:
		bool	m_isDirty_matProjection,
				m_isDirty_matView;
		Matrix	m_matProjection,
				m_matView;
	public:
		CAMERA_TYPE viewType;
		//mat4 position, rotation;
		//vec3 up;

		float fov;
		float zNear,zFar;

		Camera();
		Matrix getProjectionMatrix(float screen_width, float screen_height,float cam_near, float cam_far);
		Matrix getViewMatrix();
		void setPos(Vector3 pos) override;
		void setRotation(Quaternion quaternion) override;
		//mat4 getProjViewMatrix();
	};
}