# OpenGL Similaton of our Solar System

## Description of the Goal and how to Execute
The goal of the project is to create a program in OpenGL that will accuratly animate our Solar System.

### Requirements - Download, Install and Run:
1. CodeBlocks (any version 13 or higher)
2. OpenGL plugin for CodeBlocks
3. The math library for OpenGL


## Details about Files
There are many files in this project.  See the description of these files given below.

1. **main.cpp**   
    > This is the main driver for the program.  It starts the program and does most of the work   

2. **sphere.cpp & sphere.h**   
    > When these files work together they generate a sphere in OpenGL.  These spheres are used for a couple different objects in the Solar System, such as each planet and the body of the alien space ship.

3. **torus.cpp & torus.h**   
    > When these files work together they generate a torus (or a ring) in OpenGL.  These toruses are used for the asteriod belt,  the rings for Saturn and Uranus, and the other part of the body of the space ship.

4. **light.h, shader.cpp, shader.h, material.cpp, & material.h**  
    > All of these files working together will simlate light emitting from the Sun.  There will be specular light on the sun that will make it look shiny (look for the white spot).

5. **ship.cpp & ship.h**  
    > The "Ship" is a new OpenGL object that we created.  It is just a sphere sitting inside a torus, which makes it look like an alien space ship.

6. **fragmentShader.glsl & vertexShader.glsl**  
    > Since this entire project uses VBOs, or Vertex Buffer Objects, we needed fragment and vertex shaders to bind the colors to the color buffer.  This allowed us to make each of the planets differnet colors.  It helps the viewer distinguish between the planets easier.

## Interaction
```  
  Press space to toggle between animation on and off.
  Press 0 to display facts about the Sun.
  Press 1 to display facts about Mercury.
  Press 2 to display facts about Venus.
  Press 3 to display facts about Earth.
  Press 4 to display facts about Mars.
  Press 5 to display facts about Jupiter.
  Press 6 to display facts about Saturn.
  Press 7 to display facts about Uranus.
  Press 8 to display facts about Neptune.
  Press 9 to display facts about Pluto.
  Press a to display facts about the Asteroid belt.
  Press s to display what the pilot of the ship is saying.
  Press and hold the END key to sumulate the apocalypse.
```
