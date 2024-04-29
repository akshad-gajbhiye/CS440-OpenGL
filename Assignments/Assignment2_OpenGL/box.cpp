///////////////////////////////////////////////////////////          
// Assignment2_2203101_OpenGL.cpp
//
// This program draws a bird of flat triangles in 3D space,
// it also shows us how different locked views of bird and
// we can also move the camera using wasdqe keys.
//
// g++ Assignment2_2203101_OpenGL.cpp -o my.elf -std=c++11 -Wall -lGL -lGLU -lglut -lGLEW -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
//
// Name: 	Akshad Vivek Gajbhiye
// Roll No: 	2203101
/////////////////////////////////////////////////////////// 

#include <cstdlib>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

// Initialising menu_id for different views
int menu_id = 1;

// Initialising a, b and c for moving camera variables
float a=0.0, b=7.0, c=10.0;

// Initialising color boolean to toggle between color and line
int color=1;

// Function to draw forearm
void drawForeArm(void)
{
	if (color==0){glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
	else {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}

	// Setting the color of forearm
	glColor3f(0.4,0.4,0.4);
	
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(7.5, 3.0, 0.0);
	glVertex3f(5.0, 6.0, 1.0);
	glVertex3f(5.0, 4.0, 1.0);
	glVertex3f(5.0, 4.0, -1.0);
	glVertex3f(5.0, 6.0, -1.0);
	glVertex3f(5.0, 6.0, 1.0);
	glEnd();
	
}

// Function to draw backarm
void drawBackArm(void)
{
	if (color==0){glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);}
	else {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);}

	// Setting the color of backarm
	glColor3f(0.5,0.5,0.5);
	
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.0, 1.0, 1.0);
	glVertex3f(0.0, -1.0, 1.0);
	glVertex3f(5.0, 6.0, 1.0);
	glVertex3f(5.0, 4.0, 1.0);
	glVertex3f(5.0, 6.0, -1.0);
	glVertex3f(5.0, 4.0, -1.0);
	glVertex3f(0.0, 1.0, -1.0);
	glVertex3f(0.0, -1.0, -1.0);
	glVertex3f(0.0, 1.0, 1.0);
	glVertex3f(0.0, -1.0, 1.0);
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.0, 1.0, 1.0);
	glVertex3f(5.0, 6.0, 1.0);
	glVertex3f(0.0, 1.0, -1.0);
	glVertex3f(5.0, 6.0, -1.0);
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(0.0, -1.0, 1.0);
	glVertex3f(0.0, -1.0, -1.0);
	glVertex3f(5.0, 4.0, 1.0);
	glVertex3f(5.0, 4.0, -1.0);
	glEnd();
}

// Function to draw an arm
void drawArm(void)
{
	drawBackArm();
	drawForeArm();
}

// Function to draw the bird by creating 2 instances of arm
void drawBird(void)
{
	glPushMatrix();
	glRotatef(180, 0, 1, 0);
	drawArm();
	glPopMatrix();
	drawArm();
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//glColor3f(0.0, 0.0, 0.0);

	glLoadIdentity();

	if (menu_id==1)
	{
		// Perspective view at an angle from right
		gluLookAt(3.5, 10.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		//glutPostRedisplay();
	} 
	else if (menu_id==2)
	{
		// Perspective view at an angle from left
		gluLookAt(-3.5, 10.0, 9.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}
	else if (menu_id==3)
	{
		// Perspective viewing from side
		gluLookAt(10.0, 10.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	} 
	else if (menu_id==4)
	{
		// Perspective view from front
		gluLookAt(0.0, 7.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
		
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
	drawBird();
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glutPostRedisplay();

	glFlush();
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
		if (color == 0){
			color = 1;
		} else {
			color = 0;
		}
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

// The right button menu callback function.
void rightMenu(int id)
{
	if (id == 1)
	{
		menu_id = 1;
        	a=0.0; b=7.0; c=10.0;
	}
	else if (id == 2)
    	{
        	menu_id = 2;
        	a=0.0; b=7.0; c=10.0;
	}
	else if (id == 3)
	{
		menu_id = 3;
        	a=0.0; b=7.0; c=10.0;
	}
	else if (id == 4)
	{
		menu_id = 4;
        	a=0.0; b=7.0; c=10.0;
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

	makeMenu(); // Create menu.
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

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Assignment2_2203101_OpenGL.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}