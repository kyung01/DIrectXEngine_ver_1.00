#include "Graphic\Camera.h"
Graphic::Camera::Camera()
{
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
		m_matProjection = DirectX::SimpleMath::Matrix::CreatePerspective(m_screenWidth, m_screenHeight, m_clipNear, m_clipFar);
	}
	//DirectX::SimpleMath::Matrix::CreatePerspective()
	return m_matProjection;
}

Matrix Graphic::Camera::getViewMatrix()
{
	if (m_isDirty_matView) {
		m_isDirty_matView = false;
		m_matView = DirectX::SimpleMath::Matrix::CreateLookAt(m_pos, m_rotation*Vector3(0, 0, 1), Vector3::Up);
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
