import {Vertex} from './vertex.js';
import * as vec4 from './gl-matrix/vec4.js';
import * as vec3 from './gl-matrix/vec3.js';

export const CYL_LONGS = 30, // Number of longitudinal slices.
             CYL_LATS = 10; // Number of latitudinal slices.

const SIZEOF_UINT16 = Uint16Array.BYTES_PER_ELEMENT; 

// Fill the vertex array with co-ordinates of the sample points.
function fillCylVertexArray(cylVertices) 
{
   let i, j, k;

   k = 0;
   for (j = 0; j <= CYL_LATS; j++)
      for (i = 0; i <= CYL_LONGS; i++) 
      {
         cylVertices[k] = new Vertex
         (
            vec4.fromValues
            (
               Math.cos( (-1 + 2.0*i/CYL_LONGS) * Math.PI ),
               Math.sin( (-1 + 2.0*i/CYL_LONGS) * Math.PI ),
               -1 + 2.0*j/CYL_LATS,
               1.0
            ),
            vec3.fromValues
            (
               Math.cos( (-1 + 2.0*i/CYL_LONGS) * Math.PI ),
               Math.sin( (-1 + 2.0*i/CYL_LONGS) * Math.PI ),
               0.0
            )
         );
         k++;
      }
}

// Fill the array of index arrays.
function fillCylIndices(cylIndices) 
{
   let i, j, k;
   k = 0;
   for (j = 0; j < CYL_LATS; j++) 
      for (i = 0; i <= CYL_LONGS; i++) 
      {
         cylIndices[k++] = (j + 1) * (CYL_LONGS + 1) + i;
         cylIndices[k++] = j * (CYL_LONGS + 1) + i;
      }
}

// Fill the array of counts.
function fillCylCounts(cylCounts) 
{
   let j;
   for (j = 0; j < CYL_LATS; j++) cylCounts[j] = 2 * (CYL_LONGS + 1);
}

// Fill the array of buffer offsets.
function fillCylOffsets(cylOffsets) 
{
   let j;
   for (j = 0; j < CYL_LATS; j++) cylOffsets[j] = 2 * (CYL_LONGS + 1) * j * SIZEOF_UINT16;
}

// Initialize the cylinder.
export function fillCylinder(cylVertices, cylIndices, cylCounts, cylOffsets)
{
   fillCylVertexArray(cylVertices);
   fillCylIndices(cylIndices);
   fillCylCounts(cylCounts);
   fillCylOffsets(cylOffsets);
}
