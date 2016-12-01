#include "point_light.hlsl"
#include "deffered_indirect.hlsl"

cbuffer global00 :register(b0)
{
	matrix matProjViewInverse; //used to unwrap depth to world
	matrix matLightProjView; //used to wrap world to screen relative projected position
	matrix matLightProjViewInverse; //used to unwrap depth to world
};

// External texture-related data
Texture2D textureNormal		: register(t0);
Texture2D textureSpecular	: register(t1);
Texture2D textureDepth		: register(t2);

Texture2D textureLightNormal		: register(t4);
Texture2D textureLightRSM		: register(t5);
Texture2D textureLightDepth		: register(t6);

SamplerState samplerDefault	: register(s0);
SamplerState samplerLightRSM	: register(s1);
// Struct representing a single vertex worth of data

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)



float4 main(VertexToPixel input) : SV_TARGET
{
	float specular = textureSpecular.Sample(samplerDefault, input.uv).x;
	float3 normal = normalize(textureNormal.Sample(samplerDefault, input.uv) * 2 - 1);
	
	float4 posEye = mul(float4(0, 0, 0, 1), matProjViewInverse);
	posEye /= 0.00000001 + posEye.w;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse,samplerDefault);
	float3 disFromEyeToWorld = posWorld.xyz-posEye.xyz;
	float3 dirEyeToWorld = normalize(disFromEyeToWorld);
	float4 posFromLightProjection = mul(posWorld, matLightProjView);
	posFromLightProjection /=0.000000001+ posFromLightProjection.w;
	float2 uv = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));


	return float4(
		IndirectLighting(posWorld, normal, dirEyeToWorld, uv, specular,
		textureLightNormal, textureLightRSM, textureLightDepth,
		matLightProjViewInverse,
			samplerDefault, samplerLightRSM)
		, 1);
}