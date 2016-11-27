#include "point_light.hlsl"

cbuffer global00 :register(b0)
{
	float4 lightColor;
	float3 lightPos;
	float3 lightDir;
};

// External texture-related data
Texture2D texture_diffuse		: register(t0);
Texture2D texture_normal		: register(t1);
Texture2D texture_specular		: register(t2);
Texture2D texture_displacement	: register(t3);

SamplerState sampler_default	: register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float4 worldPos		: POSITION1;
	float4 worldPosFromEye		: POSITION2;
	float2 uv			: TEXCOORD;
};


struct PS_OUTPUT
{
	float4 normal: SV_Target0;
	float4 flux: SV_Target1;
	//float4 texture_specular: SV_Target4; At some point...
};

// Entry point for this pixel shader
PS_OUTPUT main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);
	input.biTangent = normalize(input.biTangent);


	PS_OUTPUT output;
	float3 diffuse = texture_diffuse.Sample(sampler_default, input.uv);
	float specular = texture_specular.Sample(sampler_default, input.uv).x;
	float3 normal = texture_normal.Sample(sampler_default, input.uv).xyz * 2 - 1;
	//float3 dirLightToWorld = input.worldPosFromEye.xyz;//0, 5.5, 0
	//dirLightToWorld *= -1;
	//dirLightToWorld = normalize(dirLightToWorld);//0, 5.5, 0
	float3 dirLightToWorld = normalize(input.worldPos.xyz - lightPos );//0, 5.5, 0
	normal = normal.x * input.tangent +normal.y * input.biTangent + normal.z * input.normal;

	output.normal = float4((normal + 1) *.5, 1);
	//output.flux = texture_diffuse.Sample(sampler_default, input.uv);
	output.flux = spotLight(diffuse,lightColor,lightDir, input.worldPos.xyz - lightPos,dirLightToWorld, normal, specular);
	//output.flux = float4(dirLightToWorld, 1);
	//output.normal = float4(input.uv,0,1);
	 //output.normal = float4(input.worldPos.xyz, 1);
	 //output.worldPos = float4(input.worldPos.xyz, 1);
	//output.depth = float4(input.worldPos.w, input.worldPos.w, input.worldPos.w, 1);
	return output;
}

