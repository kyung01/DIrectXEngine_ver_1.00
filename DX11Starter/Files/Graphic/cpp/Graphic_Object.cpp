#include "Graphic/Object.h"

Graphic::Object::Object():
	isDirty(true),
	m_scale(1,1,1)
{

}

void Graphic::Object::setPos(Vector3 pos)
{
	isDirty = true;
	m_pos = pos;
}
void Graphic::Object::setScale(Vector3 scale)
{
	isDirty = true;
	m_scale = scale;
}
void Graphic::Object::setRotation(Quaternion quaternion)
{
	isDirty = true;
	m_rotation = quaternion;
}


Matrix Graphic::Object::getModelMatrix()
{
	if (isDirty) {
		m_matModel =
			DirectX::SimpleMath::Matrix::CreateScale(m_scale) *
			DirectX::SimpleMath::Matrix::CreateFromQuaternion(m_rotation)*
			DirectX::SimpleMath::Matrix::CreateTranslation(m_pos);
	}
	return m_matModel;
}
