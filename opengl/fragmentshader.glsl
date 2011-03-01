#version 120
varying vec3 L, N, R, V;
void main()
{
	vec4 finalColor = vec4(0.0);
	float NL, VR;
	vec3 N2, L2, R2, V2;
	
	/* N, L, R and V are interpolated, so we need to renormalize them */
	N2 = normalize(N);
	L2 = normalize(L);
	R2 = normalize(R);
	V2 = normalize(V);
	
	/* Ambient lighting */
	finalColor += gl_FrontLightProduct[0].ambient;
	
	/* Diffuse lighting */
	NL = dot(N2, L2);
	if (NL > 0.0) {
		finalColor += gl_FrontLightProduct[0].diffuse * NL;
	}
	
	/* Specular lighting */
	VR = dot(V2, R2);
	if (VR > 0.0) {
		finalColor += gl_FrontLightProduct[0].specular * pow(VR, gl_FrontMaterial.shininess);
	}
	
	gl_FragColor = finalColor;
}
