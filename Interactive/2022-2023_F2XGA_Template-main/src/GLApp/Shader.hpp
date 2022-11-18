#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace GLApp {

class Shader
{
    public:
        int ID = -1;

        bool error = false;

        // Constructor generates the shader on the fly
        Shader(void) = default;
        Shader(const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath = nullptr);

        void init(const char* _vertexPath, const char* _fragmentPath, const char* _geometryPath = nullptr);

        void reloadShaders(void);

        // Activate the shader
        void use(void);

        // Set uniforms functions
        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec2(const std::string &name, float x, float y) const;

        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec3(const std::string &name, float x, float y, float z) const;

        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setVec4(const std::string &name, float x, float y, float z, float w);

        void setMat2(const std::string &name, const glm::mat2 &mat) const;

        void setMat3(const std::string &name, const glm::mat3 &mat) const;

        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        void checkCompileErrors(GLuint shader, std::string type);

        std::string vertexPath, fragmentPath, geometryPath;
};

}