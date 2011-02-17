/*
* An OpenGL template/framework file for the Computer Graphics course
* at the University of Groningen.
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

GLfloat cubeVertices[8*3] = {-1,-1,-1, -1,-1, 1, -1, 1,-1,  1,-1,-1, -1, 1, 1,  1,-1, 1,  1, 1,-1,  1, 1, 1};
GLubyte cubeIndices[2*12] = {
        0,1, 0,2, 0,3,                /* From three minusses to two minusses */
        1,4, 1,5, 2,4, 2,6, 3,5, 3,6, /* From two minusses to one minus */
        4,7, 5,7, 6,7                 /* From one minus to zero minusses */
    };

 

void display(void)
{
    /* Clear all pixels */
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0f,0.0f,1.0f);
    glLoadIdentity();
    gluLookAt(0.0,0.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, cubeVertices);

	// draw a cube
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_BYTE, cubeIndices);

	// deactivate vertex arrays after drawing
	glDisableClientState(GL_VERTEX_ARRAY);


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

void reshape(int w, int h)
{
    glViewport(0,0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(GLdouble)w/(GLdouble)h,1.5,20.0);
    glMatrixMode(GL_MODELVIEW);
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


    glutMainLoop();

    return 0;
}
