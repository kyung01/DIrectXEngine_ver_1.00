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


float4 getPosWorld(float2 uv, Texture2D depthTexture, matrix matProjViewInverse) {
	float4 posWorld = float4(
		uv.x * 2 - 1, (1 - uv.y) * 2 - 1,
		depthTexture.Sample(samplerLight, uv).x, 1);
	posWorld = mul(posWorld, matProjViewInverse);
	posWorld /= 0.00000001 + posWorld.w;

	return posWorld;
}

float3 getFluxColor(float2 uv,
	float3 posWorld, float3 normal,
	Texture2D textureLightDepth, Texture2D textureLightNormal, Texture2D textureLightRSM, matrix matLightProjViewInverse) {
	float3 lightPosWorld = getPosWorld(uv, textureLightDepth, matLightProjViewInverse).xyz;
	float3 lightNormal = textureLightNormal.Sample(samplerDefault, uv) * 2 - 1;
	//if (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1)
	//	return float3(0, 0, 0);
	//return lightPosWorld ;
	//return textureLightNormal.Sample(samplerLightRSM, uv);
	//if (uv.x > 1 || uv.x < 0 || uv.y >1 || uv.y < 0) {
	//	uv *= 0;
	//}
	//return  float3(uv*0.5, 0);

	float3 lightFlux = textureLightRSM.Sample(samplerLightRSM, uv);
	float3 dis = posWorld.xyz - lightPosWorld.xyz; 
	float disMag = dis.x*dis.x + dis.y*dis.y + dis.z*dis.z;
	float result = (
		max(0,dot(normal, lightPosWorld - posWorld ))
		* max(0, dot(lightNormal, posWorld - lightPosWorld ))
		)/ pow(disMag,2);
	return (lightFlux*result);// / (1 + disMag);
	//normalize(dis;
	//return float3(length(dis), 0, 0);
	//return lightFlux;// / pow(length(dis), 4);// *(result / (0.00001 + pow(dis.x*dis.x + dis.y*dis.y + dis.z*dis.z, 2)));
	//result /=  pow(normalize(dis), 4);
	//return lightPosWorld;
	//return float3(result, 0, 0);
	//return lightFlux *result;// *result * 1000;
}
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
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse);
	float3 disFromEyeToWorld = posWorld.xyz-posEye.xyz;
	float4 posFromLightProjection = mul(posWorld, matLightProjView);
	posFromLightProjection /=0.001+ posFromLightProjection.w;
	float2 uv = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	float2 uvTemp = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	

	float3 disFromLightToPos = posWorld.xyz - lightPos;
	float3 dirFromLightToPos = normalize(disFromLightToPos);
	float3 dirFromEyeToPos = normalize(posWorld.xyz - posEye.xyz);

	float lighted = posFromLightProjection.z - bias < textureLightDepth.Sample(samplerLight, uv).x;
	float3 light_spotLight = spotLight(dirFromEyeToPos, diffuse, lightColor, lightDir, posWorld.xyz - lightPos, dirFromLightToPos, normal, specular) * lighted;
	
	//return pow(max(0, dot(eye, -surfaceNormal)), 10 * luminosity);
	//return	float4(saturate( fluxColor ), 1);
	return	float4(light_spotLight, 1);
	
}