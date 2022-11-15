// 3D Graphics and Animation - Debugger Template
// Does error checking for OpenGL.

#pragma once


#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;


class Debugger{
    public:
        Debugger() = default;
        ~Debugger() = default;

        static void APIENTRY OpenGLDebugCallback(GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar *message,
                                  const GLvoid *userParam);

        static void GlGetError(GLenum errorCode, const char *file, int line);
};

//void checkErrorShader(GLuint shader);
inline void errorCallbackGLFW(int error, const char *description){cout << "Error GLFW: " << description << "\n";};
void debugGL();

void APIENTRY openGLDebugCallback(GLenum source,
                                  GLenum type,
                                  GLuint id,
                                  GLenum severity,
                                  GLsizei length,
                                  const GLchar *message,
                                  const GLvoid *userParam);
GLenum glCheckError_(const char *file, int line);
#define glCheckError() glCheckError_(__FILE__, __LINE__)