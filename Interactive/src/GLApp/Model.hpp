#pragma once

#define GL_SILENCE_DEPRECATION
#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include <tinygltf/tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Transform.hpp"
#include "Shader.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace GLApp
{

    class Model {
        public:
            Model() = default;
            ~Model() = default;

            std::map<GLuint, std::map<int, GLuint>> vaoAndEbos;
            tinygltf::Model model;
            Transform transform;

            void LoadGLTF(std::string filename);
            void Draw(const Shader &);

        private:
            // Loading
            void BindModel(tinygltf::Model &model);
            void BindModelNodes(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node);
            void BindMesh(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);

            // Drawing
            void DrawModelNodes(const std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node, glm::mat4 &parent);
            void DrawMesh(const std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);

            const Shader *cachedShader = nullptr;
    };

};