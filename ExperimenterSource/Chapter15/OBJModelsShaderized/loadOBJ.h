#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <string> 
#include "vertex.h"

#define MAX_NO_VERTICES 500 // Define to be the max number of v(ertex) lines in any OBJ file.
#define MAX_NO_FACES 1000 // Define to be the max number of f(ace) lines in any OBJ file.
#define MAX_NO_FACE_VERTICES 5 // Define to be the max number of vertices in any f line.

void loadOBJ(std::string fileName,
	Vertex vertices[MAX_NO_VERTICES],
	unsigned int indices[MAX_NO_FACES][MAX_NO_FACE_VERTICES],
	int countIndices[MAX_NO_FACES],
	int &countFaces,
	void* offsets[MAX_NO_FACES],
	float boxCenter[3]);
#endif