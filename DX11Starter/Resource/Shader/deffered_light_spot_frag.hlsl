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
float spotLight(float3 surfaceNormal, float3 dirEyeToWorld, float3 dirLightToWorld, float luminosity) {
	float3 lightDirReflected = (surfaceNormal * 2 * dot(surfaceNormal, lightDir)) -lightDir;
	float3 eye = normalize(dirEyeToWorld + dirLightToWorld);
	//float alongAxis = dot(reflected, -surfaceNormal);
	//float f = dot(reflected, normalize(surfacePos));
	return max(0,dot(dirLightToWorld, -surfaceNormal) + pow(dot(eye, -surfaceNormal), 50));
}

static float RSM_RND[200] = { 
	0.2327913410183
,0.65997414973563
,0.12462154502264
,0.74633705231656
,0.52386287437932
,0.88262292318168
,0.61554523213559
,0.44499360511312
,0.6130950034657
,0.91676919437794
,0.64255272999525
,0.70971401441363
,0.5613439197472
,0.4125660045131
,0.43557428309488
,0.80545519981787
,0.21057828199611
,0.88805355079847
,0.71250231457525
,0.64155554289071
,0.068623270871408
,0.36537500767288
,0.461374088871
,0.865835898959
,0.59392159133867
,0.93557846030946
,0.64043913578635
,0.85829120448711
,0.15096233466219
,0.38502593077022
,0.66749666336342
,0.15722010524814
,0.84482033869476
,0.936838092253
,0.88696938142505
,0.14247927076299
,0.37491727917218
,0.75763150107005
,0.63732183940584
,0.21840627687909
,0.38055854634408
,0.74638755887113
,0.60327729657445
,0.87955841556171
,0.64822624514262
,0.29369923625779
,0.64012343140325
,0.77106319916018
,0.43697770425909
,0.075619523448692
,0.86855701490704
,0.94237220377772
,0.13170516543635
,0.6957407271004
,0.30531955384897
,0.85081060782578
,0.1474253033043
,0.12735367479145
,0.95168824212239
,0.43165171678721
,0.55988681528712
,0.028901655706066
,0.026236686867772
,0.33956322601976
,0.12351968797088
,0.059477283181379
,0.15316710069457
,0.50767976674609
,0.79587328890146
,0.78382871755577
,0.34305644610108
,0.062241946376041
,0.80427669538384
,0.35345268405669
,0.6804948675821
,0.3442584920415
,0.30741343335594
,0.46087680825073
,0.45355962563938
,0.016312953092304
,0.82345152218987
,0.796408924645
,0.14102519496392
,0.28973440885997
,0.99899616511492
,0.84618982339566
,0.3188750214497
,0.0094399028501659
,0.44300369659579
,0.91501925695456
,0.34764018950408
,0.98310754819918
,0.27207221475992
,0.94784625710354
,0.47441029710435
,0.83640976708215
,0.71784059364248
,0.65005088860637
,0.23808651754543
,0.44582165612179
,0.22902442199598
,0.72527561743058
,0.61762901470886
,0.3569618795798
,0.97233686362036
,0.84343206875186
,0.037468584271832
,0.98860592347971
,0.13724810915871
,0.52888817923557
,0.70912780878559
,0.67678856601789
,0.33091585353525
,0.089111612219881
,0.32348389147012
,0.73671682027016
,0.9101065625018
,0.20803164234759
,0.57251712939354
,0.33518500362299
,0.75314408948326
,0.63742703741297
,0.23022144158847
,0.16966507591757
,0.98814619145735
,0.84912772702478
,0.060766503708794
,0.20105154542301
,0.93390774956621
,0.022513534884208
,0.9317517145219
,0.11910721059847
,0.21642315304672
,0.88165351184162
,0.51626296318893
,0.96674577471183
,0.79346984941208
,0.18568976464946
,0.54379102706154
,0.70953787849729
,0.52610565979318
,0.38247200259123
,0.44805985058102
,0.12461652798793
,0.13618271338576
,0.51160636614617
,0.32203717265373
,0.39583642054155
,0.46375127437699
,0.60721155330875
,0.30439674402792
,0.37090414314107
,0.28546703433873
,0.80493301609761
,0.79009358994201
,0.47663108421333
,0.60815546038009
,0.50075737922488
,0.78809598450926
,0.33425952602842
,0.541632502592
,0.031693998738981
,0.13049576716986
,0.51889740886115
,0.68532245731229
,0.90884100734668
,0.94122739506011
,0.28947959620947
,0.66455887102734
,0.23900908196299
,0.40012445133185
,0.89359003673009
,0.25842895091438
,0.041333955266203
,0.068949498268286
,0.078111315648123
,0.29704564730499
,0.39064779150796
,0.1887232061423
,0.69346906789274
,0.16389068084019
,0.10078267105891
,0.41963478616422
,0.80559529261924
,0.98370412084446
,0.9638472026977
,0.038251912704786
,0.87607005884688
,0.34302677136987
,0.5484520627877
,0.6280813061763
,0.7273874840361
,0.81129091783021
,0.19405516618586
,0.41593148206171
,0.46075286877377
,0.98430778551116
,0.28684311326912
,0.61218644893364
,0.040696600936678

};
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
	float bias = 0.000050;
	//values I can get from textures
	float3 diffuse = textureDiffuse.Sample(samplerDefault, input.uv);
	float specular = textureSpecular.Sample(samplerDefault, input.uv).x;
	float3 normal = textureNormal.Sample(samplerDefault, input.uv) * 2 - 1;
	
	float4 posEye = mul(float4(0, 0, 0, 1), matProjViewInverse);
	posEye /= 0.00000001 + posEye.w;
	float4 posWorld = getPosWorld(input.uv, textureDepth, matProjViewInverse);
	float3 disFromEyeToWorld = posWorld.xyz-posEye.xyz;
	float4 posFromLightProjection = mul(posWorld, matLightProjView);
	posFromLightProjection /=0.000000001+ posFromLightProjection.w;
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
	
	//lighted = getShadowAt(posWorld, bias);// (posFromLightProjection.z - bias < lighted);
	//float lighted = (posFromLightProjection.z  -bias< depthClosest );
	

	//float powerSurfaceReflection = cos((   max(0,1 - dot(dirFromLightToPos, -normal)*10.0)  ) * 3.14/2 ); // not good
	//float powerSurfaceReflection = dot(dirFromLightToPos, -normal);
	//return float4(powerSurfaceReflection,0,0,1);
	//float powerLuminance =
	//	(lightColor.w) /
	//	(1 +
	//		disFromLightToPos.x * disFromLightToPos.x +
	//		disFromLightToPos.y * disFromLightToPos.y +
	//		disFromLightToPos.z * disFromLightToPos.z);

	//float lightMaxAngle = 0.5;
	//float dotAngle = dot(lightDir, dirFromLightToPos);
	//float ratio = max(0, 1 - pow(-dotAngle + 1 ,5) / pow(lightMaxAngle, 10) );
	//float isLightedSpotlight = (dotAngle > lightMaxAngle)*ratio;// *(dotAngle / lightMaxAngle);

	//output.flux = spotLight(diffuse, lightColor, lightDir, input.worldPos.xyz - lightPos, dirLightToWorld  , normal, specular);
	//return float4(
	//	diffuse.xyz * lightColor.xyz*powerLuminance * spotLight(normal, dirFromEyeToPos, dirFromLightToPos, 2)  , lighted * isLightedSpotlight);

	/*
	
	//return posFromLightSource;
	return float4(output.x, powerReflective* lightPower* shadow, 0, 1);
	if (depthClosest-posFromLightSource.z  < 0) {
		return float4(1, 0, 0, 1);
	}
	return float4(0.1, 0.1, .1, 1);
	*/
	//return float4(depthClosest, 0, depthFarest, 1);
	/*
	//float4 lightPos = mul(float4(0, 0, 0, 1) ,matProjInverse);
	//float3 lightDir = normalize(mul(float4(0, 0, 1, 1), matProjInverse).xyz - lightPos.xyz);
	//float3 lightDirToMe = normalize(posWorld.xyz - lightPos.xyz);



	float3 posFromLightSourceWorld = posWorld.xyz - lightPos;
	float2 uvNew = (posFromLightSource.xy*0.5 + 0.5);// / posFromLightSource.w;
	float4 lightDepthRaw = textureLightDepth.Sample(samplerDefault, uvNew);// .x;
	float depthClosest = textureLightDepth.Sample(samplerDefault, uvNew).x ;
	float lightPower =
		(lightColor.w) /
		(lightColor.w + 
			posFromLightSourceWorld.x *posFromLightSourceWorld.x +
			posFromLightSourceWorld.y*posFromLightSourceWorld.y +
			posFromLightSourceWorld.z*posFromLightSourceWorld.z);
	//return float4(, 1 + ratio);
	float4 normal = textureNormal.Sample(samplerDefault, input.uv);
	float4 diffuseColor = textureDiffuse.Sample(samplerDefault, input.uv);
	float intensity = max(0, dot(-lightDir, normal.xyz*2 -1));
	float shadow = 0;
	//return float4(0, depthClosest, 0, 1);
	if (posFromLightSource.z / posFromLightSource.w< depthClosest) {
		//render
		return float4(0, 1, 0, 1);
	}
	return float4(.1, .1, .1, 1);

	//float shadow = min(1, max(0, (lightDepth - (posFromLightSource.z - 0.08))) * 100 );
	//output = diffuseColor.xyz*intensity * shadow;
	output.xyz = (diffuseColor.xyz*intensity  * lightColor.xyz* lightPower) * shadow;// *shadow;
	output.w = shadow;


	//else output = float4(0, 0, 1, 1+ ratio)*diffuseColor;
	//output = float4(0, posFromLightSource.z +0.5, ratio, 1);
	//output = float4(uvNew, -lightDepth *100+ posFromLightSource.z*lightDepth*0.00001, 1);
	//utput = lightDepthRaw;
	//if (posFromLightSource.z  < lightDepth)
	//	output = float4(1, 0, 0, 1+ ratio)*diffuseColor;
	//else output = float4(0, 0, 1, 1+ ratio)*diffuseColor;
	return output;
	
	/*
	//return textureLightDepth.Sample(samplerDefault, input.uv);
	if (lightDepth == 0)
		return float4(1, 1, 1, 1);
	if (posFromLightSource.z - lightDepth>0) {
		return float4(0, 0,1, 1)*diffuseColor;

	}
	return float4(abs(lightDepth), 0, 0, 1)*diffuseColor;
	return float4(posFromLightSource.z,0,0,1);
	if(posFromLightSource.x <-1.1)
		return float4(1,0, 0, 1)*diffuseColor;
	else return float4(0, 0, 1, 1)*diffuseColor;
	lightDepth = 1;
	return lightDepth;
	float depthMe = posFromLightSource.z;
	float depthLight = lightDepth.x;
	//return float4( posFromLightSource.xy/ posFromLightSource.w * 0.5+0.5,0, 1);
	if (depthMe < depthLight)
		return float4(1, 1, 1, 1)*diffuseColor;
	return float4(.2, .2, .2, 1)*diffuseColor;
	return float4(input.uv*100,1,1);
	return textureDiffuse.Sample(samplerDefault, input.uv);
	*/
	
}