#include "Assets.h"
using namespace Graphic;

std::list<LoadInfoMesh> Assets::getLoadListMesh()
{
	return std::list<LoadInfoMesh>();
}

std::list<LoadInfoShader> Assets::getLoadListShaderVert()
{
	return std::list<LoadInfoShader>();
}

std::list<LoadInfoShader> Assets::getLoadListShaderFrag()
{
	return std::list<LoadInfoShader>();
}

std::list<LoadInfoTexture> Assets::getLoadListTexture()
{
	return std::list<LoadInfoTexture>();
}

void Assets::load(ID3D11Device * device, ID3D11DeviceContext * context)
{
}
