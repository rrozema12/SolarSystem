#ifndef HEMISPHERE_H
#define HEMISPHERE_H

#include "vertex.h"

#define PI 3.14159265
#define SPHERE_RADIUS 2.0 // Sphere radius.
#define SPHERE_LONGS 30 // Number of longitudinal slices.
#define SPHERE_LATS 30 // Number of latitudinal slices.
#define SPHERE_COLORS 1.0, 1.0, 1.0, 1.0 // Sphere colors.
#define SUN_COLORS 1.0, 1.0, 0.0, 1.0 // Sun colors.
#define MERCURY_COLORS 1.0, 0.0, 0.0, 1.0 // Mercury colors.
#define VENUS_COLORS 0.961, 0.961, 0.863, 1.0 // Venus colors.
#define EARTH_COLORS 0.133, 0.545, 0.133, 1.0 // Earth colors.
#define MARS_COLORS 0.674, 0.165, 0.165, 1.0 // Mars colors.
#define JUPITER_COLORS 0.824, 0.412, 0.118, 1.0 // Jupiter colors.
#define SATURN_COLORS 0.824, 0.706, 0.549, 1.0 // Saturn colors.
#define URANUS_COLORS 0.529, 0.808, 0.980, 1.0 // Uranus colors.
#define NEPTUNE_COLORS 0.0, 0.0, 1.0, 1.0 // Neptune colors.
#define PLUTO_COLORS 0.412, 0.412, 0.412, 1.0 // Pluto colors.


void fillSphereVertexArray(Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)]);
void fillSphereIndices(unsigned int sphereIndices[SPHERE_LATS][2*(SPHERE_LONGS+1)]);
void fillSphereCounts(int sphereCounts[SPHERE_LATS]);
void fillSphereOffsets(void* sphereOffsets[SPHERE_LATS]);

void fillSphere(Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)],
	         unsigned int sphereIndices[SPHERE_LATS][2*(SPHERE_LONGS+1)],
			 int sphereCounts[SPHERE_LATS],
			 void* sphereOffsets[SPHERE_LATS]);

#endif
