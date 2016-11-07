#include "Graphic\Camera.h"
#include <iostream>
using namespace Graphic;
using namespace Graphic::NScene;
Camera::Camera():
	m_clipFar(100),
	m_clipNear(0),
	m_screenWidth(100),
	m_screenHeight(100)
{
	m_pos = Vector3(0, 0, 0);
	m_isDirty_matView = true;
	m_isDirty_matProjection = true;
}

Matrix Graphic::NScene::Camera::getProjectionMatrix()
{
	if (m_isDirty_matProjection) {
		m_isDirty_matProjection = false;
		m_matProjection = DirectX::XMMatrixPerspectiveFovLH(
			m_fov,		// Field of View Angle
			m_screenWidth / m_screenHeight,		// Aspect ratio
			m_clipNear, m_clipFar);					// Far clip plane distance
	}
	return m_matProjection;
}

Matrix Camera::getProjectionMatrix( float fov, float screen_width, float screen_height,float clipNear,float clipFar)
{
	if (m_fov != fov || m_screenWidth != screen_width || m_screenHeight != screen_height||m_clipNear != clipNear || m_clipFar != clipFar) {
		m_isDirty_matProjection = true;
		m_screenWidth = screen_width;
		m_screenHeight = screen_height;
		m_clipNear = clipNear;
		m_clipFar = clipFar;
		m_fov = fov;
	}
	//DirectX::SimpleMath::Matrix::CreatePerspective()
	return getProjectionMatrix();
}

Matrix Camera::getViewMatrix()
{
	if (m_isDirty_matView) {
		m_isDirty_matView = false;
		//std::cout << "DEBUG " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
		//std::cout << "rotatedFOrward " << rotatedFOrward.x << " " << rotatedFOrward.y << " " << rotatedFOrward.z << "\n";
		Vector3 target =  (Vector3)DirectX::XMVector3Rotate(Vector3(0,0,1), m_rotation);// , 0.0f);
		m_matView = DirectX::XMMatrixLookToLH(m_pos, target, Vector3::Up);
	}
	return m_matView;
}

void Camera::setPos(Vector3 pos) 
{
	Object::setPos(pos);
	m_isDirty_matView = true;
}


void Camera::setRotation(Quaternion quaternion) 
{
	Object::setRotation(quaternion);
	m_isDirty_matView = true;

}
