
// Constant Buffer for external (C++) data
cbuffer externalData : register(b0)
{
	float3 worldSize;
	matrix world;
	matrix view;
	matrix projection;
};

// Struct representing a single vertex worth of data
struct VertexShaderInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float2 uv			: TEXCOORD;
};

// Out of the vertex shader (and eventually input to the PS)
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL0;
	float3 tangent		: NORMAL1;
	float3 biTangent	: NORMAL2;
	float4 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixel main(VertexShaderInput input)
{
	// Set up output
	VertexToPixel output;

	// Calculate output position
	matrix worldViewProj = mul(mul(world, view), projection);
	output.position = mul(float4(input.position, 1.0f), worldViewProj);

	// Get the normal to the pixel shader
	output.normal = mul(input.normal, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	output.tangent = mul(input.tangent, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	output.biTangent = mul(input.biTangent, (float3x3)world); // ASSUMING UNIFORM SCALE HERE!!!  If not, use inverse transpose of world matrix
	float4 worldPos = mul(float4(input.position, 1.0f), world);
														// Get world position of vertex
	output.worldPos = float4(worldPos.xyz / worldSize, (1 - output.position.z / output.position.w));

	// Pass through the uv
	output.uv = input.uv;
	//output.depth =( 1-output.position.z / output.position.w);

	return output;
}