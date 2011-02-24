/*
* An OpenGL template/framework file for the Computer Graphics course
* at the University of Groningen.
* Authors: Roan Kattouw and Jan Paul Posma
*/

// If windows is used, windows.h should be included (before gl.h and glu.h)
#if defined(_WIN32)
#include <windows.h>
#endif

// If you use GLUT you do not need to include gl.h and glu.h
// as glut.h guarantees that gl.h and glu.h are properly 
// included.
// So if you do not wish to use  glut.h, uncomment the following lines.
//#include <GL/gl.h>
//#include <GL/glu.h>

#if defined(NEED_GLEW)
#include "glew.h"
#endif

// Apparently Mac OS X puts the GLUT headers somewhere different.
// For windows we use freeglut.
#if defined(__APPLE__)&& defined(__MACH__)
#include <GLUT/glut.h>
#elif defined(_WIN32)
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

//    2-------6
//   /|      /|
//  4-------7 |
//  | |     | |
//  | |0----|-|3
//  |/      |/
//  1-------5

GLfloat cubeVertices[8*3] = {-1,-1,-1, -1,-1, 1, -1, 1,-1,  1,-1,-1, -1, 1, 1,  1,-1, 1,  1, 1,-1,  1, 1, 1};
GLfloat cubeColours[8*3]  = { 1, 0, 0,  0, 1, 0,  0, 0, 1,  1, 1, 0,  0, 1, 1,  1, 0, 1,  0.8, 0.5, 0.3,  0.3, 0.5, 0.8};
GLubyte cubeIndices[4*6] = {
		1,5,7,4, // front
		1,4,2,0, // left
		1,0,3,5, // bottom
		5,7,6,3, // right
		4,2,6,7, // top
		0,3,6,2, // back
	};

GLuint vboCubeVertices = 0;
GLuint vboCubeIndices = 0;
GLuint vboCubeColours = 0;

int mouseButtons[5] = {GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP};
int mouseX, mouseY, width, height;
GLfloat angleX = 0, angleY = 0, zoom = 1.0;

void initVBO(void)
{
	// build all the buffers and upload the array data
	glGenBuffersARB(1, &vboCubeVertices);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboCubeVertices);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW_ARB);

	glGenBuffersARB(1, &vboCubeIndices);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboCubeIndices);
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW_ARB);
	
	glGenBuffersARB(1, &vboCubeColours);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboCubeColours);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(cubeColours), cubeColours, GL_STATIC_DRAW_ARB);
}

void destroyVBO(void)
{
	// destroy all buffers
	glDeleteBuffersARB(1, &vboCubeVertices);
	glDeleteBuffersARB(1, &vboCubeIndices);
	glDeleteBuffersARB(1, &vboCubeColours);
}

void display(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom*60.0,(GLdouble)width/(GLdouble)height,1.5,20.0);
	glMatrixMode(GL_MODELVIEW);
	
	/* Clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0f,0.0f,1.0f);
	glLoadIdentity();
	gluLookAt(0.0,0.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);

	// use the buffer indices
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vboCubeIndices);

	// add the vertex array
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboCubeVertices);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	// add the colour array
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboCubeColours);
	glColorPointer(3, GL_FLOAT, 0, 0);

	// draw the quads
	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, 0);

	// unset the client states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	// unbind the buffers
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
	
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
		case 'Q':
		case 27: // ESC key
			printf("Exiting...\n");
			exit(0);
			break;
	}
}

void motion(int x, int y)
{
	int deltaX, deltaY;
	
	// calculate the difference with the previous mouse postions
	deltaX = x - mouseX;
	deltaY = y - mouseY;
	
	// update the mouse positions
	mouseX = x;
	mouseY = y;
	
	if (mouseButtons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
	{
		// update and normalise the angles
		angleY += 0.3*deltaX;
		while(angleY<-180.0) angleY += 360.0;
		while(angleY>180.0) angleY -= 360.0;

		angleX += 0.3*deltaY;
		while(angleX<-180.0) angleX += 360.0;
		while(angleX>180.0) angleX -= 360.0;
		
		glutPostRedisplay(); 
	}
	
	if (mouseButtons[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
	{
		zoom *= exp(0.001*deltaY);
		printf("%f ", zoom); fflush(stdout);
		glutPostRedisplay(); 
	}
}

void mouse (int button, int state, int x, int y) 
{
	// store what buttons are pressed
	mouseButtons[button] = state;
	
	// store the mouse positions
	mouseX = x;
	mouseY = y;
}

void reshape(int w, int h)
{
	glViewport(0,0, (GLsizei) w, (GLsizei) h);
	width = w;
	height = h;
}

int main(int argc, char** argv)
{
#if defined(NEED_GLEW)
	GLenum err;
#endif


	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(220,100);
	glutCreateWindow("Computer Graphics - OpenGL framework");
	
	initVBO();

#if defined(NEED_GLEW)
	/* Init GLEW if needed */
	err = glewInit();
	if (GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(1);
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
	
	/* Select clearing (background) color */
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	/* Register GLUT callback functions */
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();
	
	destroyVBO();

	return 0;
}

