
cbuffer lightData : register(b0)
{
	matrix mat;
	matrix mat2;
	float4 AmbientLightColor;

	float4 DirLightColor;
	float3 DirLightDirection;

	float4 PointLightColor;
	float3 PointLightPosition;

	float3 CameraPosition;
};

// External texture-related data
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
Texture2D ShadowMap		: register(t2);
TextureCube Sky			: register(t3);
SamplerState Sampler	: register(s0);
SamplerComparisonState ShadowSampler : register(s1);
// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
	float4 posForShadow : TEXCOORD1;
};
struct VertOut {
	float4 position;
};

VertOut main(VertexToPixel input) : SV_TARGET
{
	VertOut output;
	output.position = mul(float4(input.position.xyz, 1.0f), mat);
	return output;
}