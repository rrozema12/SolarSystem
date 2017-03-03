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

#include "sphere.h"

using namespace std;

// Fill the vertex array with co-ordinates of the sample points.
void fillSphereVertexArray(Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)])
{
   int i, j, k;

   k = 0;
   for (j = 0; j <= SPHERE_LATS; j++)
      for (i = 0; i <= SPHERE_LONGS; i++)
      {
         sphereVertices[k].coords.x = cos(-2 + 3 * (float)j/SPHERE_LATS * PI/2 ) * cos( 2.0 * (float)i/SPHERE_LONGS * PI );
         sphereVertices[k].coords.y = sin(-2 + 3 * (float)j/SPHERE_LATS * PI/2 );
         sphereVertices[k].coords.z = cos(-2 + 3 * (float)j/SPHERE_LATS * PI/2 ) * sin( 2.0 * (float)i/SPHERE_LONGS * PI );
		   sphereVertices[k].coords.w = 1.0;
		   sphereVertices[k].normal.x = cos( (-1 + 2 * (float)i / SPHERE_LONGS) * PI );
         sphereVertices[k].normal.y = sin( (-1 + 2 * (float)i / SPHERE_LONGS) * PI );
		   sphereVertices[k].normal.z = 0.0;
		 k++;
	  }
}

// Fill the array of index arrays.
void fillSphereIndices(unsigned int sphereIndices[SPHERE_LATS][2*(SPHERE_LONGS+1)])
{
   int i, j;
   for(j = 0; j < SPHERE_LATS; j++)
   {
      for (i = 0; i <= SPHERE_LONGS; i++)
      {
	     sphereIndices[j][2*i] = (j + 1) * (SPHERE_LONGS + 1) + i;
	     sphereIndices[j][2*i+1] = j * (SPHERE_LONGS + 1) + i;
      }
   }
}

// Fill the array of counts.
void fillSphereCounts(int sphereCounts[SPHERE_LATS])
{
   int j;
   for(j = 0; j < SPHERE_LATS; j++) sphereCounts[j] = 2 * (SPHERE_LONGS + 1);
}

// Fill the array of buffer offsets.
void fillSphereOffsets(void* sphereOffsets[SPHERE_LATS])
{
   int j;
   for(j = 0; j < SPHERE_LATS; j++) sphereOffsets[j] = (GLvoid*)(2 * (SPHERE_LONGS+1) * j * sizeof(unsigned int));
}

// Initialize the Sphere.
void fillSphere(Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)],
	         unsigned int sphereIndices[SPHERE_LATS][2 * (SPHERE_LONGS + 1)],
			 int sphereCounts[SPHERE_LATS],
			 void* sphereOffsets[SPHERE_LATS])
{
   fillSphereVertexArray(sphereVertices);
   fillSphereIndices(sphereIndices);
   fillSphereCounts(sphereCounts);
   fillSphereOffsets(sphereOffsets);
}
