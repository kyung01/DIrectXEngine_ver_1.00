
// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	//float3 worldSize;
	float3 lightPos;
	float4 lightColor;
	matrix matProjViewInverse; //used to unwrap depth to world
	matrix matLightViewProj; //used to wrap world to screen relative projected position
};

// External texture-related data
Texture2D textureDiffuse		: register(t0);
Texture2D textureNormal		: register(t1);
Texture2D textureDepth		: register(t2);
Texture2D textureLightDepth		: register(t3);

SamplerState samplerDefault	: register(s0);
// Struct representing a single vertex worth of data

struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};
// Out of the vertex shader (and eventually input to the PS)


float4 getPosProjected(float2 uv, Texture2D depthTexture) {


	return float4(
		uv.x * 2 - 1, (1 - uv.y) * 2 - 1,
		textureDepth.Sample(samplerDefault, uv).x, 1);
}
float4 main(VertexToPixel input) : SV_TARGET
{
	float4 output;
	float4x4 matBias = float4x4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0);
	float bias = 0.002;
	//values I can get from textures
	float4 posProjected = getPosProjected(input.uv, textureDepth);
	float3 diffuse = textureDiffuse.Sample(samplerDefault, input.uv);
	float3 normal = textureNormal.Sample(samplerDefault, input.uv) * 2 - 1;

	float4 posWorld = mul(posProjected, matProjViewInverse);
	posWorld /= posWorld.w;
	float4 posFromLightProjection = mul(posWorld, matLightViewProj);
	posFromLightProjection /= posFromLightProjection.w;
	float2 uv = float2(posFromLightProjection.x*0.5 + 0.5, 1 - (posFromLightProjection.y*0.5 + 0.5));
	//return float4(uv, 0, 1);
	//posFromLightProjection /= posFromLightProjection.w;
	//posFromLightProjection = mul(posFromLightProjection, matBias);

	float depthClosest = textureLightDepth.Sample(samplerDefault, uv  ).x ;

	float3 disFromLightToPos = posWorld.xyz - lightPos;
	float3 dirFromLightToPos = normalize(disFromLightToPos);

	float lighted = (posFromLightProjection.z  -bias< depthClosest );

	float powerSurfaceReflection = cos((   max(0,1 - dot(dirFromLightToPos, -normal)*10.0)  ) * 3.14/2 );
	return float4(powerSurfaceReflection, 0, 0, 1);
	//float powerSurfaceReflection = dot(dirFromLightToPos, -normal);
	//return float4(powerSurfaceReflection,0,0,1);
	float powerluminance =
		(lightColor.w) /
		(1 +
			disFromLightToPos.x * disFromLightToPos.x +
			disFromLightToPos.y * disFromLightToPos.y +
			disFromLightToPos.z * disFromLightToPos.z);
	output.xyz = diffuse*lightColor.xyz* lighted *powerluminance*powerSurfaceReflection;
	output.w = 1;
	return output;
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