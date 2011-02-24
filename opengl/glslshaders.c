/* Some convenience code for using GLSL shaders in your OpenGL program.
*/

#include "glslshaders.h"
#include <stdio.h>
#include <stdlib.h>

char * readTextFile(FILE * fp)
{
	char * text;
	long numChars;
	fseek(fp, 0L, SEEK_END);
	/* Position to end of file */
	numChars = ftell(fp);
	rewind(fp);
	text = (char*)calloc(numChars+1, sizeof(char));
	size_t numRead = fread(text, sizeof(char), numChars, fp);
	if (numRead != numChars) {
		fprintf(stderr,"Error reading from file.\n");
	}
    text[numChars] = 0;
	return text;
}


/* Create shader instance and read source from file system */ 
GLuint loadShader(const char * srcPath, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	FILE * fp = fopen(srcPath, "rb");
	char *src = readTextFile(fp);
	fclose(fp);
	glShaderSource(shader,1,(const GLchar**)&src,NULL);
	free(src);
	return shader;
} 

void checkCompileStatus(int shader, const char * srcPath)
{
	glCompileShader(shader);
	int compileStatus;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compileStatus);
	if (compileStatus==GL_FALSE) {
		fprintf(stderr,"Error, unable to compile shader: %s\n",srcPath);
		int logSize;
		glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&logSize);
		char *log = malloc(sizeof(char)*logSize+1);
		int actualLogSize;
		glGetShaderInfoLog(shader,logSize,&actualLogSize,log);
		fprintf(stderr,"Compile message:%s\n",log);
		free(log);
	}
}

GLuint initShader(const char * srcPath, GLenum shaderType)
{
	GLuint shader = loadShader(srcPath,shaderType);
	checkCompileStatus(shader,srcPath);
	return shader;
}

GLuint initGLSLProgram(char *vertexShaderFilename, char *fragmentShaderFilename)
{
	GLuint shaderProgram;
	GLuint vertexShader,fragmentShader;
	vertexShader = initShader(vertexShaderFilename,GL_VERTEX_SHADER);
	fragmentShader = initShader(fragmentShaderFilename,GL_FRAGMENT_SHADER);
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram,vertexShader);
	glAttachShader(shaderProgram,fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	return shaderProgram;
}
