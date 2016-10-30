#include "Graphic/Object.h"
using namespace Graphic::NScene;
Object::Object():
	m_renderType(RENDER_TYPE::DEFAULT),
	m_isDirty(true),
	m_scale(1,1,1)
{

}

void Object::setPos(Vector3 pos)
{
	m_isDirty = true;
	m_pos = pos;
}
void Object::setScale(Vector3 scale)
{
	m_isDirty = true;
	m_scale = scale;
}
void Object::setRotation(Quaternion quaternion)
{
	m_isDirty = true;
	m_rotation = quaternion;
}


Matrix Object::getModelMatrix()
{
	if (m_isDirty) {
		m_isDirty = false;
		m_matModel =
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z)*
			DirectX::XMMatrixRotationQuaternion(m_rotation)*
			DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
	}
	return m_matModel;
}
