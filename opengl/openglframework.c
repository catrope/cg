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

#include "glslshaders.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SPHERE_N (20)


int mouseButtons[5] = {GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP};
int mouseX, mouseY, width, height;
GLfloat angleX = 0, angleY = 0, zoom = 1.0;

void setGlMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat ka, GLfloat kd, GLfloat ks, GLfloat n)
{
	GLfloat ambient[] = {ka*r,ka*g,ka*b,1.0};
	GLfloat diffuse[] = {kd*r,kd*g,kd*b,1.0};
	GLfloat specular[] = {ks,ks,ks,1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, n);
}

void setGlLight(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b) {
	GLfloat ambient[] = {r, g, b, 1.0};
	GLfloat diffuse[] = {r, g, b, 1.0};
	GLfloat specular[] = {r, g, b, 1.0};
	GLfloat pos[] = {x, y, z};
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void display(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom*2.0*atan2(height/2.0,1000.0)*180.0/M_PI,(GLdouble)width/(GLdouble)height,500,1000);
	glMatrixMode(GL_MODELVIEW);
	
	/* Clear all pixels */
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(200.0,200.0,1000.0,200.0,200.0,0.0,0.0,1.0,0.0);
	glTranslatef(200.0, 200.0, 200.00);
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glTranslatef(-200.0, -200.0, -200.0);

	setGlLight(-200.0, 600.0, 1500.0, 1.0, 1.0, 1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	setGlMaterial(0.0f,0.0f,1.0f,0.2,0.7,0.5,64);
	glPushMatrix();
	glTranslated(90,320,100);
	glutSolidSphere(50,SPHERE_N,SPHERE_N);
	glPopMatrix();

	setGlMaterial(0.0f,1.0f,0.0f,0.2,0.3,0.5,8);
	glPushMatrix();
	glTranslated(210,270,300);
	glutSolidSphere(50,SPHERE_N,SPHERE_N);
	glPopMatrix();

	setGlMaterial(1.0f,0.0f,0.0f,0.2,0.7,0.8,32);
	glPushMatrix();
	glTranslated(290,170,150);
	glutSolidSphere(50,SPHERE_N,SPHERE_N);
	glPopMatrix();

	setGlMaterial(1.0f,0.8f,0.0f,0.2,0.8,0.0,1);
	glPushMatrix();
	glTranslated(140,220,400);
	glutSolidSphere(50,SPHERE_N,SPHERE_N);
	glPopMatrix();

	setGlMaterial(1.0f,0.5f,0.0f,0.2,0.8,0.5,32);
	glPushMatrix();
	glTranslated(110,130,200);
	glutSolidSphere(50,SPHERE_N,SPHERE_N);
	glPopMatrix();

	/* Whatever clean up you need */
	/* ... */

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom*2.0*atan2(h/2.0,1000.0)*180.0/M_PI,(GLdouble)w/(GLdouble)h,500,1000);
	glMatrixMode(GL_MODELVIEW);
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
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(220,100);
	glutCreateWindow("Computer Graphics - OpenGL framework");
	initGLSLProgram("vertexshader.glsl", "fragmentshader.glsl");
	
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
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	/* Register GLUT callback functions */
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}

