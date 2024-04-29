#include <iostream>
#include <sstream>    
#include <string> 
#include <fstream> 
#include <cmath>

#include "loadOBJ.h"

// Routine to read and process a Wavefront OBJ file. 
// Only vertex and face lines are read. All other lines are ignored. Only the x, y and z
// coordinate values are read from vertex lines. The (optional) w value is ignored if present. 
// Within a face line only vertex indices are read. Texture and normal indices are allowed but 
// ignored.
//
// The file is processed and the output written in order for the object to be displayed with
// one glMultiDrawElements(primitive, *countIndices, type, **offsets, countPrimitives) call.
//
// OUTPUT: Vertex struct values (x/y/z/w) are written, one vertex after another, into the 
// vertices array; an array of vertex indices, one per face, is written into the 2d array 
// indices. The number of indices per face is written into the countIndices array. The 
// actual number of faces is written into countFaces. Offsets into the indices array are
// written into offsets. The coordinates of the center of the object's bounding box are 
// written into boxCenterX, boxCenterY, boxCenterZ.
void loadOBJ(std::string fileName,
	Vertex vertices[MAX_NO_VERTICES],
	unsigned int indices[MAX_NO_FACES][MAX_NO_FACE_VERTICES],
	int countIndices[MAX_NO_FACES],
	int &countFaces,
	void* offsets[MAX_NO_FACES],
	float boxCenter[3])
{
	// Temp variables.
	std::string line;
	int vertexNo = 0, indexArrayNo = 0;
	char currentCharacter, previousCharacter;
	float coordVal;

	// Initialize the min/max vertex x/y/z values and the number of faces.
	// float minX = HUGE_VALF, maxX = -HUGE_VALF, minY = HUGE_VALF, maxY = -HUGE_VALF, minZ = HUGE_VALF, maxZ = -HUGE_VALF;
	float boxMin[3] = { HUGE_VALF, HUGE_VALF, HUGE_VALF };
	float boxMax[3] = { -HUGE_VALF, -HUGE_VALF, -HUGE_VALF };
	countFaces = 0;

	std::ifstream inFile(fileName.c_str(), std::ifstream::in); // Open the OBJ file.

	while (getline(inFile, line)) // Read successive lines.
	{
		// Two if-cases in the following, one to read vertex lines and one to read face lines.
		// All other lines are ignored.
		if (line.substr(0, 2) == "v ") // Vertex line.
		{
			// Initialize a string from the character after "v " to the end.
			std::istringstream currentString(line.substr(2));

			// Successively read x/y/z values. Also update min/max x/y/z values. 
			// The (optional) w value is not read from the file but set to 1 instead. 
			for (int i = 0; i < 3; i++)
			{
				currentString >> coordVal;
				vertices[vertexNo].coords[i] = coordVal;
				if (coordVal < boxMin[i])  boxMin[i] = coordVal; if (coordVal >  boxMax[i])  boxMax[i] = coordVal;
			}
			vertices[vertexNo].coords[3] = 1.0;

			vertexNo++;
		}

		if (line.substr(0, 2) == "f ") // Face line.
		{
			// Initialize a string from the character after "f " to the end.
			std::istringstream currentString(line.substr(2));

			previousCharacter = ' ';
			int indexNo = 0, indexVal = 0;
			countIndices[indexArrayNo] = 0;

			// Within face lines only vertex indices are read, texture and normal 
			// indices are ignored even if present.
			// Logic to detect a vertex index: they and only they follow a space.
			while (currentString.get(currentCharacter))
			{
				if ((previousCharacter == ' ') && (currentCharacter != ' '))
					// Non-space character has followed a space implying it starts a vertex index.
				{
					// Move the string cursor back to just before the vertex index,
					// read the index value, decrement it by 1 as OBJ vertex indices 
					// start from 1, increment index number and keep count of the indices
					// read so far.
					currentString.unget();
					currentString >> indexVal;
					indices[indexArrayNo][indexNo] = --indexVal;
					indexNo++;
					countIndices[indexArrayNo]++;

					// Move to the character just after the vertex index just read
					// and repeat process.
					currentString.get(previousCharacter);
				}

				// Current character is not the start of a vertex index. Move ahead one character.
				else previousCharacter = currentCharacter;
			}

			// Update counts.
			indexArrayNo++;
			countFaces++;
		}
	}

	// Determine box center.
	for (int i = 0; i < 3; i++) boxCenter[i] = (boxMax[i] + boxMin[i]) / 2.0;

	// Set offset values.
	for (int j = 0; j < MAX_NO_FACES; j++) 
		offsets[j] = (void*)(MAX_NO_FACE_VERTICES * j * sizeof(unsigned int));

	inFile.close(); // Close the OBJ file.
}
