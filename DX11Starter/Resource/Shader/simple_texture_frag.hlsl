// External texture-related data
Texture2D texture00		: register(t0);
SamplerState sampler_default	: register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};



// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	return float4(1,1,1,1);
}