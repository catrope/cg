#version 120
varying vec3 N;
void main()
{
	gl_FragColor = vec4(0.5*normalize(N)+0.5,1.0);
}
