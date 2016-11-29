#include "point_light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	matrix matProjViewInverse; //used to unwrap depth to world
	matrix matLightProjView; //used to wrap world to screen relative projected position
	matrix matLightProjViewInverse; //used to unwrap depth to world
};

// External texture-related data
Texture2D textureLightDirect		: register(t0);
Texture2D textureLightIndirect		: register(t1);
Texture2D textureNormal		: register(t2);
Texture2D textureDepth		: register(t3);


SamplerState samplerDefault	: register(s0);
SamplerState samplerLinear	: register(s1);

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)


float4 getPosWorld(float2 uv, Texture2D depthTexture, matrix matProjViewInverse) {
	float4 posWorld = float4(
		uv.x * 2 - 1, (1 - uv.y) * 2 - 1,
		depthTexture.Sample(samplerDefault, uv).x, 1);
	posWorld = mul(posWorld, matProjViewInverse);
	posWorld /= 0.00000001 + posWorld.w;

	return posWorld;
}

static float PIXEL_DISTANCE = 1/200.0;

float4 main(VertexToPixel input) : SV_TARGET
{
	float diffMaxLimit = 2.0;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse);
	float3 meNormal = textureNormal.Sample(samplerDefault, input.uv).xyz * 2 - 1;
	float2 smaplingPositions[10] = { float2(0,0),
		float2(-PIXEL_DISTANCE,-PIXEL_DISTANCE),float2(0,-PIXEL_DISTANCE),float2(-PIXEL_DISTANCE,-PIXEL_DISTANCE),
		float2(-PIXEL_DISTANCE,0),float2(0,0),float2(-PIXEL_DISTANCE,0),
		float2(-PIXEL_DISTANCE,PIXEL_DISTANCE),float2(0,PIXEL_DISTANCE),float2(-PIXEL_DISTANCE,PIXEL_DISTANCE)
	};
	float3 color = textureLightDirect.Sample(samplerDefault, input.uv).xyz;
	float3 colorIndirect = float3(0, 0, 0);
	float angleDiff = 0;
	float posDiffTotal = 0;
	for (int i = 1; i < 10; i++) {
		float2 uv = input.uv + smaplingPositions[i];

		float2 uvRelative = uv;// float2(PIXEL_DISTANCE*round(uv.x / PIXEL_DISTANCE), PIXEL_DISTANCE*round(uv.y / PIXEL_DISTANCE));

		float4 otherPosWorld = getPosWorld(uvRelative, textureDepth, matProjViewInverse);
		float3 otherNormal = textureNormal.Sample(samplerDefault, uv).xyz * 2 - 1;
		//posDiffTotal += length(otherPosWorld.xyz - posWorld.xyz);
		float posDiff = length(otherPosWorld.xyz - posWorld.xyz);
		//if (posDiff < 5 && dot(meNormal, otherNormal) > 0.5)
			colorIndirect += textureLightIndirect.Sample(samplerDefault, uv) * (1 / 9.0);
		//return float4(length(uv-uvRelative)/3, 0, 0, 1);
		//angleDiff += 1-dot(meNormal, normal);
	}
	//return float4(saturate( colorIndirect), 1);
	//colorIndirect /= 5;

	//if (angleDiff > diffMaxLimit || posDiffTotal > 10.5)
		//return float4(1, 0, 0, 1);
	
	return float4(saturate(color*float3(1,0,0)+ colorIndirect), 1);
	//return float4(input.uv*0.1,1,1);
}