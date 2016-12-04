#pragma once
#include "Object.h"
#include <SimpleMath.h>
using namespace DirectX::SimpleMath;
//using namespace glm;
namespace NGraphic {
	enum CAMERA_TYPE { PERSPECTIVE, ORTHOGONAL };

	namespace NScene {
		class Camera : public Object {
			static float DEFAULT_CAMERA_ASPECT_RATIO;
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
			Matrix getProjectionMatrix(float screen_width, float screen_height);
			Matrix getProjectionMatrix(float fov,float screen_width, float screen_height, float cam_near, float cam_far);
			Matrix getViewMatrix();
			Object setPos(Vector3 pos) override;
			Object setRotation(Quaternion quaternion) override ;
			
			//mat4 getProjViewMatrix();
		};
	}
}