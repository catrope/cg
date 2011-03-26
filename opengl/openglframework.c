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
#include "lodepng.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define SPHERE_N (20)


int mouseButtons[5] = {GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP, GLUT_UP};
int mouseX, mouseY, width, height;
GLfloat angleX = 0, angleY = 0, zoom = 1;
int apertureSamples = 1;
GLdouble apertureC = 3.0;
GLUquadric *quadric;
GLuint sunTexture, mercTexture, venusTexture, earthTexture;

GLuint initTexture(char* filename) {
	unsigned char* buffer;
	unsigned char* image;
	size_t buffersize, imagesize;
	GLuint texName;
	LodePNG_Decoder decoder;

	LodePNG_loadFile(&buffer, &buffersize, filename);
	LodePNG_Decoder_init(&decoder);
	decoder.infoRaw.color.colorType = 6; /* Load image as RGBA */
	LodePNG_decode(&decoder, &image, &imagesize, buffer, buffersize);
	if(decoder.error) {
		printf("Error reading in png image: %d\n", decoder.error);
		exit(1);
	} else {
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glGenTextures(1,&texName);
		glBindTexture(GL_TEXTURE_2D,texName);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,decoder.infoPng.width,
			decoder.infoPng.height,0, GL_RGBA,GL_UNSIGNED_BYTE,image);
	} 
	return texName;
} 

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
	GLfloat pos[] = {x, y, z, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}

void drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLuint tex) {
	glPushMatrix();
	glTranslated(x, y, z);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, tex);
	glScaled(1, 1, -1);
	gluSphere(quadric, r, SPHERE_N, SPHERE_N);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void display(void)
{
	int i;
	GLdouble r, theta;
	GLdouble top, bottom, left, right, aspect;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(zoom*2.0*atan2(height/2.0,1000)*180.0/M_PI,(GLdouble)width/(GLdouble)height,1,5000);
	
	glMatrixMode(GL_MODELVIEW);
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(0.0, 0.0, 1000, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	/*glTranslatef(200.0, 200.0, 400.00);*/
	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 0.0, 1.0);
	/*glTranslatef(-200.0, -200.0, -400.0);*/
	
	/* Light source: sun */
	setGlLight(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	setGlMaterial(0.8, 0.8, 0.4, 1, 0, 0, 8);
	gluSphere(quadric, 20, SPHERE_N, SPHERE_N);
	
	/* Mercury */
	drawSphere(-58.0, 0.0, 0.0, 2.440, mercTexture);
	
	/* Venus */
	drawSphere(108.0, 0.0, 0.0, 6.052, venusTexture);
	
	/* Earth */
	drawSphere(-150.0, 0.0, 0.0, 6.378, earthTexture);

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
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	glEnable(GL_DEPTH_TEST);
	
	/* Quadric for drawing spheres */
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricOrientation(quadric, GLU_OUTSIDE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	
	sunTexture = initTexture("sun.png");
	mercTexture = initTexture("mercury.png");
	venusTexture = initTexture("venus.png");
	earthTexture = initTexture("earth.png");

	/* Register GLUT callback functions */
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	glutMainLoop();

	return 0;
}

