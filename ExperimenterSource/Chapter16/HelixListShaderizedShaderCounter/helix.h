#ifndef HELIX_H
#define HELIX_H

#include "vertex.h"

#define HEL_HALF_TURNS 10 // Half the number of turns in the helix.
#define HEL_SEGS 400 // Number of line segments comprising the helix.

void fillHelix(Vertex helVertices[HEL_SEGS+1]);

#endif