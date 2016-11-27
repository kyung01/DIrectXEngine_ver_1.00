float4 getRed() {
	return float4(1, 0, 0, 1);
}

float getPower(float3 dirLightToWorld, float3 surfaceNormal, float luminosity) {

	//float3 lightDirReflected = (surfaceNormal * 2 * dot(surfaceNormal, lightDir)) - lightDir;
	//return dot(dirLightToWorld, );
	return max(0, dot(dirLightToWorld, -surfaceNormal) *luminosity);
}
float4 spotLight(
	float3 diffuseColor,
	float4 lightColor, float3 lightDir, float3 disLightToWorld, float3 dirLightToWorld, float3 surfaceNormal, float luminosity) {
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

	return float4(diffuseColor*lightColor.xyz *getPower(dirLightToWorld, surfaceNormal, luminosity)*powerLuminance * isLightedSpotlight, 1);
}
float4 spotLight(
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

	return float4(diffuseColor*lightColor.xyz *
		(getPower(dirLightToWorld, surfaceNormal, luminosity) + +pow(dot(eye, -surfaceNormal), 10) )
		
		*powerLuminance * isLightedSpotlight, 1);
}