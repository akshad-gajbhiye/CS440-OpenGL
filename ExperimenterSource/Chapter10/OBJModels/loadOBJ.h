#ifndef LOADOBJ_H
#define LOADOBJ_H

#include <string> 

#define MAX_NO_VERTICES 500 // Define to be the max number of v(ertex) lines in any OBJ file.
#define MAX_NO_FACES 1000 // Define to be the max number of f(ace) lines in any OBJ file.
#define MAX_NO_FACE_VERTICES 5 // Define to be the max number of vertices in any f line.

void loadOBJ(std::string fileName,
	float vertices[MAX_NO_VERTICES * 3],
	unsigned int **indices,
	int countIndices[MAX_NO_FACES],
	int &countFaces,
	float boxCenter[3]);
#endif