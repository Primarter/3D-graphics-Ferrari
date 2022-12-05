#pragma once

#define GL_SILENCE_DEPRECATION
#include <string>
#include <iostream>
#include <vector>
#include <glad/glad.h>

#include <tinygltf/tiny_gltf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Transform.hpp"
#include "Shader.hpp"

// #define BUFFER_OFFSET(i) ((char *)NULL + (i)) <- arithmetic on a null pointer treated as a cast from integer to pointer is a GNU extension, MinGW not happy
#define BUFFER_OFFSET(i) ((char *)(1-1) + (i)) // this stupid bit makes MinGW happy

namespace GLApp
{
    typedef GLuint MaterialId;

    typedef uint8_t FeatureMask;

    enum ModelFeatures
    {
        NONE = 0b0,
        ALBEDO = 0b1,
        NORMALS = 0b10,
        METALLIC_ROUGHNESS = 0b100,
        AMBIENT_OCCLUSION = 0b1000,
        ALL = 0b1111
    };

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
            std::map<int, Texture> textures;
            FeatureMask features = ALBEDO & NORMALS & METALLIC_ROUGHNESS & AMBIENT_OCCLUSION;

            void loadGLTF(std::string filename, FeatureMask features = NONE);
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