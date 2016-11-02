
cbuffer lightData : register(b0)
{
	float4 AmbientLightColor;

	float4 DirLightColor;
	float3 DirLightDirection;
	
	float4 PointLightColor;
	float3 PointLightPosition;

	float3 CameraPosition;
};

// External texture-related data
Texture2D Texture		: register(t0);
Texture2D NormalMap		: register(t1);
Texture2D ShadowMap		: register(t2);
TextureCube Sky			: register(t3);
SamplerState Sampler	: register(s0);
SamplerComparisonState ShadowSampler : register(s1);

// Defines the input to this pixel shader
// - Should match the output of our corresponding vertex shader
struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float3 tangent		: TANGENT;
	float3 worldPos		: POSITION;
	float2 uv			: TEXCOORD;
	float4 posForShadow : TEXCOORD1;
};


// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	input.tangent = normalize(input.tangent);

	// "Do normal mapping" -------------------------

	// NOTE: Flip the uv's Y coord - models are wrong
	input.uv.y = 1.0f - input.uv.y;

	// Sample and "unpack" the normal
	float3 normalFromMap = NormalMap.Sample(Sampler, input.uv).rgb * 2 - 1;

	// Calculate my TBN matrix to get the normal into world space
	float3 N = input.normal;
	float3 T = normalize(input.tangent - N * dot(input.tangent, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	input.normal = normalize(mul(normalFromMap, TBN));

	// Directional light calculation ---------------
	// (need direction TO the light)
	// Normalize light direction to be safe
	float dirLightAmount = saturate(dot(input.normal, -normalize(DirLightDirection)));

	// Point light calculation ---------------------
	// Figure out direction to the light
	float3 dirToPointLight = normalize(PointLightPosition - input.worldPos);
	float pointLightAmount = saturate(dot(input.normal, dirToPointLight));

	// Specular (for point light) ------------------
	float3 toCamera = normalize(CameraPosition - input.worldPos);
	float3 refl = reflect(-dirToPointLight, input.normal);
	float spec = pow(max(dot(refl, toCamera), 0), 32);

	// Shadow map calculation

	// Figure out this pixel's UV in the SHADOW MAP
	float2 shadowUV = input.posForShadow.xy / input.posForShadow.w * 0.5f + 0.5f;
	shadowUV.y = 1.0f - shadowUV.y; // Flip the Y since UV coords and screen coords are different

	// Calculate this pixel's actual depth from the light
	float depthFromLight = input.posForShadow.z / input.posForShadow.w;

	// Sample the shadow map
	float shadowAmount = ShadowMap.SampleCmpLevelZero(ShadowSampler, shadowUV, depthFromLight);
	

	// Final colors ------------------------------------------

	// Get diffuse (albedo) color
	float4 diffuseColor = Texture.Sample(Sampler, input.uv);

	// Light colors
	//float4 skyColor = Sky.Sample(Sampler, reflect(-toCamera, input.normal));
	float4 ambient = AmbientLightColor * diffuseColor;
	float4 dirDiffuse = DirLightColor * dirLightAmount * diffuseColor;
	float4 pointDiffuse = PointLightColor * pointLightAmount * diffuseColor;
	float4 pointSpec = float4(spec.xxx, 1);

	// Add all light contributions
	return ambient + dirDiffuse * shadowAmount;
	// Skipping point light for now: // + pointDiffuse + pointSpec;
}