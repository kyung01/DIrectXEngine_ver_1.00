#include "WorldMain.h"
#include <Windows.h>
#include <tuple>
#include <iostream>
World::WorldMain::WorldMain()
{

}

void World::WorldMain::update(float timeElapsed)
{
	float x, y, speed(.5f* timeElapsed),dis_camerMove(1.3f*timeElapsed);
	int count = 0;
	for (auto it = objs.begin(); it != objs.end(); it++) {
		x = cos(.5*3.14f*count);
		y = sin(.5*3.14f*count);
		XMStoreFloat4x4(& it->pos, XMLoadFloat4x4(&it->pos)* XMMatrixTranslation(x*speed, y*speed, 0) );
		count++;
	}
	XMVECTOR dir;
	//std::cout << dir.m128_f32[0] << " " << dir.m128_f32[1] << " " << dir.m128_f32[2] << std::endl;
	if (GetAsyncKeyState('W') & 0x8000) {
		dir = XMVector4Transform(XMVectorSet(0, 0, 1, 1), XMLoadFloat4x4(&cam.rotation)) * dis_camerMove;
		XMStoreFloat4x4(&cam.pos, XMLoadFloat4x4(&cam.pos) * (XMMatrixTranslation(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2] ) ) );
		/* Do something useful */
	}

	if (GetAsyncKeyState('S') & 0x8000) {
		dir = XMVector4Transform(XMVectorSet(0, 0, -1, 1), XMLoadFloat4x4(&cam.rotation)) * dis_camerMove;
		XMStoreFloat4x4(&cam.pos, XMLoadFloat4x4(&cam.pos) * (XMMatrixTranslation(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2])));
		/* Do something useful */
	}

	if (GetAsyncKeyState('A') & 0x8000) {
		dir = XMVector4Transform(XMVectorSet(-1, 0, 0, 1), XMLoadFloat4x4(&cam.rotation)) * dis_camerMove;
		XMStoreFloat4x4(&cam.pos, XMLoadFloat4x4(&cam.pos) * (XMMatrixTranslation(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2])));
		/* Do something useful */
	}

	if (GetAsyncKeyState('D') & 0x8000) {
		dir = XMVector4Transform(XMVectorSet(1, 0, 0, 1), XMLoadFloat4x4(&cam.rotation)) * dis_camerMove;
		XMStoreFloat4x4(&cam.pos, XMLoadFloat4x4(&cam.pos) * (XMMatrixTranslation(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2])));
		/* Do something useful */
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		dir = XMVectorSet(0, -dis_camerMove, 0, 1);
		XMStoreFloat4x4(&cam.pos, XMLoadFloat4x4(&cam.pos) * (XMMatrixTranslation(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2])));
		/* Do something useful */
	}

	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		dir = XMVectorSet(0, dis_camerMove, 0, 1);
		XMStoreFloat4x4(&cam.pos, XMLoadFloat4x4(&cam.pos) * (XMMatrixTranslation(dir.m128_f32[0], dir.m128_f32[1], dir.m128_f32[2])));
		/* Do something useful */
	}

}
