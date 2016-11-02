
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	matrix view;
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{ 
	float3 position		: POSITION;
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 uvw			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output
	VertexToPixel output;

	// Copy the view matrix and remove translation
	matrix viewNoMove = view;
	viewNoMove._41 = 0;
	viewNoMove._42 = 0;
	viewNoMove._43 = 0;

	// Calculate output position
	matrix viewProj = mul(viewNoMove, projection);
	output.position = mul(float4(input.position, 1.0f), viewProj);

	// Ensure the vertex is at max depth
	output.position.z = output.position.w;

	// Use the raw vertex position as a direction
	// in space
	output.uvw = input.position;

	return output;
}