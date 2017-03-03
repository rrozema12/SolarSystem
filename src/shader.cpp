#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstring>

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

using namespace std;
#include "shader.h"

// Function to read text file.
char* readTextFile(char* aTextFile)
{
   FILE* filePointer = fopen(aTextFile, "rb");
   char* content = NULL;
   long numVal = 0;

   fseek(filePointer, 0L, SEEK_END);
   numVal = ftell(filePointer);
   fseek(filePointer, 0L, SEEK_SET);
   content = (char*) malloc((numVal+1) * sizeof(char));
   fread(content, 1, numVal, filePointer);
   content[numVal] = '\0';
   fclose(filePointer);
   return content;
}

// Function to initialize shaders.
int setShader(char* shaderType, char* shaderFile)
{
   int shaderId;
   char* shader = readTextFile(shaderFile);
   if ( shader == NULL ) {
	    std::cerr << "Failed to read shader" << endl;
	    exit( EXIT_FAILURE );
	}

   if (strcmp(shaderType, "vertex") == 0) shaderId = glCreateShader(GL_VERTEX_SHADER);
   if (strcmp(shaderType, "tessControl") == 0) shaderId = glCreateShader(GL_TESS_CONTROL_SHADER);
   if (strcmp(shaderType, "tessEvaluation") == 0) shaderId = glCreateShader(GL_TESS_EVALUATION_SHADER);
   if (strcmp(shaderType, "geometry") == 0) shaderId = glCreateShader(GL_GEOMETRY_SHADER);
   if (strcmp(shaderType, "fragment") == 0) shaderId = glCreateShader(GL_FRAGMENT_SHADER);

   glShaderSource(shaderId, 1, (const char**) &shader, NULL);
   glCompileShader(shaderId);
   GLint shaderCompiled;
   glGetShaderiv( shaderId, GL_COMPILE_STATUS, &shaderCompiled );
	if ( !shaderCompiled ) {
	    std::cerr << "  shader failed to compile:" << std::endl;
	    GLint  logSize;
	    glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &logSize );
	    char* logMsg = new char[logSize];
	    glGetShaderInfoLog( shaderId, logSize, NULL, logMsg );
	    std::cerr << logMsg << std::endl;
	    delete [] logMsg;

	    exit( EXIT_FAILURE );
	}

   return shaderId;
}
