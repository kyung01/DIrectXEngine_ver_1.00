#include "point_light.hlsl"

// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	float3 lightPos;
	matrix matProjViewInverse; //used to unwrap depth to world
};

Texture2D textureLightIndirect		: register(t0);
Texture2D textureNormal		: register(t1);
Texture2D textureSpecular	: register(t2);
Texture2D textureDepth		: register(t3);
Texture2D textureLightDirect : register(t4);


SamplerState samplerDefault	: register(s0);
SamplerState samplerIndirectLight	: register(s1);

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

static float PIXEL_DISTANCE = 1/ 256.0;

float4 main(VertexToPixel input) : SV_TARGET
{
	float diffMaxLimit = 2.0;
	float3 colorDirect = textureLightDirect.Sample(samplerDefault, input.uv).xyz;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse);
	float3 meNormal = textureNormal.Sample(samplerDefault, input.uv).xyz * 2 - 1;
	float specular = textureSpecular.Sample(samplerDefault, input.uv).x;
	float4 posEye = mul(float4(0, 0, 0, 1), matProjViewInverse);
	posEye /= 0.00000001 + posEye.w;
	float3 eyeAndLight = normalize( normalize(posWorld.xyz- posEye.xyz) + normalize(posWorld.xyz- lightPos));
	//float3 eyeAndLight = normalize((posWorld.xyz - posEye.xyz) + dirLightToWorld);
	//float	meSpecular = textureSpecular.Sample(samplerDefault, input.uv).x;
	float2 smaplingPositions[4] = {
		float2(0,0),float2(PIXEL_DISTANCE,0),
		float2(0,PIXEL_DISTANCE),float2(PIXEL_DISTANCE,PIXEL_DISTANCE)
	};
	float3 sampledColors[4] = { float3(0,0,0) ,float3(0,0,0) ,float3(0,0,0) ,float3(0,0,0) };

	float3 colorIndirect = float3(0, 0, 0);
	float angleDiff = 0;
	float posDiffTotal = 0;
	//return textureLightIndirect.Sample(samplerIndirectLight, input.uv);
	for (int i = 0; i < 4; i++) {
		//float2 uv = input.uv + smaplingPositions[i];
		smaplingPositions[i] = float2(
			PIXEL_DISTANCE*floor(input.uv.x / PIXEL_DISTANCE) + smaplingPositions[i].x,
			PIXEL_DISTANCE*floor(input.uv.y / PIXEL_DISTANCE) + smaplingPositions[i].y);
		//smaplingPositions[i] = float2(input.uv.x + smaplingPositions[i].x,
		//	input.uv.y + smaplingPositions[i].y);
		sampledColors[i] = textureLightIndirect.Sample(samplerIndirectLight, smaplingPositions[i]);// *(posDiff < 5 && dot(meNormal, otherNormal) > 0.5);


		float2 uvRelative = float2(
			PIXEL_DISTANCE*floor(smaplingPositions[i].x / PIXEL_DISTANCE) ,
			PIXEL_DISTANCE*floor(smaplingPositions[i].y / PIXEL_DISTANCE) 
			);
		
		//
		float4 otherPosWorld = getPosWorld(uvRelative, textureDepth, matProjViewInverse);
		float3 otherNormal = textureNormal.Sample(samplerIndirectLight, uvRelative).xyz * 2 - 1;
		////posDiffTotal += length(otherPosWorld.xyz - posWorld.xyz);
		float posDiff = 1 / (1+length(otherPosWorld.xyz - posWorld.xyz));
		//sampledColors[i] = textureLightIndirect.Sample(samplerIndirectLight, smaplingPositions[i]);// *(posDiff < 5 && dot(meNormal, otherNormal) > 0.5);
		//if (posDiff < 5 && dot(meNormal, otherNormal) > 0.5)
		//	colorIndirect += textureLightIndirect.Sample(samplerIndirectLight, uv) * (1 / 9.0);
		if (dot(meNormal, otherNormal) < 0.1 || length(otherPosWorld.xyz - posWorld.xyz) > 10.0) {
			return float4(1, 0, 0, 1);
			return float4(colorDirect, 1);
		}
		sampledColors[i] *= (dot(meNormal, otherNormal) >0.1) * (length(otherPosWorld.xyz - posWorld.xyz) < 0.5);
		//sampledColors[i] *= max(0, dot(meNormal, otherNormal)) * posDiff;
		//return float4(length(uv-uvRelative)/3, 0, 0, 1);
		//angleDiff += 1-dot(meNormal, normal);
	}
	float xRatioFromEdge =   (smaplingPositions[1].x - input.uv.x) / PIXEL_DISTANCE;
	float xRatioFromStart =  (input.uv.x- smaplingPositions[0].x)  / PIXEL_DISTANCE;
	
	//float3 x00 = sampledColors[0] * (( ) / PIXEL_DISTANCE);
	//float3 x01 = sampledColors[1] * ((input.uv.x - smaplingPositions[0].x) / PIXEL_DISTANCE);
	//float3 x10 = sampledColors[2] * ((smaplingPositions[1].x - input.uv.x) / PIXEL_DISTANCE);
	//float3 x11 = sampledColors[3] * ((input.uv.x - smaplingPositions[0].x) / PIXEL_DISTANCE);
	
	float3 x0 = xRatioFromEdge *sampledColors[0] + xRatioFromStart *sampledColors[1];
	float3 x1 = xRatioFromEdge *sampledColors[2] + xRatioFromStart *sampledColors[3];
	//return float4(x0 + x1, 1);
	float yFromEdge = (smaplingPositions[2].y - input.uv.y) / PIXEL_DISTANCE;
	float yFromSTART = ( input.uv.y- smaplingPositions[0].y) / PIXEL_DISTANCE;
	float3 y0 = x0 * yFromEdge;
	float3 y1 = x1 *yFromSTART;

	colorIndirect = (y0 + y1);// *(specularPower(normalize(posWorld.xyz - posEye.xyz), normalize(posWorld.xyz - lightPos), meNormal, specular));// pow(max(0, dot(eyeAndLight, -meNormal)), 10 * specular);
	//return float4(colorIndirect, 1);
	return float4(saturate( colorIndirect +saturate( colorDirect) ), 1);
	//return float4(input.uv*0.1,1,1);
}