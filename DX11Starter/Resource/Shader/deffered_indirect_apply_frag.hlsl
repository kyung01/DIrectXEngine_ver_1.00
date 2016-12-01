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
float getArea(float2 vertA, float2 vertB, float2 vertC) {
	float2 sideA = vertB - vertA;
	float2 sideB = vertC - vertB;
	float2 sideC = vertA - vertC;
	float a = length(sideA);
	float b = length(sideB);
	float c = length(sideC);

	float s = 0.5* (a + b + c);
	return sqrt(s*(s - a)*(s - b)*(s - c));
}
float3 linearFilter (float2 uv, float3 colors[4],float2 uvs[4], int checks[4]) {
	int count = 0;
	for (int i = 0; i < 4; i++) {
		if (checks[i] == -1) continue;
		colors[count] = colors[i];
		uvs[count] = uvs[i];
		count++;
	}
	float total = PIXEL_DISTANCE*PIXEL_DISTANCE / 2;
	float areaA = getArea(uv, uvs[1], uvs[2]);
	float areaB = getArea(uv, uvs[2], uvs[0]);
	float areaC = getArea(uv, uvs[0], uvs[1]);
	if (areaA / total > 1 || areaB / total > 1 || areaC / total > 1)
		return float3(0,0,0);
	float3 colorA = colors[0] * (areaA / total);
	float3 colorB = colors[1] * (areaB / total);
	float3 colorC = colors[2] * (areaC / total);
	return colorA + colorB + colorC;
}
float4 main(VertexToPixel input) : SV_TARGET
{
	float diffMaxLimit = 2.0;
	float3 colorDirect = textureLightDirect.Sample(samplerDefault, input.uv).xyz;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse);
	float3 meNormal = normalize(textureNormal.Sample(samplerDefault, input.uv).xyz * 2 - 1);
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
	int indexs[4] = { 0,1,2,3 };
	int failCount = 0;

	float3 colorIndirect = float3(0, 0, 0);
	float angleDiff = 0;
	float posDiffTotal = 0;
	for (int ii = 0; ii < 4; ii++) {
		smaplingPositions[ii] = float2(
			PIXEL_DISTANCE*floor(input.uv.x / PIXEL_DISTANCE) + smaplingPositions[ii].x,
			PIXEL_DISTANCE*floor(input.uv.y / PIXEL_DISTANCE) + smaplingPositions[ii].y);
		sampledColors[ii] = textureLightIndirect.Sample(samplerIndirectLight, smaplingPositions[ii]);// *(posDiff < 5 && dot(meNormal, otherNormal) > 0.5);
	}
	//return textureLightIndirect.Sample(samplerIndirectLight, input.uv);
	for (int i = 0; i < 4; i++) {


		float2 uvRelative = float2(
			PIXEL_DISTANCE*floor(smaplingPositions[i].x / PIXEL_DISTANCE) ,
			PIXEL_DISTANCE*floor(smaplingPositions[i].y / PIXEL_DISTANCE) 
			);
		
		//
		float4 otherPosWorld = getPosWorld(uvRelative, textureDepth, matProjViewInverse);
		float3 otherNormal = normalize(textureNormal.Sample(samplerIndirectLight, uvRelative).xyz * 2 - 1);
		////posDiffTotal += length(otherPosWorld.xyz - posWorld.xyz);
		float posDiff = 1 / (1+length(otherPosWorld.xyz - posWorld.xyz));
		if (dot(meNormal, otherNormal) < 0.11 || length(otherPosWorld.xyz - posWorld.xyz) > 1.0) {
			indexs[i] = -1;
			failCount++;
		}
	}
	if (failCount == 0) {

		//return float4(0, 0, 0, 1);

		//if (failCount == 2) {
		//	return float4(0,1,0, 1);
		//}
		//if (failCount == 4)return float4(0,1,0, 1);
		//if (failCount == 3)return float4(0,1,0, 1);
		float xRatioFromEdge = (smaplingPositions[1].x - input.uv.x) / PIXEL_DISTANCE;
		float xRatioFromStart = (input.uv.x - smaplingPositions[0].x) / PIXEL_DISTANCE;

		//float3 x00 = sampledColors[0] * (( ) / PIXEL_DISTANCE);
		//float3 x01 = sampledColors[1] * ((input.uv.x - smaplingPositions[0].x) / PIXEL_DISTANCE);
		//float3 x10 = sampledColors[2] * ((smaplingPositions[1].x - input.uv.x) / PIXEL_DISTANCE);
		//float3 x11 = sampledColors[3] * ((input.uv.x - smaplingPositions[0].x) / PIXEL_DISTANCE);

		float3 x0 = xRatioFromEdge *sampledColors[0] + xRatioFromStart *sampledColors[1];
		float3 x1 = xRatioFromEdge *sampledColors[2] + xRatioFromStart *sampledColors[3];
		//return float4(x0 + x1, 1);
		float yFromEdge = (smaplingPositions[2].y - input.uv.y) / PIXEL_DISTANCE;
		float yFromSTART = (input.uv.y - smaplingPositions[0].y) / PIXEL_DISTANCE;
		float3 y0 = x0 * yFromEdge;
		float3 y1 = x1 *yFromSTART;

		colorIndirect += (y0 + y1);// *(specularPower(normalize(posWorld.xyz - posEye.xyz), normalize(posWorld.xyz - lightPos), meNormal, specular));// pow(max(0, dot(eyeAndLight, -meNormal)), 10 * specular);
								   //return float4(colorIndirect, 1);
		return float4(saturate(colorIndirect + saturate(colorDirect)), 1);
	}
	if (failCount == 1) {
		//return float4(1, 0, 0, 1);
		return float4(saturate(colorDirect +
			linearFilter(input.uv, sampledColors, smaplingPositions, indexs)), 1);

	}
	return float4(1, 0, 1, 1);
	
	//return float4(input.uv*0.1,1,1);
}