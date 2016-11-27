#include "Y_Axis_Rotate.h"
#include <Game\Context.h>
#include <iostream>
void NGame::NScript::Y_Axis_Rotate::init(Context& context, Entity& me)
{
	progress = 0;
	progressDir = 1;
	rotationInit = me.rotation;
}

void NGame::NScript::Y_Axis_Rotate::update(Context & context, Entity & me, float timeElapsed)
{
	progress += progressDir * timeElapsed*0.1;
	if (abs(progress) > 1){
		progress = progressDir;
		progressDir *= -1;
	}
	me.position.z += timeElapsed*0.1;
	me.setRotation(rotationInit 
		//* Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), progress*(3.14*0.1))

		* Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), progress*(3.14*0.5))
	);
}
