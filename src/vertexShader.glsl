#version 430 core

#define PLANET 0
#define SUN 1
#define MERCURY 2
#define VENUS 3
#define EARTH 4
#define MARS 5
#define JUPITER 6
#define SATURN 7
#define URANUS 8
#define NEPTUNE 9
#define PLUTO 10
#define TORUS 11
#define SHIP 12

layout(location=0) in vec4 sphereCoords; // Sphere Coordinates
layout(location=1) in vec4 torCoords; // Torus Coordinates
layout(location=2) in vec4 shipCoords;// Ship Coordinates

uniform mat4 projMat;
uniform mat4 modelViewMat;
uniform uint object;

vec4 coords;

void main(void)
{
   if (object == PLANET) coords = sphereCoords;
   if (object == SUN) coords = sphereCoords;
   if (object == MERCURY) coords = sphereCoords;
   if (object == VENUS) coords = sphereCoords;
   if (object == EARTH) coords = sphereCoords;
   if (object == MARS) coords = sphereCoords;
   if (object == JUPITER) coords = sphereCoords;
   if (object == SATURN) coords = sphereCoords;
   if (object == URANUS) coords = sphereCoords;
   if (object == NEPTUNE) coords = sphereCoords;
   if (object == PLUTO) coords = sphereCoords;
   if (object == TORUS) coords = torCoords;
   if (object == SHIP) coords = shipCoords;

   gl_Position = projMat * modelViewMat * coords;


}
