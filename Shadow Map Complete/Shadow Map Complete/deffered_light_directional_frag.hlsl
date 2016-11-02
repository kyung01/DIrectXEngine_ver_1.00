
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
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float2 uv			: TEXCOORD;
};


float4 main(VertexToPixel input) : SV_TARGET
{
	float4 diffuseColor = textureDiffuse.Sample(samplerDefault, input.uv);
	float4 posProjected = float4(
		input.uv.x * 2 - 1, (1 - input.uv.y) * 2 - 1,
		textureDepth.Sample(samplerDefault, input.uv).x ,1);
	float4 p = mul(posProjected,matProjInverse);
	p.xyz /= p.w;
	p.w = 1;
	//return p;
	float4 posFromLightSource = mul(p, matLightViewProj);
	float lightDepth = textureLightDepth.Sample(samplerDefault, input.uv).x;
	return float4(lightDepth,0,0,1);

}