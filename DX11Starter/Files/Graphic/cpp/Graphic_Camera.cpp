#include "Graphic\Camera.h"
#include <iostream>
Graphic::Camera::Camera()	
{
	m_pos = Vector3(0, 0, -5);
	m_isDirty_matView = true;
	m_isDirty_matProjection = true;
}

Matrix Graphic::Camera::getProjectionMatrix(float screen_width, float screen_height,float clipNear,float clipFar)
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

Matrix Graphic::Camera::getViewMatrix()
{
	if (m_isDirty_matView) {
		m_isDirty_matView = false;
		Vector3 target = XMQuaternionMultiply(m_rotation, Vector3(0, 0, 1));// , 0.0f);
		std::cout <<"HEY"<< target.x << " " << target.y << " " << target.z << "\n";
		m_matView = DirectX::XMMatrixLookToLH(m_pos, target, Vector3::Up);
	}
	return m_matView;
}

void Graphic::Camera::setPos(Vector3 pos) 
{
	Graphic::Object::setPos(pos);
	m_isDirty_matView = true;
}


void Graphic::Camera::setRotation(Quaternion quaternion)
{
	Graphic::Object::setRotation(quaternion);
	m_isDirty_matView = true;
}
