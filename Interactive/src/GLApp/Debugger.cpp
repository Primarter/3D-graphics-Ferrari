#include "Debugger.hpp"


namespace GLApp
{

    void APIENTRY Debugger::OpenGLDebugCallback(__attribute__((unused)) GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    __attribute__((unused)) GLsizei length,
                                    const GLchar *message,
                                    __attribute__((unused)) const GLvoid *userParam)
    {

        cout << "OpenGL: " << endl;
        cout << "Message: " << message << endl;
        cout << "type: ";
        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR: cout << "ERROR"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: cout << "DEPRECATED_BEHAVIOR"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: cout << "UNDEFINED_BEHAVIOR"; break;
            case GL_DEBUG_TYPE_PORTABILITY: cout << "PORTABILITY"; break;
            case GL_DEBUG_TYPE_PERFORMANCE: cout << "PERFORMANCE"; break;
            case GL_DEBUG_TYPE_OTHER: cout << "OTHER"; break;
        }
        cout << "id: " << id << " --- ";
        cout << "severity: ";
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_LOW: cout << "LOW"; break;
            case GL_DEBUG_SEVERITY_MEDIUM: cout << "MEDIUM"; break;
            case GL_DEBUG_SEVERITY_HIGH: cout << "HIGH"; break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: cout << "NOTIFICATION";
        }
        cout << endl;
    }

    void Debugger::GlGetError(GLenum errorCode, const char *file, int line)
    {
        auto error = "UNKNOWN"s;
        switch (errorCode)
            {
                case GL_INVALID_ENUM: error = "INVALID_ENUM"s; break;
                case GL_INVALID_VALUE: error = "INVALID_VALUE"s; break;
                case GL_INVALID_OPERATION: error = "INVALID_OPERATION"s; break;
                case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"s; break;
                case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"s; break;
                case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"s; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"s; break;
            }
        std::cout << "ERROR - OpenGL: " << error << " - source file: " << file << " (Code line: " << line << ")" << std::endl;
    }

    void APIENTRY openGLDebugCallback(GLenum source,
                                    GLenum type,
                                    GLuint id,
                                    GLenum severity,
                                    GLsizei length,
                                    const GLchar *message,
                                    const GLvoid *userParam)  // Debugger callback for Win64 - OpenGL versions 4.2 or better.
    {
        Debugger::OpenGLDebugCallback(source, type, id, severity, length, message, userParam);
    }

    void debugGL() // Debugger function for Win64 - OpenGL versions 4.2 or better.
    {
        // Enable Opengl Debug
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
    }

    GLenum glCheckError_(const char *file, int line) // Debugger manual function for Apple - OpenGL versions 3.3 to 4.1.
    {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR) Debugger::GlGetError(errorCode, file, line);

        return errorCode;
    }

}