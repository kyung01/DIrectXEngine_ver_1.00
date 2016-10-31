
cbuffer global : register(b0)
{
	float3 worldSize;
};

// External texture-related data
Texture2D texture_diffuse		: register(t0);
Texture2D texture_normal		: register(t1);
Texture2D texture_specular		: register(t2);
Texture2D texture_displacement	: register(t3);

SamplerState sampler_default	: register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};



// Entry point for this pixel shader
PS_OUTPUT main(VertexToPixel input) : SV_TARGET
{
	return float4(1,0,0,1);
}

