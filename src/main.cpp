/**
Sander Susee and Ryan Rozema
12/9/2016
SuseeRozemaFinalProject

This program displays a large central sphere, representing the sun.
Around the sun, there are 8 planets, Pluto, and the asteroid belt.
Each planet has the characteristics of the planets in real life, such
as colors, spots, rings, distance from the sun, orbit period, etc.
In our solar system, we also have a Gonzaga themed space ship (red and
blue) which is controlled by Thayne.  He is moving the ship in a pattern
different from the planets (not elliptical) and is moving at random speeds.

Interaction:
  Press space to toggle between animation on and off.
  Press 0 to display facts about the Sun
  Press 1 to display facts about Mercury
  Press 2 to display facts about Venus
  Press 3 to display facts about Earth
  Press 4 to display facts about Mars
  Press 5 to display facts about Jupiter
  Press 6 to display facts about Saturn
  Press 7 to display facts about Uranus
  Press 8 to display facts about Neptune
  Press 9 to display facts about Pluto
  Press a to display facts about the Asteroid belt
  Press s to display facts about the space ship
  Press END to see what the pilot of the ship is saying
 **/

#include <cmath>
#include <iostream>
#include <fstream>

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "light.h"
#include "sphere.h"
#include "shader.h"
#include "material.h"
#include "torus.h"
#include "ship.h"

using namespace std;
using namespace glm;

/** Declarations of the VAOs and the VBOs **/
enum object {SPHERE, SUN_SPHERE, MERCURY_SPHERE, VENUS_SPHERE, EARTH_SPHERE,
             MARS_SPHERE, JUPITER_SPHERE, SATURN_SPHERE, URANUS_SPHERE, NEPTUNE_SPHERE,
             PLUTO_SPHERE, TORUS, SHIP}; // VAO ids
enum buffer {SPHERE_VERTICES, SPHERE_INDICES, TOR_VERTICES, TOR_INDICES,
              SHIP_VERTICES, SHIP_INDICES}; // VBO ids.

/** Global variable declarations **/
static float latAngleMercury, latAngleVenus, latAngleEarth, latAngleMars, latAngleJupiter,
             latAngleSaturn, latAngleUranus, latAngleNeptune, latAnglePluto, latAngleShip = 0.0; // Latitudinal angles.

/** Global variables that control the ship and the apocalypse **/
static float blackHole = 1.0, randomShipMovement = 1.0, sunSizeIncrease = 1.0;

/** Global variables to control the initial orientation of the solar system **/
static float Xangle = 1.0, Yangle = 0.0, Zangle = 0.0; // Angles to rotate scene.

/** Global variables that control the animation of the objects **/
static int isAnimate = 0; // Animated?
static int animationPeriod = 25; // Time interval between frames.
bool isIncreasing = true;
bool isDecreasing = false;

/** Variables that contain all the data for the planets **/
static Vertex sphereVertices[(SPHERE_LONGS + 1) * (SPHERE_LATS + 1)];
static unsigned int sphereIndices[SPHERE_LATS][2*(SPHERE_LONGS+1)];
static int sphereCounts[SPHERE_LATS];
static void* sphereOffsets[SPHERE_LATS];
static vec4 sphereColors = vec4(SPHERE_COLORS);
static vec4 sunColors = vec4(SUN_COLORS);
static vec4 mercuryColors = vec4(MERCURY_COLORS);
static vec4 venusColors = vec4(VENUS_COLORS);
static vec4 earthColors = vec4(EARTH_COLORS);
static vec4 marsColors = vec4(MARS_COLORS);
static vec4 jupiterColors = vec4(JUPITER_COLORS);
static vec4 saturnColors = vec4(SATURN_COLORS);
static vec4 uranusColors = vec4(URANUS_COLORS);
static vec4 neptuneColors = vec4(NEPTUNE_COLORS);
static vec4 plutoColors = vec4(PLUTO_COLORS);

/** Variables that contain all the data for the toruses, e.g. rings **/
static Vertex torVertices[(TOR_LONGS + 1) * (TOR_LATS + 1)];
static unsigned int torIndices[TOR_LATS][2*(TOR_LONGS+1)];
static int torCounts[TOR_LATS];
static void* torOffsets[TOR_LATS];
static vec4 torColors = vec4(TOR_COLORS);

/** Variables that contain all the data for the ship **/
static Vertex shipVertices[(SHIP_LONGS + 1) * (SHIP_LATS + 1)];
static unsigned int shipIndices[SHIP_LATS][2*(SHIP_LONGS+1)];
static int shipCounts[SHIP_LATS];
static void* shipOffsets[SHIP_LATS];
static vec4 shipColors = vec4(SHIP_COLORS);

/** Variables where the light data is contained **/
static const Light light0 =
{
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    vec4(0.0, 50.0, 0.0, 1.0),
};

/** Global ambient. **/
static const vec4 globAmb = vec4(0.2, 0.2, 0.2, 1.0);

static const Material material0 =
{
	vec4(0.9, 0.0, 0.0, 1.0),
	vec4(0.9, 0.0, 0.0, 1.0),
	vec4(1.0, 1.0, 1.0, 1.0),
	vec4(0.0, 0.0, 0.0, 1.0),
	50.0
};

static mat4 modelViewMat = mat4(1.0);
static mat4 projMat = mat4(1.0);

/** More global variables **/
static unsigned int
   programId,
   vertexShaderId,
   fragmentShaderId,
   modelViewMatLoc,
   projMatLoc,
   objectLoc,
   sphereColorLoc,
   torColorLoc,
   sunColorLoc,
   mercuryColorLoc,
   venusColorLoc,
   earthColorLoc,
   marsColorLoc,
   jupiterColorLoc,
   saturnColorLoc,
   uranusColorLoc,
   neptuneColorLoc,
   plutoColorLoc,
   shipColorLoc,
   buffer[6],
   vao[13];

/** Initialization routine.**/
void setup(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);

   /// Create shader program executable.
   vertexShaderId = setShader("vertex", "vertexShader.glsl");
   fragmentShaderId = setShader("fragment", "fragmentShader.glsl");
   programId = glCreateProgram();
   glAttachShader(programId, vertexShaderId);
   glAttachShader(programId, fragmentShaderId);
   glLinkProgram(programId);
   glUseProgram(programId);

   /// Initialize hemishpere, torus, and ship
   fillSphere(sphereVertices, sphereIndices, sphereCounts, sphereOffsets);
   fillTorus(torVertices, torIndices, torCounts, torOffsets);
   fillShip(shipVertices, shipIndices, shipCounts, shipOffsets);

   /// Create VAOs and VBOs
   glGenVertexArrays(13, vao);
   glGenBuffers(6, buffer);

   /// associate data with vertex shader.
   glBindVertexArray(vao[SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Sun data with vertex shader.
   glBindVertexArray(vao[SUN_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Mercury data with vertex shader.
   glBindVertexArray(vao[MERCURY_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Venus data with vertex shader.
   glBindVertexArray(vao[VENUS_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Earth data with vertex shader.
   glBindVertexArray(vao[EARTH_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Mars data with vertex shader.
   glBindVertexArray(vao[MARS_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Jupiter data with vertex shader.
   glBindVertexArray(vao[JUPITER_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Saturn data with vertex shader.
   glBindVertexArray(vao[SATURN_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Uranus data with vertex shader.
   glBindVertexArray(vao[URANUS_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Neptune data with vertex shader.
   glBindVertexArray(vao[NEPTUNE_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Pluto data with vertex shader.
   glBindVertexArray(vao[PLUTO_SPHERE]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SPHERE_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SPHERE_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sphereIndices), sphereIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(sphereVertices[0]), 0);
   glEnableVertexAttribArray(0);

   /// associate Torus data with vertex shader.
   glBindVertexArray(vao[TORUS]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[TOR_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(torVertices), torVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[TOR_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(torIndices), torIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(torVertices[0]), 0);
   glEnableVertexAttribArray(1);

   /// associate Ship data with vertex shader.
   glBindVertexArray(vao[SHIP]);
   glBindBuffer(GL_ARRAY_BUFFER, buffer[SHIP_VERTICES]);
   glBufferData(GL_ARRAY_BUFFER, sizeof(shipVertices), shipVertices, GL_STATIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[SHIP_INDICES]);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(shipIndices), shipIndices, GL_STATIC_DRAW);
   glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(shipVertices[0]), 0);
   glEnableVertexAttribArray(2);

   /// Obtain projection matrix uniform location and set value.
   projMatLoc = glGetUniformLocation(programId,"projMat");
   projMat = frustum(-5.0, 5.0, -5.0, 5.0, 5.0, 200.0);
   glUniformMatrix4fv(projMatLoc, 1, GL_FALSE, value_ptr(projMat));

   /// Obtain color uniform locations and set values.
   sphereColorLoc = glGetUniformLocation(programId, "sphereColor");
   glUniform4fv(sphereColorLoc, 1, &sphereColors[0]);

   sunColorLoc = glGetUniformLocation(programId, "sunColor");
   glUniform4fv(sunColorLoc, 1, &sunColors[0]);

   mercuryColorLoc = glGetUniformLocation(programId, "mercuryColor");
   glUniform4fv(mercuryColorLoc, 1, &mercuryColors[0]);

   venusColorLoc = glGetUniformLocation(programId, "venusColor");
   glUniform4fv(venusColorLoc, 1, &venusColors[0]);

   earthColorLoc = glGetUniformLocation(programId, "earthColor");
   glUniform4fv(earthColorLoc, 1, &earthColors[0]);

   marsColorLoc = glGetUniformLocation(programId, "marsColor");
   glUniform4fv(marsColorLoc, 1, &marsColors[0]);

   marsColorLoc = glGetUniformLocation(programId, "jupiterColor");
   glUniform4fv(marsColorLoc, 1, &marsColors[0]);

   marsColorLoc = glGetUniformLocation(programId, "saturnColor");
   glUniform4fv(marsColorLoc, 1, &marsColors[0]);

   uranusColorLoc = glGetUniformLocation(programId, "uranusColor");
   glUniform4fv(uranusColorLoc, 1, &uranusColors[0]);

   neptuneColorLoc = glGetUniformLocation(programId, "neptuneColor");
   glUniform4fv(neptuneColorLoc, 1, &neptuneColors[0]);

   plutoColorLoc = glGetUniformLocation(programId, "plutoColor");
   glUniform4fv(plutoColorLoc, 1, &plutoColors[0]);

   torColorLoc = glGetUniformLocation(programId, "torColor");
   glUniform4fv(torColorLoc, 1, &torColors[0]);

   shipColorLoc = glGetUniformLocation(programId, "shipColor");
   glUniform4fv(shipColorLoc, 1, &shipColors[0]);

   /// Obtain modelview matrix uniform and object uniform locations.
   modelViewMatLoc = glGetUniformLocation(programId,"modelViewMat");
   objectLoc = glGetUniformLocation(programId, "object");

   /// Obtain light property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "light0.ambCols"), 1, &light0.ambCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.difCols"), 1, &light0.difCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.specCols"), 1, &light0.specCols[0]);
   glUniform4fv(glGetUniformLocation(programId, "light0.coords"), 1, &light0.coords[0]);

   /// Obtain global ambient uniform location and set value.
   glUniform4fv(glGetUniformLocation(programId, "globAmb"), 1, &globAmb[0]);

   /// Obtain front material property uniform locations and set values.
   glUniform4fv(glGetUniformLocation(programId, "material0.ambRefl"), 1, &material0.ambRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "material0.difRefl"), 1, &material0.difRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "material0.specRefl"), 1, &material0.specRefl[0]);
   glUniform4fv(glGetUniformLocation(programId, "material0.emitCols"), 1, &material0.emitCols[0]);
   glUniform1f(glGetUniformLocation(programId, "material0.shininess"), material0.shininess);

   /// Makes the spheres filled rather than wired.
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void drawSun()
{
   /// Calculate and update modelview matrix for the sun.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(sunSizeIncrease + 15.0, sunSizeIncrease + 15.0, sunSizeIncrease + 15.0));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw the Sun as a sphere
   glUniform1ui(objectLoc, SUN_SPHERE);
   glBindVertexArray(vao[SUN_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawMercury()
{
   /// Calculate and update modelview matrix for Mercury.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(0.6, 0.6, 0.6));


   float ellipse_x = blackHole * 30 * cos(latAngleMercury);
   float ellipse_z = blackHole * -32 * sin(latAngleMercury);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Mercury as a sphere
   glUniform1ui(objectLoc, MERCURY_SPHERE);
   glBindVertexArray(vao[MERCURY_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawVenus()
{
   /// Calculate and update modelview matrix for Venus.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(0.8, 0.8, 0.8));


   float ellipse_x = blackHole * 35 * cos(latAngleVenus);
   float ellipse_z = blackHole * -37 * sin(latAngleVenus);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Venus as a sphere
   glUniform1ui(objectLoc, VENUS_SPHERE);
   glBindVertexArray(vao[VENUS_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawEarth()
{
   /// Calculate and update modelview matrix for Earth.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(1.0, 1.0, 1.0));


   float ellipse_x = blackHole * 36 * cos(latAngleEarth);
   float ellipse_z = blackHole * -38 * sin(latAngleEarth);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Earth as a sphere
   glUniform1ui(objectLoc, EARTH_SPHERE);
   glBindVertexArray(vao[EARTH_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawMars()
{
   /// Calculate and update modelview matrix for Mars.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(0.9, 0.9, 0.9));


   float ellipse_x = blackHole * 46 * cos(latAngleMars);
   float ellipse_z = blackHole * -50 * sin(latAngleMars);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Mars as a sphere
   glUniform1ui(objectLoc, MARS_SPHERE);
   glBindVertexArray(vao[MARS_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawAsteroidBelt()
{
   /// Calculate and update modelview matrix for the Asteroid belt.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(-14.0, -5.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle + 8, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(blackHole * 7.0, blackHole * 7.0, blackHole * 7.0));


   float ellipse_x = 2;
   float ellipse_z = -2;
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Asteroid as a torus
   glUniform1ui(objectLoc, TORUS);
   glBindVertexArray(vao[TORUS]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, torCounts, GL_UNSIGNED_INT, (const void **)torOffsets, TOR_LATS);
   modelViewMat = mat4(1.0);
}

void drawJupiter()
{
   /// Calculate and update modelview matrix for Jupiter.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(4.0, 4.0, 4.0));


   float ellipse_x = blackHole * 15 * cos(latAngleJupiter);
   float ellipse_z = blackHole * -16 * sin(latAngleJupiter);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Jupiter as a sphere
   glUniform1ui(objectLoc, JUPITER_SPHERE);
   glBindVertexArray(vao[JUPITER_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);

   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -96.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(1.0, 1.0, 1.0));

   /// Draw spot on Jupiter
   float ellipse_x_spot = blackHole * 60 * cos(latAngleJupiter);
   float ellipse_z_spot = blackHole * -64 * sin(latAngleJupiter);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x_spot, 0.0, ellipse_z_spot));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   glUniform1ui(objectLoc, MERCURY_SPHERE);
   glBindVertexArray(vao[MERCURY_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawSaturn()
{
   /// Calculate and update modelview matrix for Saturn.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(2.6, 2.6, 2.6));


   float ellipse_x = blackHole * 27 * cos(latAngleSaturn);
   float ellipse_z = blackHole * -33 * sin(latAngleSaturn);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Saturn as a sphere
   glUniform1ui(objectLoc, SATURN_SPHERE);
   glBindVertexArray(vao[SATURN_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
}

void drawSaturnRing()
{
   /// Calculate and update modelview matrix for Saturn's ring.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(1.0, 1.0, 1.0));


   float ellipse_x = blackHole * 70.2 * cos(latAngleSaturn);
   float ellipse_z = blackHole * -85.8 * sin(latAngleSaturn);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Saturn's ring as a torus
   glUniform1ui(objectLoc, TORUS);
   glBindVertexArray(vao[TORUS]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, torCounts, GL_UNSIGNED_INT, (const void **)torOffsets, TOR_LATS);
}

void drawUranus()
{
   /// Calculate and update modelview matrix for Uranus.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(1.2, 1.2, 1.2));


   float ellipse_x = blackHole * 65 * cos(latAngleUranus);
   float ellipse_z = blackHole * -75 * sin(latAngleUranus);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Uranus as a sphere
   glUniform1ui(objectLoc, URANUS_SPHERE);
   glBindVertexArray(vao[URANUS_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawUranusRing()
{
   /// Calculate and update modelview matrix for Uranus's Ring.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(0.5, 0.5, 0.5));


   float ellipse_x = blackHole * 156 * cos(latAngleUranus);
   float ellipse_z = blackHole * -180 * sin(latAngleUranus);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Uranus ring as a torus
   glUniform1ui(objectLoc, TORUS);
   glBindVertexArray(vao[TORUS]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, torCounts, GL_UNSIGNED_INT, (const void **)torOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawNepture()
{
   /// Calculate and update modelview matrix for Neptune.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(1.2, 1.2, 1.2));


   float ellipse_x = blackHole * 69 * cos(latAngleNeptune);
   float ellipse_z = blackHole * -80 * sin(latAngleNeptune);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Neptune as a sphere
   glUniform1ui(objectLoc, NEPTUNE_SPHERE);
   glBindVertexArray(vao[NEPTUNE_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawPluto()
{
   /// Calculate and update modelview matrix for Pluto.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(0.4, 0.4, 0.4));


   float ellipse_x = blackHole * 225 * cos(latAnglePluto);
   float ellipse_z = blackHole * -305 * sin(latAnglePluto);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Pluto as a sphere
   glUniform1ui(objectLoc, PLUTO_SPHERE);
   glBindVertexArray(vao[PLUTO_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);
}

void drawSpaceShip()
{
   /// Calculate and update modelview matrix for the space ship.
   modelViewMat = mat4(1.0);
   modelViewMat = translate(modelViewMat, vec3(0.0, 0.0, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle + 1, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(2.25, 2.25, 2.25));


   float ellipse_x = randomShipMovement * blackHole * 6.6 * cos(latAngleMars);
   float ellipse_z = randomShipMovement * blackHole * -6.6 * sin(latAngleMars);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x, 0.0, ellipse_z));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Space ship body as a sphere
   glUniform1ui(objectLoc, NEPTUNE_SPHERE);
   glBindVertexArray(vao[NEPTUNE_SPHERE]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, sphereCounts, GL_UNSIGNED_INT, (const void **)sphereOffsets, SPHERE_LATS);
   modelViewMat = mat4(1.0);

   modelViewMat = translate(modelViewMat, vec3(0.0, -1.5, -100.0));
   modelViewMat = rotate(modelViewMat, Zangle, vec3(0.0, 0.0, 1.0));
   modelViewMat = rotate(modelViewMat, Yangle, vec3(0.0, 1.0, 0.0));
   modelViewMat = rotate(modelViewMat, Xangle + 1, vec3(1.0, 0.0, 0.0));
   modelViewMat = scale(modelViewMat, vec3(1.0, 1.0, 1.0));


   float ellipse_x_ring = randomShipMovement * blackHole * 15 * cos(latAngleMars);
   float ellipse_z_ring = randomShipMovement * blackHole * -15 * sin(latAngleMars);
   modelViewMat = translate(modelViewMat, vec3(ellipse_x_ring, 0.0, ellipse_z_ring));
   glUniformMatrix4fv(modelViewMatLoc, 1, GL_FALSE, value_ptr(modelViewMat));

   /// Draw Mercury as a torus
   glUniform1ui(objectLoc, SHIP);
   glBindVertexArray(vao[SHIP]);
   glMultiDrawElements(GL_TRIANGLE_STRIP, shipCounts, GL_UNSIGNED_INT, (const void **)shipOffsets, SHIP_LATS);
   modelViewMat = mat4(1.0);
}

/// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   drawSun();
   drawMercury();
   drawVenus();
   drawEarth();
   drawMars();
   drawAsteroidBelt();
   drawJupiter();
   drawSaturn();
   drawSaturnRing();
   drawUranus();
   drawUranusRing();
   drawNepture();
   drawPluto();
   drawSpaceShip();
   glutSwapBuffers();
}

/// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
}

/// Timer function to animate the solar system
void animate(int value)
{
   if (isAnimate)
   {
      latAngleMercury += 0.4;
	  if (latAngleMercury > 360.0) {latAngleMercury -= 360.0;}

      latAngleVenus += 0.16;
	  if (latAngleVenus > 360.0) {latAngleVenus -= 360.0; }

      latAngleEarth += 0.1;
	  if (latAngleEarth > 360.0) {latAngleEarth -= 360.0; }

      latAngleMars += 0.09;
	  if (latAngleMars > 360.0) {latAngleMars -= 360.0; }

	  latAngleJupiter += 0.008;
	  if (latAngleJupiter > 360.0) {latAngleJupiter -= 360.0; }

	  latAngleSaturn += 0.005;
	  if (latAngleSaturn > 360.0) {latAngleSaturn -= 360.0;}

	  latAngleUranus += 0.0021;
	  if (latAngleUranus > 360.0) {latAngleUranus -= 360.0;}

	  latAngleNeptune += 0.0018;
	  if (latAngleNeptune > 360.0) {latAngleNeptune -= 360.0;}

	  latAnglePluto += 0.001;
	  if (latAnglePluto > 360.0) {latAnglePluto -= 360.0;}

	  if (latAngleShip > 360.0) {latAngleShip -= 360.0;}

	  if (randomShipMovement > 3.0)
	  {
	      isIncreasing = false;
	      isDecreasing = true;
	  }
	  else if (randomShipMovement < 1.3)
      {
          isIncreasing = true;
	      isDecreasing = false;
      }

      if (isIncreasing)
      {
          randomShipMovement += .03;
      }

      if (isDecreasing)
      {
          randomShipMovement -= .03;
      }

      randomShipMovement -= .01;
	  glutPostRedisplay();
      glutTimerFunc(animationPeriod, animate, 1);
   }
}

/// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 27:
         exit(0);
         break;
	  case ' ':
         if (isAnimate) isAnimate = 0;
		 else
		 {
	        isAnimate = 1;
			animate(1);
		 }
		 break;
      // Print out the facts for the planets
      case '0':
        cout << endl << "Sun \n Diameter: 696,000 kilometers \n" << endl;
        break;

      case '1':
        cout << "Mercury \n Diameter: 4,879 km \n Distance from the Sun: 57,909,227 km \n Orbit Period: 88 Earth Days \n" << endl;
        //cout << "Current year count compared to earth: " << yearCountMercury << endl;
        break;

      case '2':
        cout << "Venus \n Diameter: 12,104 km \n Distance from the Sun: 108,209,475 km \n Orbit Period: 225 Earth Days \n" << endl;
        break;

      case '3':
        cout << "Earth \n Diameter: 12,742  km \n Distance from the Sun: 149,598,262 km \n Orbit Period: 365 Earth Days \n" << endl;
        break;

      case '4':
        cout << "Mars \n Diameter: 6,779 km \n Distance from the Sun: 227,943,824 km km \n Orbit Period: 1.9 Earth Years \n" << endl;
        break;

      case '5':
        cout << "Jupiter \n Diameter: 139,822 km \n Distance from the Sun: 778,340,821 km \n Orbit Period: 11.9 Earth Years \n" << endl;
        break;

      case '6':
        cout << "Saturn \n Diameter: 116,464 km \n Distance from the Sun: 1,426,666,422 km \n Orbit Period: 29.5 Earth Years \n" << endl;
        break;

      case '7':
        cout << "Uranus \n Diameter: 50,724 km \n Distance from the Sun: 2,870,658,186 km \n Orbit Period: 84 Earth Years \n" << endl;
        break;

      case '8':
        cout << "Neptune \n Diameter: 50,724 km \n Distance from the Sun: 4,498,396,441 km \n Orbit Period: 164.8 Earth Years \n" << endl;
        break;

      case '9':
        cout << "Pluto \n Diameter: 2,300 km \n Distance from the Sun: 5,906,376,272 km \n Orbit Period: 248 Earth Years \n" << endl;
        break;

      case 'a':
        cout << "Asteroid Belt \n Diameter: 329-478 million km \n Distance from the Sun: 5,906,376,272 km \n Orbit Period: 248 Earth Years \n" << endl;
        break;

      case 's':
        cout << "WEEEEEEE I'm in a space ship!!!\n" << endl;
        break;
    break;

   }
}

/// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_DOWN) animationPeriod += 10;
   if (key == GLUT_KEY_UP) if (animationPeriod > 1000) animationPeriod -= 10;

   if (key == GLUT_KEY_END)
   {
      if (sunSizeIncrease < 40.0) sunSizeIncrease += 0.05;

      if (blackHole > 0.0)
      {
          blackHole -= 0.0025;
      }
   }
   glutPostRedisplay();
}

/// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press space to toggle between animation on and off" << endl
             << "Press 0 to display facts about the Sun" << endl
             << "Press 1 to display facts about Mercury" << endl
             << "Press 2 to display facts about Venus" << endl
             << "Press 3 to display facts about Earth" << endl
             << "Press 4 to display facts about Mars" << endl
             << "Press 5 to display facts about Jupiter" << endl
             << "Press 6 to display facts about Saturn" << endl
             << "Press 7 to display facts about Uranus" << endl
             << "Press 8 to display facts about Neptune" << endl
             << "Press 9 to display facts about Pluto" << endl
             << "Press a to display facts about the Asteroid belt" << endl
             << "Press s to display what the pilot of the ship is saying" << endl
             << "Press END key to begin the apocalypse" << endl;
}

/// Main routine.
int main(int argc, char **argv)
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(3, 3);
   glutInitContextProfile(GLUT_CORE_PROFILE);
   glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(900, 900);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Planet");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);

   glewExperimental = GL_TRUE;
   glewInit();

   setup();

   glutMainLoop();
}

