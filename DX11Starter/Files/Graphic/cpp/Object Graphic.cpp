#include "Graphic/Object.h"
using namespace Graphic::NScene;
int Graphic::NScene::Object::OBJECT_UNIQUE_ID = 0;
Object::Object() :
	m_id(OBJECT_UNIQUE_ID++),
	m_ObjectType(SOLID),
	m_meshType(MESH_ID::CUBE),
	m_renderType(RENDER_TYPE::DEFAULT),
	m_isDirty(true),
	m_scale(1,1,1)
{
	m_textures[TEXTURE_TYPE::TEXTURE_DIFFUSE] = TEXTURE_ID::TEXTURE_DEFAULT;
	m_textures[TEXTURE_TYPE::TEXTURE_NORMAL] = TEXTURE_ID::TEXTURE_DEFAULT;
	m_textures[TEXTURE_TYPE::TEXTURE_SPECULAR] = TEXTURE_ID::TEXTURE_DEFAULT;
	m_textures[TEXTURE_TYPE::TEXTURE_DISPLACEMENT] = TEXTURE_ID::TEXTURE_DEFAULT;
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
