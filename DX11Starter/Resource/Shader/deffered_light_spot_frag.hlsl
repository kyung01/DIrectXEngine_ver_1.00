#include "point_light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	//float3 worldSize;
	float3 lightPos;
	float3 lightDir;
	float4 lightColor;
	matrix matProjViewInverse; //used to unwrap depth to world
	matrix matLightProjView; //used to wrap world to screen relative projected position
	matrix matLightProjViewInverse; //used to unwrap depth to world
};

// External texture-related data
Texture2D textureDiffuse		: register(t0);
Texture2D textureNormal		: register(t1);
Texture2D textureSpecular	: register(t2);
Texture2D textureDepth		: register(t3);

Texture2D textureLightNormal		: register(t4);
Texture2D textureLightRSM		: register(t5);
Texture2D textureLightDepth		: register(t6);

SamplerState samplerDefault	: register(s0);
SamplerState samplerLight	: register(s1);
SamplerState samplerLightRSM	: register(s2);
// Struct representing a single vertex worth of data

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)




float4 main(VertexToPixel input) : SV_TARGET
{
	float4 output;
	float4x4 matBias = float4x4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0);
	float bias = 0.00000000000010;
	//values I can get from textures
	float3 diffuse = textureDiffuse.Sample(samplerDefault, input.uv);
	float specular = textureSpecular.Sample(samplerDefault, input.uv).x;
	float3 normal = normalize(textureNormal.Sample(samplerDefault, input.uv) * 2 - 1);
	
	float4 posEye = mul(float4(0, 0, 0, 1), matProjViewInverse);
	posEye /= 0.001 + posEye.w;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse, samplerLight);
	float3 disFromEyeToWorld = posWorld.xyz-posEye.xyz;
	float4 posFromLightProjection = mul(posWorld, matLightProjView);
	posFromLightProjection /=0.001+ posFromLightProjection.w;
	float2 uv = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	float2 uvTemp = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	

	float3 disFromLightToPos = posWorld.xyz - lightPos;
	float3 dirFromLightToPos = normalize(disFromLightToPos);
	float3 dirFromEyeToPos = normalize(posWorld.xyz - posEye.xyz);

	float lighted = posFromLightProjection.z - bias < textureLightDepth.Sample(samplerLight, uv).x;
	float3 light_spotLight = spotLight(dirFromEyeToPos, diffuse, lightColor, lightDir, disFromLightToPos, dirFromLightToPos, normal, specular) * lighted;
	
	//return pow(max(0, dot(eye, -surfaceNormal)), 10 * luminosity);
	//return	float4(saturate( fluxColor ), 1);
	return	float4(light_spotLight, 1);
	
}