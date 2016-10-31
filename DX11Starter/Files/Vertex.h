#pragma once

#include <DirectXMath.h>

// --------------------------------------------------------
// A custom vertex definition
//
// You will eventually ADD TO this, and/or make more of these!
// --------------------------------------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Position;	    // The position of the vertex
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT3 biTangent;
	DirectX::XMFLOAT2 UV;
	Vertex();
	//DirectX::XMFLOAT4 Color;        // The color of the vertex
};