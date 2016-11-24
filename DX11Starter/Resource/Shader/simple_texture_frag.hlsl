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
	return texture00.Sample(sampler_default, input.uv);
}