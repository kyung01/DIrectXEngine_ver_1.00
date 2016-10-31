#include "Graphic\Camera.h"
#include <iostream>
using namespace Graphic;
using namespace Graphic::NScene;
Camera::Camera()	
{
	m_pos = Vector3(0, 0, 0);
	m_isDirty_matView = true;
	m_isDirty_matProjection = true;
}

Matrix Camera::getProjectionMatrix(float screen_width, float screen_height,float clipNear,float clipFar)
{
	if (m_screenWidth != screen_width || m_screenHeight != screen_height||m_clipNear != clipNear || m_clipFar != clipFar) {
		m_isDirty_matProjection = true;
		m_screenWidth = screen_width;
		m_screenHeight = screen_height;
		m_clipNear = clipNear;
		m_clipFar = clipFar;
	}
	if (m_isDirty_matProjection) {
		m_isDirty_matProjection = false;
		m_matProjection = DirectX::XMMatrixPerspectiveFovLH(
			0.25f * 3.1415926535f,		// Field of View Angle
			screen_width / screen_height,		// Aspect ratio
			clipNear, clipFar);					// Far clip plane distance
	}
	//DirectX::SimpleMath::Matrix::CreatePerspective()
	return m_matProjection;
}

Matrix Camera::getViewMatrix()
{
	if (m_isDirty_matView) {
		m_isDirty_matView = false;
		auto rotatedFOrward = (Vector3)DirectX::XMVector3Rotate(Vector3::Forward, m_rotation);
		std::cout << "DEBUG " << m_pos.x << " " << m_pos.y << " " << m_pos.z << "\n";
		std::cout << "rotatedFOrward " << rotatedFOrward.x << " " << rotatedFOrward.y << " " << rotatedFOrward.z << "\n";
		Vector3 target = m_pos + (Vector3)DirectX::XMVector3Rotate(Vector3::Forward, m_rotation);// , 0.0f);
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
