//////////////////////////////////////////////////////////////////////////////////////
// circleDrawElements.cpp
//
// This program modifies circle.cpp to draw the circle with one glDrawElements() call.
//
// Interaction:
// Press +/- to increase/decrease the number of vertices of the loop. 
//
// Sumanta Guha.
////////////////////////////////////////////////////////////////////////////////////// 

#define _USE_MATH_DEFINES 

#include <cstdlib>
#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

// Globals.
static float R = 40.0; // Radius of circle.
static float X = 50.0; // X-coordinate of center of circle.
static float Y = 50.0; // Y-coordinate of center of circle.
static int numVertices = 5; // Number of vertices on circle.
static float *vertices = NULL; // Vertex array of sample points on the circle.
static float *colors = NULL; // Vertex array of colors at sample points on the circle.
static unsigned int *indices = NULL; // Array of indices.

// Fill the vertex array with co-ordinates of the sample points.
void fillVertexArray(void)
{
   float t = 0; // Angle parameter.
   int i, k;

   k = 0;
   for (i = 0; i < numVertices; i++)
   {
	  vertices[k++] = X + R * cos(t);
	  vertices[k++] = Y + R * sin(t);
	  vertices[k++] = 0.0;
	  t += 2 * M_PI / numVertices;
   }
}

// Fill the colors array with random colors.
void fillColorArray(void)
{
   int i, k;

   k = 0;
   for (i = 0; i < numVertices; i++)
   {
	  colors[k++] = (float)rand() / (float)RAND_MAX;
	  colors[k++] = (float)rand() / (float)RAND_MAX;
	  colors[k++] = (float)rand() / (float)RAND_MAX;
   }
}

void fillIndices(void)
{
   int i;

   for (i = 0; i < numVertices; i++) indices[i] = i;
}

// Drawing routine.
void drawScene(void)
{
   int i;

   glClear(GL_COLOR_BUFFER_BIT);

   // Create global arrays.
   vertices = new float[3 * numVertices];
   colors = new float[3 * numVertices];
   indices = new unsigned int[numVertices];

   // Initialize global arrays.
   fillVertexArray();
   fillColorArray();
   fillIndices();

   // Point to global arrays.
   glVertexPointer(3, GL_FLOAT, 0, vertices);
   glColorPointer(3, GL_FLOAT, 0, colors);

   // Draw the circle as a line loop.
   glDrawElements(GL_LINE_LOOP, numVertices, GL_UNSIGNED_INT, indices);

   glFlush();
}

// Initialization routine.
void setup(void)
{
   glClearColor(1.0, 1.0, 1.0, 0.0);

   // Enable two vertex arrays: co-ordinates and color.
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_COLOR_ARRAY);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 27:
	  exit(0);
	  break;
   case '+':
	  numVertices++;
	  glutPostRedisplay();
	  break;
   case '-':
	  if (numVertices > 3) numVertices--;
	  glutPostRedisplay();
	  break;
   default:
	  break;
   }
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   std::cout << "Interaction:" << std::endl;
   std::cout << "Press +/- to increase/decrease the number of vertices on the circle." << std::endl;
}

// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(4, 3);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("circleDrawElements.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}
