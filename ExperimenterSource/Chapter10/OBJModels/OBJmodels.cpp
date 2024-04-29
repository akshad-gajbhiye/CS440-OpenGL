//////////////////////////////////////////////////////////////////////////////////
// OBJmodels.cpp
//
// External Wavefront OBJ files of two objects are read and processed with help of
// the routine loadOBJ(). After processing each object's mesh data is used to draw
// it with a glMultiDrawElements() call.
//
// Sumanta Guha.
//////////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#include "loadOBJ.h"

// Hippo mesh data variables.
static float hippoVertices[MAX_NO_VERTICES * 3]; // Vertex x/y/z coordinates.
static unsigned int **hippoIndices; // Array of indices arrays, sized in setup() below. 								      
static int hippoCountIndices[MAX_NO_FACES]; // Array of the number of indices per face.
static int hippoCountFaces; // Number of faces.
static float hippoBoxCenter[3]; // Bounding box center coordinates.

// Camel mesh data variables.
static float camelVertices[3 * MAX_NO_VERTICES]; // Vertex x/y/z coordinates.
static unsigned int **camelIndices; // Array of indices arrays, sized in setup() below. 								         
static int camelCountIndices[MAX_NO_FACES]; // Array of the number of indices per face.
static int camelCountFaces; // Number of faces.
static float camelBoxCenter[3]; // Bounding box center coordinates.

// Initialization routine.
void setup(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Size hippoIndices.
	hippoIndices = new unsigned int*[MAX_NO_FACES];
	for (int i = 0; i < MAX_NO_FACES; i++) hippoIndices[i] = new unsigned int[MAX_NO_FACE_VERTICES];

	// Load hippo mesh data.
	loadOBJ("Meshes/hippo.obj",
		hippoVertices,
		hippoIndices,
		hippoCountIndices,
		hippoCountFaces,
		hippoBoxCenter);

	// Size camelIndices.
	camelIndices = new unsigned int*[MAX_NO_FACES];
	for (int i = 0; i < MAX_NO_FACES; i++) camelIndices[i] = new unsigned int[MAX_NO_FACE_VERTICES];

	// Load camel mesh data.
	loadOBJ("Meshes/camel.obj",
		camelVertices,
		camelIndices,
		camelCountIndices,
		camelCountFaces,
		camelBoxCenter);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 370.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glPushMatrix();
	
	// Transformations to pose hippo.
	glTranslatef(-100.0, -40.0, 0.0);
	glRotatef(110.0, 0.0, 1.0, 0.0);
	glRotatef(275.0, 1.0, 0.0, 0.0);

	// Center hippo.
	glTranslatef(-hippoBoxCenter[0], -hippoBoxCenter[1], -hippoBoxCenter[2]); 

	// Draw hippo.
	glVertexPointer(3, GL_FLOAT, 0, hippoVertices);
	glMultiDrawElements(GL_TRIANGLE_FAN, hippoCountIndices, GL_UNSIGNED_INT,
		(const void **)hippoIndices, hippoCountFaces);
	
	glPopMatrix();

	glPushMatrix();
	
	// Transformations to pose camel.
	glTranslatef(120.0, 0.0, 0.0);
	glRotatef(185.0, 0.0, 1.0, 0.0);
	glRotatef(265.0, 1.0, 0.0, 0.0);

	// Center camel.
	glTranslatef(-camelBoxCenter[0], -camelBoxCenter[1], -camelBoxCenter[2]); 

	// Draw camel.
	glVertexPointer(3, GL_FLOAT, 0, camelVertices);
	glMultiDrawElements(GL_TRIANGLE_FAN, camelCountIndices, GL_UNSIGNED_INT,
		(const void **)camelIndices, camelCountFaces);
	
	glPopMatrix();

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 300.0, 500.0);
	glMatrixMode(GL_MODELVIEW);
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// Main routine.
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OBJmodels.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

