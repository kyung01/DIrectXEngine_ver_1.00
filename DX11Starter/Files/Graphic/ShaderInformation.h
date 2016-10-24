#pragma once
#include "ShaderID.h"
#include "ShaderType.h"
namespace Graphic {
	struct ShaderInformation {
		ShaderID id;
		ShaderType type;
		char* path;
	};
}