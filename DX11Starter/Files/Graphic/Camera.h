#pragma once
#include "Object.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace Graphic {
	enum CAMERA_TYPE { PERSPECTIVE, ORTHOGONAL };

	namespace NScene {
		class Camera : public Object {
		private:
			float
				m_fov,
				m_screenWidth, m_screenHeight,
				m_clipNear, m_clipFar;
			Matrix	m_matProjection,
				m_matView;
		protected:
		public:
			bool	m_isDirty_matProjection,
					m_isDirty_matView;
			CAMERA_TYPE viewType;
			//mat4 position, rotation;
			//vec3 up;

			float fov;
			float zNear, zFar;

			Camera();
			Matrix getProjectionMatrix();
			Matrix getProjectionMatrix(float fov,float screen_width, float screen_height, float cam_near, float cam_far);
			Matrix getViewMatrix();
			void setPos(Vector3 pos) override;
			void setRotation(Quaternion quaternion) override ;
			
			//mat4 getProjViewMatrix();
		};
	}
}