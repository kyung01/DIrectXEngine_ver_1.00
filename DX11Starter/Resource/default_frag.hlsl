
cbuffer lightData : register(b0)
{
	float4 DirLightColor;
	float3 DirLightDirection;

	float4 PointLightColor;
	float3 PointLightPosition;

	float3 CameraPosition;
};

// External texture-related data
Texture2D Texture		: register(t0);
Texture2D Rust			: register(t1);
Texture2D Spec			: register(t2);
SamplerState Sampler	: register(s0);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};


struct PS_OUTPUT
{
	float4 normal: SV_Target0;
	float4 worldPos: SV_Target1;
};

// Entry point for this pixel shader
PS_OUTPUT main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);


	PS_OUTPUT output;
	output.normal = float4((input.normal +1 ) *.5,1);
	output.worldPos = float4(input.worldPos,1);
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return output;
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
