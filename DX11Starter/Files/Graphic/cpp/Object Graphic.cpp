#include "Graphic/Object.h"
using namespace Graphic::NScene;
int Graphic::NScene::Object::OBJECT_UNIQUE_ID = 0;
Object::Object() :
	m_isAlive(true),
	m_id(OBJECT_UNIQUE_ID++),
	m_ObjectType(KEnum::OBJ_TYPE_SOLID),
	m_meshId	(KEnum::MESH_ID_CUBE),
	m_renderType(KEnum::RENDER_TYPE_DEFAULT),
	m_isDirty(true),
	m_scale(1,1,1),
	m_dirLook(0,0,1)
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
	m_dirLook = DirectX::XMVector3Rotate(Vector3(0, 0, 1), m_rotation);
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
