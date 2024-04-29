import {Vertex} from './vertex.js';
import * as vec4 from './gl-matrix/vec4.js';

export const MAX_NO_VERTICES = 500, // Define to be the max number of v(ertex) lines in any OBJ file.
             MAX_NO_FACES = 1000, // Define to be the max number of f(ace) lines in any OBJ file.
             MAX_NO_FACE_VERTICES = 5; // Define to be the max number of vertices in any f line.

const SIZEOF_UINT16 = Uint16Array.BYTES_PER_ELEMENT; 

// Determine the index of the first non-space character of line searching rightward
// starting from the character at index start. 
function indexOfNonSpace(line, start=0) 
{
    let i = start;
    while (line[i] == ' ') i++;
    if (i == line.length) return -1;
    else return i;
 }

// Routine to read and process a Wavefront OBJ file. 
// Only vertex and face lines are read. All other lines are ignored. Only the x, y and z
// coordinate values are read from vertex lines. The (optional) w value is ignored if present. 
// Within a face line only vertex indices are read. Texture and normal indices are allowed but 
// ignored.
//
// The file is processed and the output written in order for the object to be displayed with
// a loop of gl.drawElements(primitive, countIndices, type, offset) calls.
//
// OUTPUT: Vertex coords values (x/y/z/w) are written, one vertex after another, as a 
// Vertex object each into the vertices array; a subarray of vertex indices, one per face, 
// is written into the array indices. The number of indices per face is written into 
// the countIndices array. The actual number of faces is written into countFaces. Offsets 
// into the indices array are written into offsets. The coordinates of the center of the 
// object's bounding box are written into boxCenterX, boxCenterY, boxCenterZ.
export async function loadOBJ(fileName,
	vertices,
	indices,
	countIndices,
	countFaces,
	offsets,
	boxCenter)
{
   // Fetch and read the source OBJ file as text.
   const fileResponse = await fetch(fileName);
   const fileSource = await fileResponse.text();
      
   // Temp variables.
   let line,
       lineStart = 0, lineEnd = 0,
       vertexNo = 0,
       indexArrayNo = 0;
       
   // Initialize the min/max vertex x/y/z values and the number of faces.
   let boxMin = new Float32Array([Number.MAX_VALUE, Number.MAX_VALUE, Number.MAX_VALUE]),
       boxMax = new Float32Array([-Number.MAX_VALUE, -Number.MIN_VALUE, -Number.MAX_VALUE]);
       countFaces[0] = 0;

   while (lineEnd != -1) // Empty line = eof has been reached.
   {
      // On successive passes of the outside while loop the following block
      // reads successive lines of the input file into the variable line.
      lineEnd = fileSource.indexOf('\n', lineStart);
      if (lineEnd == -1) line = fileSource.substring(lineStart);
      else line = fileSource.substring(lineStart, lineEnd);
      lineStart = lineEnd + 1;

      // Two if-cases in the following, one to read vertex lines and one to read face lines.
		// All other lines are ignored.
      if (line.substr(0, 2) == 'v ') // Vertex line.
      {
         let start = 1,
             vertexVec = vec4.create(); // Create an empty 4-vector to hold vertex coordinate values.
         
         // Successively read x/y/z values. Also update min/max x/y/z values. 
			// The (optional) w value is not read from the file but set to 1 instead. 
         for (let i = 0; i < 3; i++) 
         {
            // Read a coordinate value.
            vertexVec[i] = parseFloat(line.substr(start)); 

            if (vertexVec[i] < boxMin[i]) boxMin[i] = vertexVec[i]; if (vertexVec[i] > boxMax[i]) boxMax[i] = vertexVec[i];

            // First non-space after the current space = start of current coordinate value.
            start = indexOfNonSpace(line, start); 

            // First space after current coordinate value = index to call parseFloat() to 
            // read next coordinate value.
            start = line.indexOf(' ', start); 
         }
         vertexVec[3] = 1.0;

         
         // Add a new Vertex with value the vector of coordinate values to the array vertices.
         vertices[vertexNo] = new Vertex(vertexVec);
         
         vertexNo++;
      }

      if (line.substr(0, 2) == 'f ') // Face line.
      {
         let indexNo = 0,
             start = 1;
         
         // Initialize countIndices to 0 for the face. Note that indexArrayNo is same as 
         // the index of the current face, which in turn is the same as the index of the
         // current (sub)array of the array indices. E.g., the first MAX_NO_FACE_VERTICES
         // integers in indices form the 0th subarray and are the v-indices of 0th face;
         // the next MAX_NO_FACE_VERTICES integers form the 1th subarray and are the 
         // v-indices of 1th face; and so on.
         countIndices[indexArrayNo] = 0;

         // Within face lines only vertex indices are read, texture and normal 
			// indices are ignored even if present.
         while (start != -1) 
         {
            // Read v-index integer value in line into appropriate position in indices
            // and increment indexNo and countIndices.
            indices[MAX_NO_FACE_VERTICES * indexArrayNo + indexNo] = parseInt(line.substr(start)) - 1;
            indexNo++;
            countIndices[indexArrayNo]++;

            // First non-space after the current space = start of current vertex's number sequence.
            start = indexOfNonSpace(line, start); 

            // First space after current vertex's number sequence = index to call parseInt() 
            // to read the v-index of the next vertex's number sequence.
            start = line.indexOf(' ', start);
         }
         indexArrayNo++; // Next face.
         countFaces[0]++; // Increment face count.
      }
   }

   // Determine box center.
   for (let i = 0; i < 3; i++) boxCenter[i] = (boxMax[i] + boxMin[i]) / 2.0;
   
   // Fill up the rest of the vertices array to its max size MAX_NO_VERTICES with 
   // empty Vertex objects.
   for (let i = vertexNo; i < MAX_NO_VERTICES; i++) vertices[i] = new Vertex(vec4.create());
   
   // Set offset values.
   for (let j = 0; j < MAX_NO_FACES; j++) offsets[j] = MAX_NO_FACE_VERTICES * j * SIZEOF_UINT16;
}