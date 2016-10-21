#include "Camera.h"

World::Camera::Camera()
{
	this->pos = XMFLOAT4X4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -5.f, 1);
}
