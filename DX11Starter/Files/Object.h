#pragma once

#include <DirectXMath.h>
#include	"Material.h"
using namespace DirectX;
/*
*/
namespace World {
	class Object {
	protected:
	public:
		Graphic::Masterial *mat; // should gameobject have something related to graphic? I do not feel comfortable with this
		XMFLOAT4X4 pos, rotation, scale; //access these directly 
		Object(); //Object(Material *mat)? I do not like Object taking something not necessary to build Object as aprt of constructor argument 
		void Update(float timeElapsed);
	};
}