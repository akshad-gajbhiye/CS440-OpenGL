import {Vertex} from './vertex.js';
import * as vec4 from './gl-matrix/vec4.js';

export const SPH_RADIUS = 2.0, // Sphere radius.
             SPH_LONGS = 10, // Number of longitudinal slices.
             SPH_LATS = 5, // Number of latitudinal slices.
             SPH_COLORS = new Float32Array([0.0, 0.0, 1.0, 1.0]); // Sphere colors.

const SIZEOF_UINT16 = Uint16Array.BYTES_PER_ELEMENT; 

// Fill the vertex array with co-ordinates of the sample points.
function fillSphVertexArray(sphVertices) 
{
   let i, j, k;

   k = 0;
   for (j = 0; j <= SPH_LATS; j++)
      for (i = 0; i <= SPH_LONGS; i++) 
      {
         sphVertices[k] = new Vertex
         (
            vec4.fromValues
            (
               SPH_RADIUS * Math.cos(-Math.PI/2.0 + j/SPH_LATS * Math.PI) * Math.cos(2.0*i/SPH_LONGS * Math.PI),
               SPH_RADIUS * Math.sin(-Math.PI/2.0 + j/SPH_LATS * Math.PI),
               SPH_RADIUS * Math.cos(-Math.PI/2.0 + j/SPH_LATS * Math.PI) * Math.sin(2.0*i/SPH_LONGS * Math.PI),
               1.0
            )
         );
         k++;
      }
}

// Fill the array of index arrays.
function fillSphIndices(sphIndices) 
{
   let i, j, k;
   k = 0;
   for (j = 0; j < SPH_LATS; j++) 
      for (i = 0; i <= SPH_LONGS; i++) 
      {
         sphIndices[k++] = (j + 1) * (SPH_LONGS + 1) + i;
         sphIndices[k++] = j * (SPH_LONGS + 1) + i;
      }
}

// Fill the array of counts.
function fillSphCounts(sphCounts) 
{
   let j;
   for (j = 0; j < SPH_LATS; j++) sphCounts[j] = 2 * (SPH_LONGS + 1);
}

// Fill the array of buffer offsets.
function fillSphOffsets(sphOffsets) 
{
   let j;
   for (j = 0; j < SPH_LATS; j++) sphOffsets[j] = 2 * (SPH_LONGS + 1) * j * SIZEOF_UINT16;
}

// Fill the sphere color array.
function fillSphColors(sphColors)
{
   let i;
   for (i = 0; i < 4; i++) sphColors[i] = SPH_COLORS[i];
}

// Initialize the sphere.
export function fillSphere(sphVertices, sphIndices, sphCounts, sphOffsets, sphColors)
{
   fillSphVertexArray(sphVertices);
   fillSphIndices(sphIndices);
   fillSphCounts(sphCounts);
   fillSphOffsets(sphOffsets);
   fillSphColors(sphColors);
}
