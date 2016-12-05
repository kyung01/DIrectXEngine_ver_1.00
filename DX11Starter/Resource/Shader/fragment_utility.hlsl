float4 getPosWorld(
	float2 uv, Texture2D depthTexture, matrix matProjViewInverse,sampler samplerClamp) {
	float4 posWorld = float4(
		uv.x * 2 - 1, (1 - uv.y) * 2 - 1,
		depthTexture.Sample(samplerClamp, uv).x, 1);
	posWorld = mul(posWorld, matProjViewInverse);
	posWorld /= 0.00000001 + posWorld.w;

	return posWorld;
}

float specularPower(float3 dirEyeToWorld, float3 dirLightToWorld, float3 surfaceNormal, float power) {
	float3 reflected = normalize(dirLightToWorld - 2 * dot(surfaceNormal, dirLightToWorld) *surfaceNormal);
	float cosAngle = dot(dirEyeToWorld, -reflected);
	//float3 eye = normalize(dirEyeToWorld + dirLightToWorld);
	return pow(max(0, cosAngle), 1 + 10 * (power));
}