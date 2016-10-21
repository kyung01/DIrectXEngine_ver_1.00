#include "Object.h"

World::Object::Object():
	pos(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1),
	rotation(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1)

{
}

void World::Object::Update(float timeElapsed)
{
}
