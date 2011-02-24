/* Some convenience code for using GLSL shaders in your OpenGL program.
*/

#ifndef OPENGLFRAMEWORK_GLSLSHADERS_H
#define OPENGLFRAMEWORK_GLSLSHADERS_H

#if defined(NEED_GLEW)
#include "glew.h"
#endif

#if defined(__APPLE__)&& defined(__MACH__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

GLuint initGLSLProgram(char *vertexShaderFilename, char *fragmentShaderFilename);

#endif/*OPENGLFRAMEWORK_GLSLSHADERS_H*/
