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


uniform uint object;
uniform vec4 sphereColor; // Sphere Color
uniform vec4 sunColor; // Sun Color
uniform vec4 mercuryColor; // Mercury Color
uniform vec4 venusColor; // Venus Color
uniform vec4 earthColor; // Earth Color
uniform vec4 marsColor; // Mars Color
uniform vec4 jupiterColor; // Jupiter Color
uniform vec4 saturnColor; // Saturn Color
uniform vec4 uranusColor; // Uranus Color
uniform vec4 neptuneColor; // Neptune Color
uniform vec4 plutoColor; // Pluto Color
uniform vec4 torColor; // Torus Color
uniform vec4 shipColor; // Ship Color

out vec4 colorsOut;

void main(void)
{
   if (object == PLANET) colorsOut = sphereColor;
   if (object == SUN) colorsOut = sunColor;
   if (object == MERCURY) colorsOut = mercuryColor;
   if (object == VENUS) colorsOut = venusColor;
   if (object == EARTH) colorsOut = earthColor;
   if (object == MARS) colorsOut = marsColor;
   if (object == JUPITER) colorsOut = jupiterColor;
   if (object == SATURN) colorsOut = saturnColor;
   if (object == URANUS) colorsOut = uranusColor;
   if (object == NEPTUNE) colorsOut = neptuneColor;
   if (object == PLUTO) colorsOut = plutoColor;
   if (object == TORUS) colorsOut = torColor;
   if (object == SHIP) colorsOut = shipColor;
}

