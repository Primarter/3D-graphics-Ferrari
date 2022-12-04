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
    typedef GLuint MaterialId;

    struct Mesh
    {
        GLuint vao;
        std::map<int, GLuint> vbos;
        MaterialId material;
    };

    struct Texture
    {
        GLuint id;
        std::string name;
        std::string type;
        std::string path;
    };

    class Model {
        public:
            Model() = default;
            ~Model() = default;

            std::map<int, Mesh> meshes;
            tinygltf::Model model;
            Transform transform;
            std::vector<Texture> textures;

            void loadGLTF(std::string filename);
            void draw(const Shader &);

        private:
            // Loading
            void bindModel(tinygltf::Model &model);
            void bindModelNodes(tinygltf::Model &model, tinygltf::Node &node);
            Mesh bindMesh(tinygltf::Model &model, tinygltf::Mesh &mesh);
            void loadTextures(tinygltf::Model &model);

            // Drawing
            void drawModelNodes(tinygltf::Model &model, tinygltf::Node &node, glm::mat4 &parent);
            void drawMesh(Mesh mesh, tinygltf::Model &model, tinygltf::Mesh &tinymesh);

            const Shader *cachedShader = nullptr;
    };

}