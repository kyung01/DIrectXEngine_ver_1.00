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
		depthTexture.Sample(samplerDefault, uv).x, 1);
	posWorld = mul(posWorld, matProjViewInverse);
	posWorld /= 0.00000001 + posWorld.w;

	return posWorld;
}
/*
float spotLight(float3 surfaceNormal, float3 lightDir, float3 dirEyeToWorld, float luminosity) {
float3 lightDirReflected = (surfaceNormal * 2 * dot(surfaceNormal, lightDir)) -lightDir;
//float alongAxis = dot(reflected, -surfaceNormal);
//float f = dot(reflected, normalize(surfacePos));
return  pow(max(0, dot(dirEyeToWorld, lightDirReflected)), 3);// dot(surfaceNormal, -dirLightToPos)* pow(dot(surfaceNormal, -lightDir), 5);
//f = abs( f);
//return pow(f,3);
//return dot(surfaceNormal, -dirLightToPos)*0.1 + pow(f,3);// pow(, luminosity);
}

*/


static float RSM_RND[100] = { 
	0.29590568332742
,0.41743820319764
,0.13382127002525
,0.18318340283967
,0.8100260686176
,0.047784169692446
,0.071989211287344
,0.53041597666704
,0.09080183417108
,0.10177050489083
,0.98117514000329
,0.089178184088868
,0.86606879619233
,0.13237884833123
,0.70882361135856
,0.76025760395464
,0.036645643430131
,0.92027377286939
,0.46810587377665
,0.90505280201559
,0.88863488514378
,0.66955663294976
,0.6227704028705
,0.040741108376878
,0.59035164704097
,0.7004268493971
,0.17147452764747
,0.13375828933611
,0.15168734740079
,0.68052200492496
,0.63148507039597
,0.16145673587986
,0.43561000164394
,0.83137130217225
,0.0050628217892083
,0.27896004462566
,0.85218585741342
,0.87010870029689
,0.87562631623616
,0.45899393617129
,0.42812792417972
,0.52522369871159
,0.10367382182911
,0.082821500060531
,0.12923910195438
,0.24033515678734
,0.88863384718477
,0.12877347791976
,0.79208160601188
,0.29554569362455
,0.0048673893347696
,0.79571751216227
,0.92075355347281
,0.14072971890714
,0.17210425863606
,0.82600199609343
,0.67973155466827
,0.44238646675012
,0.53271645099517
,0.88774550421524
,0.093337823214632
,0.80329355728035
,0.67883591059541
,0.57065419693042
,0.8104674587075
,0.034169195701447
,0.094773254867072
,0.062645989033694
,0.21199834263511
,0.23935642477095
,0.82256846121632
,0.42372704410168
,0.075496244745094
,0.51429333189237
,0.78702556006006
,0.11988947685803
,0.37866540177663
,0.86932832834745
,0.50573449977941
,0.93987428859802
,0.86120072466377
,0.19229293437316
,0.505536929474
,0.4383238351151
,0.33058575835572
,0.34624211180315
,0.77414629504743
,0.92142584171213
,0.59397554890903
,0.26408057951558
,0.030105168479544
,0.49250235664309
,0.089778136503779
,0.48004923503848
,0.2971716287067
,0.70431401985898
,0.85945018746865
,0.62556135450749
,0.4154598495995
,0.83369169050534


};
static float RSM_RND_LENGTH[100] = {
 0.30736099244438
,0.92660360873053
,0.66044586555122
,0.79180850591129
,0.054900341227139
,0.29958385429326
,0.010456602093976
,0.14739603323275
,0.3080204484556
,0.81187630063476
,0.20616662791286
,0.26788632444473
,0.39385606972215
,0.74476688902116
,0.99123163008654
,0.096320238474906
,0.93392351173513
,0.7307273478856
,0.71987126242363
,0.60290341572971
,0.23895098838906
,0.85165073622561
,0.48868495108964
,0.082387876269588
,0.85550034179143
,0.27649479372264
,0.65410484636859
,0.050252391514486
,0.73751128359582
,0.72014490595094
,0.58597103300782
,0.6694380523029
,0.7666620801979
,0.62740687263543
,0.86824895016302
,0.12687068485044
,0.39875629609393
,0.37110043800022
,0.55415716001492
,0.45018113285777
,0.91914555380081
,0.29071704544626
,0.23026779584133
,0.99484459450228
,0.06492251952408
,0.43752914594371
,0.26160786173381
,0.10104218223181
,0.72825572953013
,0.38359587005507
,0.17632663397879
,0.50784686883346
,0.9909382923464
,0.8270677536852
,0.96880994642564
,0.82770519835302
,0.10907876263795
,0.56159126551896
,0.2241848480069
,0.39957770490999
,0.071392852380589
,0.86319080594144
,0.92153804885295
,0.9321715864037
,0.78869161745007
,0.77361783654132
,0.62129625893258
,0.069340688208742
,0.1179838125212
,0.71738911220682
,0.42113255356491
,0.094206097579657
,0.42432559347913
,0.7261734668753
,0.29274555123073
,0.14110486728191
,0.94934911511342
,0.32272235365711
,0.96583492633227
,0.49574940721306
,0.26301765547275
,0.58096059671648
,0.56860801743744
,0.056316336177437
,0.81954459045992
,0.40970279714544
,0.35342449664763
,0.036607869452149
,0.745971437891
,0.2518574033174
,0.28768258555219
,0.42272533449471
,0.709542144886
,0.37594491819662
,0.85356229443735
,0.87369922728916
,0.68225410658971
,0.47773826470493
,0.70689343321458
,0.95382645677488
};

float3 getFluxColor(
	float3 dirEyeToWorld,
	float2 uv,
	float3 posWorld, float3 normal, float specular,
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
	return lightFlux*result *(1+ min(1,specularPower(dirEyeToWorld,normalize(0.000001 + posWorld - lightPosWorld), normal, specular) ) ) ;// / (1 + disMag);
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
	float bias = 0.000050;
	//values I can get from textures
	float3 diffuse = textureDiffuse.Sample(samplerDefault, input.uv);
	float specular = textureSpecular.Sample(samplerDefault, input.uv).x;
	float3 normal = normalize(textureNormal.Sample(samplerDefault, input.uv) * 2 - 1);
	
	float4 posEye = mul(float4(0, 0, 0, 1), matProjViewInverse);
	posEye /= 0.00000001 + posEye.w;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse);
	float3 disFromEyeToWorld = posWorld.xyz-posEye.xyz;
	float3 dirEyeToWorld = normalize(disFromEyeToWorld);
	float4 posFromLightProjection = mul(posWorld, matLightProjView);
	posFromLightProjection /=0.000000001+ posFromLightProjection.w;
	float2 uv = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	float2 uvTemp = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	float3 fluxColor = float3(0,0,0);

	float theRange = length(posWorld.xyz - posEye.xyz);
	//theRange = -pow(theRange - 1, 2) + 1;
	
	float distanceMax = .1;
	//float	ratioPower = 10.0;
	float iMax = 100;
	
	//distanceMax =min(1, 0.15*(1+ theRange) );
	//ratioPower = 0.955;	
	for (float angleInit = 0; angleInit < 1; angleInit++) {

		for (float i = 0; i < iMax; i++) {
			float distance = RSM_RND_LENGTH[i] * distanceMax;// / iMax);
			float x = saturate(uv.x + cos(angleInit + 6.28*RSM_RND[i])*distance), y = saturate(uv.y + sin(angleInit + 6.28*RSM_RND[i])*distance);
			//float x = RSM_RND[i * 2], y = RSM_RND[i*2+1];
			float xx = x - uv.x, yy = y - uv.y;
			float ratio = sqrt(xx*xx + yy*yy);
			fluxColor += (ratio)* getFluxColor(
				dirEyeToWorld,
				float2(x, y), posWorld, normal, specular,
				textureLightDepth, textureLightNormal, textureLightRSM, matLightProjViewInverse);

		}
	}
	//fluxColor *= 2.0;
	fluxColor = saturate(fluxColor);
	return float4(fluxColor,1);
	
	
}