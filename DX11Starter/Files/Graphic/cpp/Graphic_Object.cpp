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
		isDirty = false;
		m_matModel =
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)*
			DirectX::XMMatrixRotationQuaternion(m_rotation)*
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	}
	return m_matModel;
}
