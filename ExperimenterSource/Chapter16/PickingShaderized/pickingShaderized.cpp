/////////////////////////////////////////////////////////////////////////////////////////          
// pickingShaderized.cpp
//
// Forward-compatible core GL 4.3 adaptation of picking.cpp based on color-coding objects
// and reading a pixel's color from a framebuffer instead of using a pick matrix.
// 
// Interaction:
// Click the left mouse button to select a triangle.
//
// Sumanta Guha
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "prepShader.h"
#include "vertex.h"

using namespace glm;

const int INITIAL_WINDOW_WIDTH = 500, INITIAL_WINDOW_HEIGHT = 500;

static enum object { TRIANGLE1, TRIANGLE2, TRIANGLE3, NONE };
static enum buffer { TRIANGLE1_VERTICES, TRIANGLE2_VERTICES, TRIANGLE3_VERTICES };

static mat4
modelViewMat,
projMat;

static unsigned int
programId,
vertexShaderId,
fragmentShaderId,
modelViewMatLoc,
projMatLoc,
vao[3],
buffer[3],
texture,
framebuffer,
renderbuffer,
colorCodeLoc,
highlightColorLoc,
isSelectingLoc,
isHighlightLoc,
clickedObj,
windowWidth = INITIAL_WINDOW_WIDTH,
windowHeight = INITIAL_WINDOW_HEIGHT;

// Data for 3 triangles including vertex coordinate and color values, and color code.
static Vertex triangle1Vertices[] =  // Vertical triangle.
{
	{ vec4(-3.0, -4.0, -5.3, 1.0), vec4(1.0, 0.0, 0.0, 1.0) },
	{ vec4(-1.0, -4.0, -5.3, 1.0), vec4(0.0, 1.0, 0.0, 1.0) },
	{ vec4(-3.0, 5.0, -5.3, 1.0), vec4(0.0, 0.0, 1.0, 1.0) }
};
static vec4 triangle1ColorCode = vec4(1.0, 0.0, 0.0, 1.0);

static Vertex triangle2Vertices[] = // Angled triangle.
{
	{ vec4(4.0, 4.0, -5.2, 1.0), vec4(0.0, 0.0, 0.0, 1.0) },
	{ vec4(2.0, 4.0, -5.2, 1.0), vec4(1.0, 1.0, 1.0, 1.0) },
	{ vec4(-5.0, -4.0, -5.2, 1.0), vec4(0.0, 1.0, 1.0, 1.0) }
};
static vec4 triangle2ColorCode = vec4(0.0, 1.0, 0.0, 1.0);

static Vertex triangle3Vertices[] = // Horizontal triangle.
{
	{ vec4(-4.0, 1.0, -5.1, 1.0), vec4(0.0, 1.0, 1.0, 1.0) },
	{ vec4(-4.0, -1.0, -5.1, 1.0), vec4(1.0, 1.0, 0.0, 1.0) },
	{ vec4(5.0, -1.0, -5.1, 1.0), vec4(1.0, 0.0, 1.0, 1.0) }
};
static vec4 triangle3ColorCode = vec4(0.0, 0.0, 1.0, 1.0);

// Picked triangle highlight color.
static vec4 highlightColor = vec4(1.0, 1.0, 0.0, 1.0);

void fitSizedAttachmentsToFramebuffer(int width, int height);

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

	// Create VAOs and VBOs... 
	glGenVertexArrays(3, vao);
	glGenBuffers(3, buffer);

	// ...and associate TRIANGLE1 data with vertex shader.
	glBindVertexArray(vao[TRIANGLE1]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TRIANGLE1_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle1Vertices), triangle1Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colors));
	glEnableVertexAttribArray(1);

	// ...and associate TRIANGLE2 data with vertex shader.
	glBindVertexArray(vao[TRIANGLE2]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TRIANGLE2_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle2Vertices), triangle2Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colors));
	glEnableVertexAttribArray(1);

	// ...and associate TRIANGLE3 data with vertex shader.
	glBindVertexArray(vao[TRIANGLE3]);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[TRIANGLE3_VERTICES]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle3Vertices), triangle3Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, colors));
	glEnableVertexAttribArray(1);

	// Obtain modelview matrix uniform location and set value.
	modelViewMat = mat4(1.0);
	modelViewMatLoc = glGetUniformLocation(programId, "modelViewMat");
	glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

	// Obtain projection matrix uniform location and set value.
	projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
	projMatLoc = glGetUniformLocation(programId, "projMat");
	glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

	// Create texture, renderbuffer and framebuffer ids.
	glGenTextures(1, &texture);
	glGenRenderbuffers(1, &renderbuffer);
	glGenFramebuffers(1, &framebuffer);

	// Bind FBO framebuffer and attach to it a texture as color buffer and renderbuffer
	// as depth buffer - see definition of fitSizedAttachmentsToFramebuffer() - of current
	// window size, and then bind default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	fitSizedAttachmentsToFramebuffer(windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);

	// Obtain fragment shader colorCode, highlightColor, isSelecting and
	// isHighlight uniform locations.
	colorCodeLoc = glGetUniformLocation(programId, "colorCode");
	highlightColorLoc = glGetUniformLocation(programId, "highlightColor");
	isSelectingLoc = glGetUniformLocation(programId, "isSelecting");
	isHighlightLoc = glGetUniformLocation(programId, "isHighlight");

	// Set value of highlightColor uniform and initial values of  
	// isSelecting and isHighlight uniforms.
	glUniform4fv(highlightColorLoc, 1, &highlightColor[0]);
	glUniform1ui(isSelectingLoc, 0);
	glUniform1ui(isHighlightLoc, 0);

	// Initial object clicked none.
	clickedObj = NONE;
}

// Routine to attach sized texture as color buffer and renderbuffer as depth buffer
// to currently bound FBO.
void fitSizedAttachmentsToFramebuffer(int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Transmit TRIANGLE1's color code to the fragment shader's colorCode uniform,
	// set the uniform isHighlight if TRIANGLE1 has been picked so that TRIANGLE1
	// will be highlighted in this case, and draw TRIANGLE1.
	glUniform4fv(colorCodeLoc, 1, &triangle1ColorCode[0]);
	if (clickedObj == TRIANGLE1) glUniform1ui(isHighlightLoc, 1); else glUniform1ui(isHighlightLoc, 0);
	glBindVertexArray(vao[TRIANGLE1]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Transmit TRIANGLE2's color code to the fragment shader's colorCode uniform,
	// set the uniform isHighlight if TRIANGLE2 has been picked so that TRIANGLE2
	// will be highlighted in this case, and draw TRIANGLE2.
	glUniform4fv(colorCodeLoc, 1, &triangle2ColorCode[0]);
	if (clickedObj == TRIANGLE2) glUniform1ui(isHighlightLoc, 1); else glUniform1ui(isHighlightLoc, 0);
	glBindVertexArray(vao[TRIANGLE2]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Transmit TRIANGLE3's color code to the fragment shader's colorCode uniform,
	// set the uniform isHighlight if TRIANGLE3 has been picked so that TRIANGLE3
	// will be highlighted in this case, and draw TRIANGLE3.
	glUniform4fv(colorCodeLoc, 1, &triangle3ColorCode[0]);
	if (clickedObj == TRIANGLE3) glUniform1ui(isHighlightLoc, 1); else glUniform1ui(isHighlightLoc, 0);
	glBindVertexArray(vao[TRIANGLE3]);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

// Mouse callback routine.
void pickFunction(int button, int state, int mouseX, int mouseY)
{
	unsigned char pixel[4];

	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;

	// Set the fragment shader uniform isSelecting so that triangles are drawn
	// color coded in the FBO framebuffer.
	glUniform1ui(isSelectingLoc, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	drawScene();

	// Read the clicked pixel.
	glReadPixels(mouseX, windowHeight - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);

	// Set clickedObj by matching color of clicked pixel with object color codes.
	if (int(pixel[0]) == 255 && int(pixel[1]) == 0 && int(pixel[2]) == 0) clickedObj = TRIANGLE1;
	if (int(pixel[0]) == 0 && int(pixel[1]) == 255 && int(pixel[2]) == 0) clickedObj = TRIANGLE2;
	if (int(pixel[0]) == 0 && int(pixel[1]) == 0 && int(pixel[2]) == 255) clickedObj = TRIANGLE3;
	if (int(pixel[0]) == 255 && int(pixel[1]) == 255 && int(pixel[2]) == 255) clickedObj = NONE;

	// Reset the fragment shader uniform isSelecting, bind the default framebuffer and draw again.
	glUniform1ui(isSelectingLoc, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
	drawScene();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);

	// Resize FBO framebuffer attachments to current window size.
	windowWidth = w; windowHeight = h;
	fitSizedAttachmentsToFramebuffer(windowWidth, windowHeight);
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

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Click the left mouse button to select a triangle." << std::endl;
}

// Main routine.
int main(int argc, char* argv[])
{
	printInteraction();
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("pickingShaderized.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(pickFunction);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
