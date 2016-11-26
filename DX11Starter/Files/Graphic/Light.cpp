#include <Graphic\Light.h>
using namespace NGraphic::NScene;
Light::Light()
{
	Camera::Camera();
	m_ObjectType = OBJ_TYPE_LIGHT;
}
