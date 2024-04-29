#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#include "helix.h"

// Fill the vertex array with co-ordinates of the sample points.
void fillHelVertexArray(Vertex helVertices[HEL_SEGS+1])
{
   float t;
   int k = 0;

   for(t = -1.0 * HEL_HALF_TURNS * M_PI; t <= HEL_HALF_TURNS * M_PI; t += (2 * HEL_HALF_TURNS * M_PI)/HEL_SEGS)
      {
         helVertices[k].coords.x = 2 * HEL_HALF_TURNS * cos(t);
         helVertices[k].coords.y = 2 * HEL_HALF_TURNS * sin(t);
         helVertices[k].coords.z = t;
		 helVertices[k].coords.w = 1.0;
		 k++;
	  }
}

// Initialize the helix.
void fillHelix(Vertex helVertices[HEL_SEGS+1])
{
   fillHelVertexArray(helVertices);
}
