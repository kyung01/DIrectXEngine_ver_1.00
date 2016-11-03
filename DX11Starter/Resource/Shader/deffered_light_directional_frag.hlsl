
// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	//float3 worldSize;
	float3 lightPos;
	float3 lightDir;
	float4 lightColor;
	matrix matProjInverse; //used to unwrap depth to world
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



float4 main(VertexToPixel input) : SV_TARGET
{
	float4 output;
	float4 posProjected = float4(
		input.uv.x * 2 - 1, (1-input.uv.y) * 2 - 1,
		textureDepth.Sample(samplerDefault, input.uv).x, 1);
	float4 posWorld = mul(posProjected,matProjInverse);
	posWorld /= posWorld.w;

	float4 posFromLightSource = mul(posWorld, matLightViewProj);
	posFromLightSource /= posFromLightSource.w;
	float depthFarest = posFromLightSource.z;
	float bias = 0.0250000711;
	float2 index = posFromLightSource.xy* 0.5 + 0.5;
	float depthClosest = textureLightDepth.Sample(samplerDefault, float2( index.x, 1 -index.y ) ).x ;

	//return float4(depthClosest, 0, 0, 1);
	posFromLightSource.w = 1;
	//return posFromLightSource;
	return float4(0,1* (posFromLightSource.z -bias> depthClosest),0, 1);
	if (depthClosest-posFromLightSource.z  < 0) {
		return float4(1, 0, 0, 1);
	}
	return float4(0.1, 0.1, .1, 1);
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