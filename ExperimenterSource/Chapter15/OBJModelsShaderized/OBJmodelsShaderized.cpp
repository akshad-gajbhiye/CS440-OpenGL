///////////////////////////////////////////////////////////          
// OBJmodelsShaderized.cpp
//
// Forward-compatible core GL 4.3 version of OBJmodels.cpp.
//
// Sumanta Guha
/////////////////////////////////////////////////////////// 

#include <cmath>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "prepShader.h"
#include "loadOBJ.h"

using namespace glm;

static enum object { HIPPO, CAMEL }; // VAO ids.
static enum buffer { HIPPO_VERTICES, HIPPO_INDICES, CAMEL_VERTICES, CAMEL_INDICES }; // VBO ids.

// Hippo mesh data variables.
static Vertex hippoVertices[MAX_NO_VERTICES]; // Vertex x/y/z coordinates.
static unsigned int hippoIndices[MAX_NO_FACES][MAX_NO_FACE_VERTICES]; // Array of indices arrays.
static int hippoCountIndices[MAX_NO_FACES]; // Array of the number of indices per face.
static int hippoCountFaces; // Number of faces.
static void* hippoOffsets[MAX_NO_FACES];
static float hippoBoxCenter[3]; // Bounding box center coordinates.

// Camel mesh data variables.
static Vertex camelVertices[MAX_NO_VERTICES]; // Vertex x/y/z coordinates.
static unsigned int camelIndices[MAX_NO_FACES][MAX_NO_FACE_VERTICES]; // Array of indices arrays.
static int camelCountIndices[MAX_NO_FACES]; // Array of the number of indices per face.
static int camelCountFaces; // Number of faces.
static void* camelOffsets[MAX_NO_FACES];
static float camelBoxCenter[3]; // Bounding box center coordinates.

static vec4 blackColor = vec4(0.0, 0.0, 0.0, 1.0);
static mat4 modelViewMat, projMat;

static unsigned int
windowWidth,
windowHeight,
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
colorLoc,
buffer[4],
vao[2];

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	// Create shader program executable.
	vertexShaderId = setShader("vertex", "Shaders/vertexShader.glsl");
	fragmentShaderId = setShader("fragment", "Shaders/fragmentShader.glsl");
	programId = glCreateProgram();
	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);
	glUseProgram(programId);

	// Load hippo mesh data.
	loadOBJ("Meshes/hippo.obj",
		hippoVertices,
		hippoIndices,
		hippoCountIndices,
		hippoCountFaces,
		hippoOffsets,
		hippoBoxCenter);

	// Load camel mesh data.
	loadOBJ("Meshes/camel.obj",
		camelVertices,
		camelIndices,
		camelCountIndices,
		camelCountFaces,
		camelOffsets,
		camelBoxCenter);

	// Create VAOs and VBOs... 
	glGenVertexArrays(2, vao);
	glGenBuffers(4, buffer);

	// ...and associate hippo data with the vertex shader.
	glBindVertexArray(vao[HIPPO]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[HIPPO_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hippoVertices), hippoVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[HIPPO_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hippoIndices), hippoIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// ...and associate camel data with the vertex shader.
	glBindVertexArray(vao[CAMEL]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[CAMEL_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(camelVertices), camelVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[CAMEL_INDICES]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(camelIndices), camelIndices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	// Obtain color uniform location and set value.
	colorLoc = glGetUniformLocation(programId, "colors");
	glUniform4fv(colorLoc, 1, &blackColor[0]);

	// Obtain modelview matrix uniform and projection matrix uniform locations.
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	projMatLoc = glGetUniformLocation(programId, "projMat");

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the projection matrix.
	projMat = perspective(radians(60.0f), (float)windowWidth / windowHeight, 300.0f, 500.0f);
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	// Transformations to pose hippo.
	modelViewMat = lookAt(vec3(0.0, 0.0, 370.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(-100.0, -40.0, 0.0));
	modelViewMat = rotate(modelViewMat, radians(110.0f), vec3(0.0, 1.0, 0.0));
	modelViewMat = rotate(modelViewMat, radians(275.0f), vec3(1.0, 0.0, 0.0));

	// Center hippo.
	modelViewMat = translate(modelViewMat, vec3(-hippoBoxCenter[0], -hippoBoxCenter[1], -hippoBoxCenter[2])); 

	// Draw hippo.
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));
	glBindVertexArray(vao[HIPPO]);
	glMultiDrawElements(GL_TRIANGLE_FAN, hippoCountIndices, GL_UNSIGNED_INT, 
		(const void **)hippoOffsets, hippoCountFaces);

	// Transformations to pose camel.
	modelViewMat = lookAt(vec3(0.0, 0.0, 370.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
	modelViewMat = translate(modelViewMat, vec3(120.0, 0.0, 0.0));
	modelViewMat = rotate(modelViewMat, radians(185.0f), vec3(0.0, 1.0, 0.0));
	modelViewMat = rotate(modelViewMat, radians(265.0f), vec3(1.0, 0.0, 0.0));

	// Center camel.
	modelViewMat = translate(modelViewMat, vec3(-camelBoxCenter[0], -camelBoxCenter[1], -camelBoxCenter[2])); 

	// Draw camel.
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat)); 
	glBindVertexArray(vao[CAMEL]);
	glMultiDrawElements(GL_TRIANGLE_FAN, camelCountIndices, GL_UNSIGNED_INT, 
		(const void **)camelOffsets, camelCountFaces);

	glutSwapBuffers();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	windowWidth = w;
	windowHeight = h;
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
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OBJmodelsShaderized.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}

