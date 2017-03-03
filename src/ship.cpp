#include <cmath>
#include <iostream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib")
#endif

#include "ship.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillShipVertexArray(Vertex shipVertices[(SHIP_LONGS + 1) * (SHIP_LATS + 1)])
{
   int i, j, k;
   k = 0;
   for (j = 0; j <= SHIP_LATS; j++)
      for (i = 0; i <= SHIP_LONGS; i++)
      {
         shipVertices[k].coords.x = ( SHIP_OUTRAD + SHIP_INRAD * cos( (-1 + 2*(float)j/SHIP_LATS) * PI ) ) * cos( (-1 + 2*(float)i/SHIP_LONGS) * PI );
         shipVertices[k].coords.y = ( SHIP_OUTRAD + SHIP_INRAD * cos( (-1 + 2*(float)j/SHIP_LATS) * PI ) ) * sin( (-1 + 2*(float)i/SHIP_LONGS) * PI );
         shipVertices[k].coords.z = SHIP_INRAD * sin( (-1 + 2*(float)j/SHIP_LATS) * PI );
		 shipVertices[k].coords.w = 1.0;
		 k++;
	  }
}

// Fill the array of index arrays.
void fillShipIndices(unsigned int shipIndices[SHIP_LATS][2*(SHIP_LONGS+1)])
{
   int i, j;
   for(j = 0; j < SHIP_LATS; j++)
   {
      for (i = 0; i <= SHIP_LONGS; i++)
      {
	     shipIndices[j][2*i] = (j+1)*(SHIP_LONGS + 1) + i;
	     shipIndices[j][2*i+1] = j*(SHIP_LONGS + 1) + i;
      }
   }
}

// Fill the array of counts.
void fillShipCounts(int shipCounts[SHIP_LATS])
{
   int j;
   for(j = 0; j < SHIP_LATS; j++) shipCounts[j] = 2*(SHIP_LONGS + 1);
}

// Fill the array of buffer offsets.
void fillShipOffsets(void* shipOffsets[SHIP_LATS])
{
   int j;
   for(j = 0; j < SHIP_LATS; j++) shipOffsets[j] = (GLvoid*)(2*(SHIP_LONGS+1)*j*sizeof(unsigned int));
}

// Initialize the ship.
void fillShip(Vertex shipVertices[(SHIP_LONGS + 1) * (SHIP_LATS + 1)],
	         unsigned int shipIndices[SHIP_LATS][2*(SHIP_LONGS+1)],
			 int shipCounts[SHIP_LATS],
			 void* shipOffsets[SHIP_LATS])
{
   fillShipVertexArray(shipVertices);
   fillShipIndices(shipIndices);
   fillShipCounts(shipCounts);
   fillShipOffsets(shipOffsets);
}
