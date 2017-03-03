#ifndef SHIP_H
#define SHIP_H

#include "vertex.h"

#define PI 3.14159265
#define SHIP_OUTRAD 2.0 // Ship outer radius.
#define SHIP_INRAD 0.75 // Ship inner radius.
#define SHIP_LONGS 20 // Ship of longitudinal slices.
#define SHIP_LATS 20 // Ship of latitudinal slices.
#define SHIP_COLORS 1.0, 0.0, 0.0, 1.0 // Ship colors.

void fillShipVertexArray(Vertex shipVertices[(SHIP_LONGS + 1) * (SHIP_LATS + 1)]);
void fillShipIndices(unsigned int shipIndices[SHIP_LATS][2*(SHIP_LONGS+1)]);
void fillShipCounts(int shipCounts[SHIP_LATS]);
void fillShipOffsets(void* shipOffsets[SHIP_LATS]);

void fillShip(Vertex shipVertices[(SHIP_LONGS + 1) * (SHIP_LATS + 1)],
	         unsigned int shipIndices[SHIP_LATS][2*(SHIP_LONGS+1)],
			 int shipCounts[SHIP_LATS],
			 void* shipOffsets[SHIP_LATS]);
#endif
