#define _USE_MATH_DEFINES 

#include <cmath>
#include <iostream>

#include <GL/glew.h>
#include <GL/freeglut.h> 

#include "sphere.h"

using namespace glm;

// Fill the vertex array with co-ordinates of the sample points.
void fillSphVertexArray(Vertex sphVertices[(SPH_LONGS + 1) * (SPH_LATS + 1)])
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= SPH_LATS; j++)
      for (i = 0; i <= SPH_LONGS; i++)
      {
         sphVertices[k].coords.x = SPH_RADIUS * cos( -M_PI/2 + (float)j/SPH_LATS * M_PI ) * cos( 2.0 * (float)i/SPH_LONGS * M_PI );
         sphVertices[k].coords.y = SPH_RADIUS * sin( -M_PI/2 + (float)j/SPH_LATS * M_PI );
         sphVertices[k].coords.z = SPH_RADIUS * cos( -M_PI/2 + (float)j/SPH_LATS * M_PI ) * sin( 2.0 * (float)i/SPH_LONGS * M_PI );
		 sphVertices[k].coords.w = 1.0;
		 k++;
	  }
}

// Fill the array of index arrays.
void fillSphIndices(unsigned int sphIndices[SPH_LATS][2*(SPH_LONGS+1)])
{
   int i, j;
   for(j = 0; j < SPH_LATS; j++)
   {
      for (i = 0; i <= SPH_LONGS; i++)
      {
	     sphIndices[j][2*i] = (j+1)*(SPH_LONGS + 1) + i;
	     sphIndices[j][2*i+1] = j*(SPH_LONGS + 1) + i;
      }
   }
}

// Fill the array of counts.
void fillSphCounts(int sphCounts[SPH_LATS])
{
   int j;
   for(j = 0; j < SPH_LATS; j++) sphCounts[j] = 2*(SPH_LONGS + 1);
}

// Fill the array of buffer offsets.
void fillSphOffsets(void* sphOffsets[SPH_LATS])
{
   int j;
   for(j = 0; j < SPH_LATS; j++) sphOffsets[j] = (GLvoid*)(2*(SPH_LONGS+1)*j*sizeof(unsigned int));
}

// Fill the sphere color vector.
void fillSphColors(vec4 &sphColors)
{
	sphColors = vec4(SPH_COLORS);
}

// Initialize the sphere.
void fillSphere(Vertex sphVertices[(SPH_LONGS + 1) * (SPH_LATS + 1)], 
	         unsigned int sphIndices[SPH_LATS][2*(SPH_LONGS+1)],
			 int sphCounts[SPH_LATS],
			 void* sphOffsets[SPH_LATS],
	         vec4 &sphColors)
{
   fillSphVertexArray(sphVertices);
   fillSphIndices(sphIndices);
   fillSphCounts(sphCounts);
   fillSphOffsets(sphOffsets);
   fillSphColors(sphColors);
}
