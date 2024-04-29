//////////////////////////////////////////////////////      
// sphereInBox1.cpp
//
// This program draws a green sphere inside a red box. 
// There is a single positional light source.
// 
// Interaction:
// Press up/down arrow keys to open/close the box.
//
// Sumanta Guha.
//////////////////////////////////////////////////////      

#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#define ONE_BY_ROOT_THREE 0.57735

// Begin globals.
static int step = 0; // Steps in open/closing the box lid.

// Initialising menu_id for different views
int menu_id = 1;

// Initialising a, b and c for moving camera variables
float a = 0.0, b = 2.5, c = 5.0;

// Initialising color boolean to toggle between color and line
int color = 0;

// Box vertex co-ordinate vectors. 
static float vertices[] =
{
	1.0, 0.0, 1.0,
	1.0, 1.0, 1.0,
	1.0, 1.0, 0.0,
	1.0, 0.0, 0.0,
	0.0, 0.0, 1.0,
	0.0, 1.0, 1.0,
	0.0, 1.0, 0.0,
	0.0, 0.0, 0.0
};

// Vertex indices of triangle strip going around the box, making the 
// front, right, back and left sides in that order (exactly similar
// to the triangle strip going around a square annulus in Ch. 3).
static unsigned char stripIndices0[] = { 5, 4, 1, 0, 2, 3, 6, 7, 5, 4 };
// { 5, 6, 4, 7, 0, 3, 1, 2 };

// Vertex indices of triangle strip making the bottom of the box.
static unsigned char stripIndices1[] = { 0, 4, 3, 7 };

// Vertex indices of triangle strip making the top of the box.
static unsigned char stripIndices2[] = { 6, 5, 2, 1 };

// Box vertex normal vectors = normalized unit vector pointing from origin to vertex.
static float normals[] =
{
	ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
	ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE,
	-ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE, -ONE_BY_ROOT_THREE
};
// End globals.

// The right button menu callback function.
void rightMenu(int id)
{
	if (id == 1)
	{
		menu_id = 1;
		a = 0.0; b = 2.5; c = 5.0;
	}
	else if (id == 2)
	{
		menu_id = 2;
		a = 0.0; b = 2.5; c = 5.0;
	}
	else if (id == 3)
	{
		menu_id = 3;
		a = 0.0; b = 2.5; c = 5.0;
	}
	else if (id == 4)
	{
		menu_id = 4;
		a = 0.0; b = 2.5; c = 5.0;
	}
	else if (id == 5)
	{
		menu_id = 5;
	}
	else if (id == 6)
	{
		exit(0);
	}
	glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
	glutCreateMenu(rightMenu);
	glutAddMenuEntry("Right Angled View", 1);
	glutAddMenuEntry("Left Angled View", 2);
	glutAddMenuEntry("Side View", 3);
	glutAddMenuEntry("Front View", 4);
	glutAddMenuEntry("Moving Camera", 5);
	glutAddMenuEntry("Quit", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST); // Enable depth testing.

	// Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);

	// Light property vectors.
	float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
	float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightPos[] = { 0.0, 1.5, 3.0, 1.0 };
	float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

	// Light properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE); // Enable two-sided lighting.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint.
	
	makeMenu(); // Create menu.
}

// Drawing routine.
void drawScene(void)
{
	// Material property vectors.
	float matAmbAndDif1[] = { 0.9, 0.0, 0.0, 1.0 };
	float matAmbAndDif2[] = { 0.0, 0.9, 0.0, 1.0 };
	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	float matShine[] = { 50.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	if (menu_id==1)
	{
		// Perspective view at an angle from right
		gluLookAt(1.75, 5.0, 4.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		//glutPostRedisplay();
	} 
	else if (menu_id==2)
	{
		// Perspective view at an angle from left
		gluLookAt(-1.75, 5.0, 4.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else if (menu_id==3)
	{
		// Perspective viewing from side
		gluLookAt(5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	} 
	else if (menu_id==4)
	{
		// Perspective view from front
		gluLookAt(0.0, 3.5, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		
	}
	else if (menu_id==5)
	{
		// Moving Camera
		gluLookAt(a, b, c, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	// BackFace Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// Implementation of Z-buffer algorithm
	glEnable(GL_DEPTH_TEST);

	// Material properties of the box.
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShine);

	// Enable two vertex arrays: position and normal.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	// Specify locations for the position and normal arrays.
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	// Five sides of the box, excluding the top. The normal at each 
	// vertex is along the line from the origin to that vertex.
	glDrawElements(GL_TRIANGLE_STRIP, 10, GL_UNSIGNED_BYTE, stripIndices0);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, stripIndices1);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, stripIndices2);

	// Material properties of the sphere (only the front is ever seen).
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbAndDif2);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glutPostRedisplay();

	glFlush();

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(100.0, (float)w / (float)h, 1.0, 50.0);
	glutPostRedisplay();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 97:
		// For "a"
		a -= 0.2;
		break;
	case 100:
		// For "d"
		a += 0.2;
		break;
	case 115:
		// For "s"
		b -= 0.2;
		break;
	case 119:
		// For "w"
		b += 0.2;
		break;
	case 113:
		// For "q"
		c -= 0.2;
		break;
	case 101:
		// For "e"
		c += 0.2;
		break;
	case 99:
		// For "c"
		if (color == 0) {
			color = 1;
		}
		else {
			color = 0;
		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Press c for enabling and disabling color." << std::endl;
	std::cout << "-------------------------------------" << std::endl;
	std::cout << "Right click for menu options." << std::endl
		<< "1. Right Angled View" << std::endl
		<< "2. Left Angled View" << std::endl
		<< "3. Side View" << std::endl
		<< "4. Front View" << std::endl
		<< "5. Moving Camera" << std::endl
		<< "6. Quit" << std::endl;
	std::cout << "--------------------------------------" << std::endl
		<< "Instructions for moving camera:" << std::endl
		<< "Decrement y axis: s" << std::endl
		<< "Increment y axis: w" << std::endl
		<< "Decrement x axis: a" << std::endl
		<< "Increment x axis: d" << std::endl
		<< "Decrement z axis: q" << std::endl
		<< "Increment z axis: e" << std::endl;
}


// Main routine.
int main(int argc, char **argv)
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("sphereInBox1.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
