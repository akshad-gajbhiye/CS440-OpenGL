import {Vertex} from './vertex.js';
import * as vec4 from './gl-matrix/vec4.js';

export const TOR_OUTRAD = 12.0, // Torus outer radius.
             TOR_INRAD = 2.0, // Torus inner radius.
             TOR_LONGS = 20, // Number of longitudinal slices.
             TOR_LATS = 20, // Number of latitudinal slices.
             TOR_COLORS = new Float32Array([0.0, 1.0, 0.0, 1.0]); // Torus colors.

const SIZEOF_UINT16 = Uint16Array.BYTES_PER_ELEMENT; 

// Fill the vertex array with co-ordinates of the sample points.
function fillTorVertexArray(torVertices) 
{
   let i, j, k;

   k = 0;
   for (j = 0; j <= TOR_LATS; j++)
      for (i = 0; i <= TOR_LONGS; i++) 
      {
         torVertices[k] = new Vertex
         (
            vec4.fromValues
            (
               (TOR_OUTRAD + TOR_INRAD * Math.cos((-1.0 + 2.0*j/TOR_LATS) * Math.PI)) * Math.cos((-1.0 + 2.0*i/TOR_LONGS) * Math.PI),
               (TOR_OUTRAD + TOR_INRAD * Math.cos((-1.0 + 2.0*j/TOR_LATS) * Math.PI)) * Math.sin((-1.0 + 2.0*i/TOR_LONGS) * Math.PI),
               TOR_INRAD * Math.sin((-1.0 + 2.0*j/TOR_LATS) * Math.PI),
               1.0
            )
         );
         k++;
      }
}

// Fill the array of index arrays.
 function fillTorIndices(torIndices) 
 {
    let i, j, k;
    k = 0;
    for (j = 0; j < TOR_LATS; j++) 
       for (i = 0; i <= TOR_LONGS; i++) 
       {
          torIndices[k++] = (j + 1) * (TOR_LONGS + 1) + i;
          torIndices[k++] = j * (TOR_LONGS + 1) + i;
       }
 }
 
 // Fill the array of counts.
 function fillTorCounts(torCounts) 
 {
    let j;
    for (j = 0; j < TOR_LATS; j++) torCounts[j] = 2 * (TOR_LONGS+1);
 }
 
 // Fill the array of buffer offsets.
 function fillTorOffsets(torOffsets) 
 {
    let j;
    for (j = 0; j < TOR_LATS; j++) torOffsets[j] = 2 * (TOR_LONGS+1) * j * SIZEOF_UINT16;
 }

 // Fill the torus color array.
 function fillTorColors(torColors)
 {
    let i;
    for (i = 0; i < 4; i++) torColors[i] = TOR_COLORS[i];
 }
 
 // Initialize the torus.
 export function fillTorus(torVertices, torIndices, torCounts, torOffsets, torColors)
 {
    fillTorVertexArray(torVertices);
    fillTorIndices(torIndices);
    fillTorCounts(torCounts);
    fillTorOffsets(torOffsets);
    fillTorColors(torColors);
 }