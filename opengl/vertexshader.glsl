#version 120
varying vec3 N;
void main()
{
	gl_Position = ftransform();
	N = gl_NormalMatrix * gl_Normal;
}
