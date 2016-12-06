
#include "deffered_indirect.hlsl"


Texture2D textureDirectLight	: register(t0);
Texture2D textureIndirectLight		: register(t1);

SamplerState samplerDefault	: register(s0);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)



float4 main(VertexToPixel input) : SV_TARGET
{
	float4 color = textureDirectLight.Sample(samplerDefault, input.uv) + textureIndirectLight.Sample(samplerDefault, input.uv);
	return float4(saturate(color.xyz), 1);
}