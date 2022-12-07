#pragma once

#define GL_SILENCE_DEPRECATION
#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.hpp"
#include "Shader.hpp"

namespace GLApp
{
    class Framebuffer
    {
        private:
            unsigned int quadVAO, quadVBO;
            GLuint textureColorbuffer, rbo;

        public:
            GLuint id;
            Framebuffer(void) = default;
            ~Framebuffer(void) = default;

            Shader screenShader;

            void setup(const char *vertShader, const char *fragShader, size_t windowWidth, size_t windowHeight);
            void draw(void);
    };
}