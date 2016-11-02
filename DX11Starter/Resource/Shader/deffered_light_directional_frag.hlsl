
// Constant Buffer for external (C++) data
cbuffer global00 :register(b0)
{
	//float3 worldSize;
	matrix matProjInverse;
	matrix matLightViewProj;
};

// External texture-related data
Texture2D textureDiffuse		: register(t0);
Texture2D textureNormal		: register(t1);
Texture2D textureDepth		: register(t2);
Texture2D textureLightDepth		: register(t3);

SamplerState samplerDefault	: register(s0);
// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};

// Out of the vertex shader (and eventually input to the PS)



float4 main(VertexShaderInput input) : SV_TARGET
{
	float4 output;
	float4 diffuseColor = textureDiffuse.Sample(samplerDefault, input.uv);
	float4 posProjected = float4(
		input.uv.x * 2 - 1, ( input.uv.y) * 2 - 1,
		textureDepth.Sample(samplerDefault, input.uv).x, 1);
	float4 p = mul(posProjected,matProjInverse);
	p /= p.w;
	float4 posFromLightSource = mul(p, matLightViewProj);
	float2 uvNew = (posFromLightSource.xy*0.5 + 0.5)/ posFromLightSource.w ;
	float4 lightDepthRaw = textureLightDepth.Sample(samplerDefault, uvNew);// .x;
	float lightDepth = textureLightDepth.Sample(samplerDefault, uvNew).x;
	float ratio = posFromLightSource.z*lightDepth*0.00001;
	
//	return float4(lightDepthRaw.xyz, 1 + ratio);

	if (posFromLightSource.z - 0.08< lightDepth)

		output = float4(1, 1, 1, 1+ ratio)*diffuseColor;
	else output = float4(0, .1, .1, 1+ ratio)*diffuseColor;


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