///////////////////////////////////////////////////////////////////////////////////
// Sutheland-Cohen_2203101.cpp
//
// This program allows the user to draw simple shapes on a canvas.
//
// Interaction:
// Left click on a box on the left to select a primitive.
// Then left click on the drawing area: once for point, twice for line or rectangle.
// Right click for menu options. (Grid, Clear, Clip, Quit)
//
// Akshad Vivek Gajbhiye.
////////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <vector>
#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define INACTIVE 0
#define POINT 1
#define LINE 2
#define RECTANGLE 3
#define NUMBERPRIMITIVES 3

// Globals.
static int width, height; // OpenGL window size.
static float pointSize = 3.0; // Size of point
static int primitive = INACTIVE; // Current drawing primitive.
static int pointCount = 0; // Number of  specified points.
static int tempX, tempY; // Co-ordinates of clicked point.
static int isGrid = 1; // Is there grid?
float x_max, x_min, y_max, y_min; // Points to store clipping window coordinates
int point_count = 0; // For storing number of points in "points" array
int line_count = 0; // For storing number of lines in "lines" array
int rect_count = 0; // For keeping the count of clipping window. We will stop this at 1

// Point class.
class Point
{
public:
	Point(float xVal, float yVal, float sizeVal)
	{
		x = xVal; y = yVal; size = sizeVal;
	}
	Point() {};
	void drawPoint(void); // Function to draw a point.
	float x, y; // x and y co-ordinates of point.
private:
	//float x, y; // x and y co-ordinates of point.
	float size; // Size of point.
};

//float Point::size = pointSize; // Set point size.

// Function to draw a point.
void Point::drawPoint()
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(0.0,0.0,1.0);
	glVertex3f(x, y, 0.0);
	glEnd();
}

// Vector of points.
std::vector<Point> points;

// Iterator to traverse a Point array.
std::vector<Point>::iterator pointsIterator;

// Function to draw all points in the points array.
void drawPoints(void)
{
	// Loop through the points array drawing each point.
	for (auto point : points) { point.drawPoint(); }
}

// Line class.
class Line
{
public:
	Line(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Line() {};
	void drawLine();
	float x1, y1, x2, y2;
private:
    //float x1, y1, x2, y2; // x and y co-ordinates of endpoints.
};


// Function to draw a line.
void Line::drawLine()
{
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// Vector of lines.
std::vector<Line> lines;

// Function to draw all lines in the lines array.
void drawLines(void)
{
	// Loop through the lines array drawing each line.
	for (auto line : lines) { line.drawLine(); }
}

// Rectangle class.
class Rect
{
public:
	Rect(float x1Val, float y1Val, float x2Val, float y2Val)
	{
		x1 = x1Val; y1 = y1Val; x2 = x2Val; y2 = y2Val;
	}
	Rect() {};
	void drawRectangle();
private:
    float x1, y1, x2, y2; // x and y co-ordinates of diagonally opposite vertices.
};

// Function to draw a rectangle.
void Rect::drawRectangle()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0,0.0,0.0);
	glRectf(x1, y1, x2, y2);
}

// Vector of rectangles.
std::vector<Rect> rectangles;

// Function to draw all rectangles in the rectangles array.
void drawRectangles(void)
{
	// Loop through the rectangles array drawing each rectangle.
	for (auto rectangle : rectangles) { rectangle.drawRectangle(); }
}

// Function to draw point selection box in left selection area.
void drawPointSelectionBox(void)
{
	if (primitive == POINT) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.9*height, 0.1*width, height);

	// Draw point icon.
	glPointSize(pointSize);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	glVertex3f(0.05*width, 0.95*height, 0.0);
	glEnd();
}

// Function to draw line selection box in left selection area.
void drawLineSelectionBox(void)
{
	if (primitive == LINE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.8*height, 0.1*width, 0.9*height);

	// Draw line icon.
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.025*width, 0.825*height, 0.0);
	glVertex3f(0.075*width, 0.875*height, 0.0);
	glEnd();
}

// Function to draw rectangle selection box in left selection area.
void drawRectangleSelectionBox(void)
{
	if (primitive == RECTANGLE) glColor3f(1.0, 1.0, 1.0); // Highlight.
	else glColor3f(0.8, 0.8, 0.8); // No highlight.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw black boundary.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.7*height, 0.1*width, 0.8*height);

	// Draw rectangle icon.
	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.025*width, 0.735*height, 0.075*width, 0.765*height);
	glEnd();
}

// Function to draw unused part of left selection area.
void drawInactiveArea(void)
{
	glColor3f(0.6, 0.6, 0.6);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);

	glColor3f(0.0, 0.0, 0.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glRectf(0.0, 0.0, 0.1*width, (1 - NUMBERPRIMITIVES*0.1)*height);
}

// Function to draw temporary point.
void drawTempPoint(void)
{
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	glVertex3f(tempX, tempY, 0.0);
	glEnd();
}

// Function to draw a grid.
void drawGrid(void)
{
	int i;

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, 0x5555);
	glColor3f(0.75, 0.75, 0.75);

	glBegin(GL_LINES);
	for (i = 2; i <= 9; i++)
	{
		glVertex3f(i*0.1*width, 0.0, 0.0);
		glVertex3f(i*0.1*width, height, 0.0);
	}
	for (i = 1; i <= 9; i++)
	{
		glVertex3f(0.1*width, i*0.1*height, 0.0);
		glVertex3f(width, i*0.1*height, 0.0);
	}
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

// Drawing routine.
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	drawPointSelectionBox();
	drawLineSelectionBox();
	drawRectangleSelectionBox();
	drawInactiveArea();

	drawPoints();
	drawLines();
	drawRectangles();
	if (((primitive == LINE) || (primitive == RECTANGLE)) &&
		(pointCount == 1)) drawTempPoint();
	if (isGrid) drawGrid();

	glutSwapBuffers();
}

// Function to pick primitive if click is in left selection area.
void pickPrimitive(int y)
{
	if (y < (1 - NUMBERPRIMITIVES*0.1)*height) primitive = INACTIVE;
	else if (y < (1 - 2 * 0.1)*height) primitive = RECTANGLE;
	else if (y < (1 - 1 * 0.1)*height) primitive = LINE;
	else primitive = POINT;
}

void ClipCoords(float x1, float y1, float x2, float y2)
{
    if (x1>x2)
    {
        x_max = x1;
        x_min = x2;
    }
    else
    {
        x_max = x2;
        x_min = x1;
    }
    if (y1>y2)
    {
        y_max = y1;
        y_min = y2;
    }
    else
    {
        y_max = y2;
        y_min = y1;
    }
}

// The mouse callback routine.
void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		y = height - y; // Correct from mouse to OpenGL co-ordinates.

		if (x < 0 || x > width || y < 0 || y > height); // Click outside canvas - do nothing.

		// Click in left selection area.
		else if (x < 0.1*width)
		{
			pickPrimitive(y);
			pointCount = 0;
		}

		// Click in canvas.
		else
		{
			if (primitive == POINT)
            {
                    points.push_back(Point(x, y, pointSize));
                    point_count++;
			}
			else if (primitive == LINE)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					lines.push_back(Line(tempX, tempY, x, y));
					line_count++;
					pointCount = 0;
				}
			}
			else if (primitive == RECTANGLE && rect_count==0)
			{
				if (pointCount == 0)
				{
					tempX = x; tempY = y;
					pointCount++;
				}
				else
				{
					rectangles.push_back(Rect(tempX, tempY, x, y));
					ClipCoords(tempX, tempY, x, y);
					rect_count++;
					pointCount = 0;
				}
			}
		}
	}
	glutPostRedisplay();
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set viewing box dimensions equal to window dimensions.
	glOrtho(0.0, (float)w, 0.0, (float)h, -1.0, 1.0);

	// Pass the size of the OpenGL window to globals.
	width = w;
	height = h;

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
	default:
		break;
	}
}

// Clear the canvas and reset for fresh drawing.
void clearAll(void)
{
	points.clear();
	lines.clear();
	rectangles.clear();
	primitive = INACTIVE;
	pointCount = 0;
    point_count = 0;
    line_count = 0;
    rect_count = 0;
}

const int TOP=8;
const int BOTTOM=4;
const int RIGHT=2;
const int LEFT=1;

// Function to get code of a point in binary
int PointCode(float x, float y)
{
    int code = 0;
    if (y>y_max)
    {
        code |= TOP;
    }
    else if (y<y_min)
    {
        code |= BOTTOM;
    }
    if (x>x_max)
    {
        code |= RIGHT;
    }
    else if (x<x_min)
    {
        code |= LEFT;
    }
    return code;
}

//Function of clip point outside the clipping window with the help of the point code
void ClipPoint(void)
{
    int i=0;
    int temp = point_count;
    while (point_count>0){
        //if (CheckPoint(points[i].x, points[i].y) == false)
        if(PointCode(points[i].x, points[i].y) != 0)
        {
            points.erase(points.begin()+i);
            temp--;
        }
        else
        {
            i++;
        }
        point_count--;
    }
    point_count = temp;
}


//Function of clip line using the Sutherland-Cohen Algorithm
void ClipLine(void)
{
    int i=0;
    int temp = line_count;
    int pointcode_1, pointcode_2, pointcode_final;
    bool accept, done;
    while (line_count>0)
    {
        accept = false;
        done = false;
        int x1 = lines[i].x1;
        int y1 = lines[i].y1;
        int x2 = lines[i].x2;
        int y2 = lines[i].y2;
        pointcode_1 = PointCode(x1, y1);
        pointcode_2 = PointCode(x2, y2);
        do
        {
            if ((pointcode_1 | pointcode_2) == false)
            {
                accept=true;
                done=true;
            }
            else if ((pointcode_1 & pointcode_2) != false)
            {
                done=true;
                accept=false;
            }
            else
            {
                double x,y;
                pointcode_final = pointcode_1?pointcode_1:pointcode_2;
                if(pointcode_final & TOP)
                {
                    x = x1 + (x2-x1)*(y_max-y1)/(y2-y1);
                    y = y_max;
                }
                else if(pointcode_final & BOTTOM)
                {
                    x = x1 + (x2-x1)*(y_min-y1)/(y2-y1);
                    y = y_min;
                }
                else if(pointcode_final & RIGHT)
                {
                    y = y1 + (y2-y1)*(x_max-x1)/(x2-x1);
                    x = x_max;
                }
                else
                {
                    y = y1 + (y2-y1)*(x_min-x1)/(x2-x1);
                    x = x_min;
                }
                if (pointcode_final==pointcode_1)
                {
                    x1 = x;
                    y1 = y;
                    lines[i].x1 = x1;
                    lines[i].y1 = y1;
                    pointcode_1 = PointCode(x1, y1);
                }
                else
                {
                    x2 = x;
                    y2 = y;
                    lines[i].x2 = x2;
                    lines[i].y2 = y2;
                    pointcode_2 = PointCode(x2,y2);
                }
            }
        } while(!done);
        if (accept == true)
        {
            i++;
        }
        else
        {
            lines.erase(lines.begin()+i);
            temp--;
        }
        line_count--;
    }
    line_count=temp;
}

// The right button menu callback function.
void rightMenu(int id)
{
	if (id == 1)
	{
		clearAll();
		glutPostRedisplay();
	}
	if (id == 2)
    {
        ClipPoint();
        ClipLine();
        glutPostRedisplay();
	}
	if (id == 3) exit(0);
}

// The sub-menu callback function.
void grid_menu(int id)
{
	if (id == 3) isGrid = 1;
	if (id == 4) isGrid = 0;
	glutPostRedisplay();
}

// Function to create menu.
void makeMenu(void)
{
	int sub_menu;
	sub_menu = glutCreateMenu(grid_menu);
	glutAddMenuEntry("On", 3);
	glutAddMenuEntry("Off", 4);

	glutCreateMenu(rightMenu);
	glutAddSubMenu("Grid", sub_menu);
	glutAddMenuEntry("Clear", 1);
	glutAddMenuEntry("Clip", 2);
	glutAddMenuEntry("Quit", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Initialization routine.
void setup(void)
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	makeMenu(); // Create menu.
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
	std::cout << "Interaction:" << std::endl;
	std::cout << "Left click on a box on the left to select a primitive." << std::endl
		<< "Then left click on the drawing area: once for point, twice for line or rectangle." << std::endl
		<< "Right click for menu options." << std::endl
		<< "1. Grid" << std::endl
		<< "2. Clear" << std::endl
		<< "3. Clip" << std::endl
		<< "4. Quit" << std::endl;
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
	glutCreateWindow("canvas.cpp");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mouseControl);

	glewExperimental = GL_TRUE;
	glewInit();

	setup();

	glutMainLoop();
}
