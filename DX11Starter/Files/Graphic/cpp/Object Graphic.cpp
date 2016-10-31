#include "Graphic/Object.h"
using namespace Graphic::NScene;
Object::Object():
	m_renderType(RENDER_TYPE::DEFAULT),
	m_isDirty(true),
	m_scale(1,1,1)
{
	m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = TEXTURE_ID::TEXTURE_A;
	m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = TEXTURE_ID::TEXTURE_B;
	m_textures[TEXTURE_TYPE::TEXTURE_SPECULAR] = TEXTURE_ID::TEXTURE_C;
	m_textures[TEXTURE_TYPE::TEXTURE_DISPLACEMENT] = TEXTURE_ID::TEXTURE_D;
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
