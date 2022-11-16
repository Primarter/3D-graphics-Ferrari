#pragma once

#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

#include <tinygltf/tiny_gltf.h>

#include <string>
#include <iostream>
#include <vector>
using namespace std;

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

namespace GLApp
{

    class Model {
        public:
            Model() = default;
            ~Model() = default;

            pair<GLuint, map<int, GLuint>> vaoAndEbos;
            tinygltf::Model model;

            void LoadGLTF(string filename);
            void Draw(const pair<GLuint, map<int, GLuint>> &vaoAndEbos, tinygltf::Model &model);

        private:
            // Loading
            pair<GLuint, map<int, GLuint>> BindModel(tinygltf::Model &model);
            void BindModelNodes(map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node);
            void BindMesh(map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);

            // Drawing
            void DrawModelNodes(const pair<GLuint, map<int, GLuint>> &vaoAndEbos, tinygltf::Model &model, tinygltf::Node &node);
            void DrawMesh(const map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh);
    };

};