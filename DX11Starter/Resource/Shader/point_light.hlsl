float4 getRed() {
	return float4(1, 0, 0, 1);
}

float getPower(float3 dirLightToWorld, float3 surfaceNormal) {
	//float3 lightDirReflected = (surfaceNormal * 2 * dot(surfaceNormal, lightDir)) - lightDir;
	//return dot(dirLightToWorld, );
	return max(0, dot(dirLightToWorld, -surfaceNormal) );
}
//used by reflective shadow map
float4 spotLightRSM(
	float3 diffuseColor,
	float4 lightColor, float3 lightDir, float3 disLightToWorld, float3 dirLightToWorld, float3 surfaceNormal) {
	float powerLuminance =
		(lightColor.w) /
		(1 +
			disLightToWorld.x * disLightToWorld.x +
			disLightToWorld.y * disLightToWorld.y +
			disLightToWorld.z * disLightToWorld.z);

	float lightMaxAngle = 0.5;
	float dotAngle = dot(lightDir, dirLightToWorld);
	float ratio = max(0, 1 - pow(-dotAngle + 1, 5) / pow(lightMaxAngle, 10));
	float isLightedSpotlight = (dotAngle > lightMaxAngle)*ratio;// *(dotAngle / lightMaxAngle);

	return float4(
		diffuseColor*lightColor.xyz *getPower(dirLightToWorld, surfaceNormal)*powerLuminance * isLightedSpotlight, 1);
}
float specularPower(float3 dirEyeToWorld, float3 dirLightToWorld, float3 surfaceNormal, float power) {
	float3 reflected = normalize(dirLightToWorld - 2 * dot(surfaceNormal, dirLightToWorld) *surfaceNormal);
	float cosAngle = dot(dirEyeToWorld, -reflected);
	//float3 eye = normalize(dirEyeToWorld + dirLightToWorld);
	return pow(max(0, cosAngle), 1 + 10 * (power));
}
float3 spotLight(
	float3 dirEyeToWorld,
	float3 diffuseColor,
	float4 lightColor, float3 lightDir, float3 disLightToWorld, float3 dirLightToWorld, float3 surfaceNormal, float luminosity) {
	float3 eye = normalize(dirEyeToWorld + dirLightToWorld);

	float powerLuminance =
		(lightColor.w) /
		(1 +
			disLightToWorld.x * disLightToWorld.x +
			disLightToWorld.y * disLightToWorld.y +
			disLightToWorld.z * disLightToWorld.z);

	float lightMaxAngle = 0.5;
	float dotAngle = dot(lightDir, dirLightToWorld);
	float ratio = max(0, 1 - pow(-dotAngle + 1, 5) / pow(lightMaxAngle, 10));
	float isLightedSpotlight = (dotAngle > lightMaxAngle)*ratio;// *(dotAngle / lightMaxAngle);

	return saturate(
		float3(diffuseColor*lightColor.xyz) *
		(	
			getPower(dirLightToWorld, surfaceNormal)*powerLuminance
			
			+ specularPower(dirEyeToWorld, dirLightToWorld, surfaceNormal, luminosity)
		)* isLightedSpotlight//pow(max(0, dot(eye, -surfaceNormal)), 32 * luminosity)
		
	);
}