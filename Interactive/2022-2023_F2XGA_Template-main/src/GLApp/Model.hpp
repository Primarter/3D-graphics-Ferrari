#pragma once

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

#include <tinygltf/tiny_gltf.h>

#include <string>
#include <iostream>
#include <vector>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace GLApp
{

    class Model {
        public:
            Model() = default;
            ~Model() = default;

            std::pair<GLuint, std::map<int, GLuint>> vaoAndEbos;
            tinygltf::Model model;

            void LoadGLTF(std::string filename);
            void Draw(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos, tinygltf::Model &model);

        private:
            // Loading
            std::pair<GLuint, std::map<int, GLuint>> BindModel(tinygltf::Model &model);
            void BindModelNodes(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node);
            void BindMesh(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);

            // Drawing
            void DrawModelNodes(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos, tinygltf::Model &model, tinygltf::Node &node);
            void DrawMesh(const std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);
    };

};