
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	float3 worldSize;
	matrix world;
	matrix view;
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float2 uv			: TEXCOORD;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;
	matrix worldViewProj = mul(mul(world, view), projection);

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.uv = input.uv;

	return output;
}