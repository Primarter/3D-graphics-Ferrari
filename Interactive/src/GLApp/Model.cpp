#include "Model.hpp"


namespace GLApp {

    void Model::LoadGLTF(std::string filename)
    {

        std::cout << "Trying to load model " << filename << "\n";

        tinygltf::TinyGLTF loader;
        std::string err;
        std::string warn;

        bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
        if (!warn.empty()) std::cout << "WARNING - GLTF: " << warn << std::endl;
        if (!err.empty())  std::cout << "ERROR - GLTF: " << err << std::endl;

        if (!res)
            std::cout << "ERROR - Failed to load glTF: " << filename << std::endl;
        else
            std::cout << "OK - Loaded glTF: " << filename << std::endl;

        vaoAndEbos = BindModel(model);
    }


    void Model::BindMesh(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh)
    {
        for (size_t i = 0; i < model.bufferViews.size(); ++i)
        {
            const tinygltf::BufferView &bufferView = model.bufferViews[i];
            if (bufferView.target == 0)
            { // TODO impl drawarrays
                std::cout << "WARN: bufferView.target is zero" << std::endl;
                continue; // Unsupported bufferView.
            }

            const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
            std::cout << "bufferview.target " << bufferView.target << std::endl;

            GLuint vbo;
            glGenBuffers(1, &vbo);
            vbos[i] = vbo;
            glBindBuffer(bufferView.target, vbo);

            std::cout << "buffer.data.size = " << buffer.data.size()
                    << ", bufferview.byteOffset = " << bufferView.byteOffset
                    << std::endl;

            glBufferData(bufferView.target, bufferView.byteLength,
                        &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);
        }

        for (size_t i = 0; i < mesh.primitives.size(); ++i)
        {
            tinygltf::Primitive primitive = mesh.primitives[i];
            tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

            for (auto &attrib : primitive.attributes)
            {
                tinygltf::Accessor accessor = model.accessors[attrib.second];
                int byteStride = accessor.ByteStride(model.bufferViews[accessor.bufferView]);
                glBindBuffer(GL_ARRAY_BUFFER, vbos[accessor.bufferView]);

                int size = 1;
                if (accessor.type != TINYGLTF_TYPE_SCALAR)
                {
                    size = accessor.type;
                }

                int vaa = -1;
                if (attrib.first.compare("POSITION") == 0)
                    vaa = 0;
                if (attrib.first.compare("NORMAL") == 0)
                    vaa = 1;
                if (attrib.first.compare("TEXCOORD_0") == 0)
                    vaa = 2;
                if (vaa > -1)
                {
                    glEnableVertexAttribArray(vaa);
                    glVertexAttribPointer(vaa, size, accessor.componentType,
                                        accessor.normalized ? GL_TRUE : GL_FALSE,
                                        byteStride, BUFFER_OFFSET(accessor.byteOffset));
                }
                else
                    std::cout << "vaa missing: " << attrib.first << std::endl;
            }

            if (model.textures.size() > 0)
            {
                // fixme: Use material's baseColor
                tinygltf::Texture &tex = model.textures[0];

                if (tex.source > -1)
                {

                    GLuint texid;
                    glGenTextures(1, &texid);

                    tinygltf::Image &image = model.images[tex.source];

                    glBindTexture(GL_TEXTURE_2D, texid);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                    GLenum format = GL_RGBA;

                    if (image.component == 1) format = GL_RED;
                    else if (image.component == 2) format = GL_RG;
                    else if (image.component == 3) format = GL_RGB;
                    else { // ???
                    }

                    GLenum type = GL_UNSIGNED_BYTE;
                    if (image.bits == 8){// ok
                    }
                    else if (image.bits == 16) type = GL_UNSIGNED_SHORT;
                    else{ // ???
                    }

                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0,
                                format, type, &image.image.at(0));
                }
            }
        }
    }

    // bind models
    void Model::BindModelNodes(std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Node &node)
    {
        if ((node.mesh >= 0) && (static_cast<size_t>(node.mesh) < model.meshes.size()))
        {
            BindMesh(vbos, model, model.meshes[node.mesh]);
        }

        for (size_t i = 0; i < node.children.size(); i++)
        {
            assert((node.children[i] >= 0) && (static_cast<size_t>(node.children[i]) < model.nodes.size()));
            BindModelNodes(vbos, model, model.nodes[node.children[i]]);
        }
    }

    std::pair<GLuint, std::map<int, GLuint>> Model::BindModel(tinygltf::Model &model)
    {
        std::map<int, GLuint> vbos;
        GLuint vao;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i)
        {
            assert((scene.nodes[i] >= 0) && (static_cast<size_t>(scene.nodes[i]) < model.nodes.size()));
            BindModelNodes(vbos, model, model.nodes[scene.nodes[i]]);
        }

        glBindVertexArray(0);

        // cleanup vbos but do not delete index buffers yet
        for (auto it = vbos.cbegin(); it != vbos.cend();)
        {
            tinygltf::BufferView bufferView = model.bufferViews[it->first];
            if (bufferView.target != GL_ELEMENT_ARRAY_BUFFER)
            {
                glDeleteBuffers(1, &vbos[it->first]);
                vbos.erase(it++);
            }
            else
            {
                ++it;
            }
        }

        return {vao, vbos};
    }

    void Model::DrawMesh(const std::map<int, GLuint> &vbos, tinygltf::Model &model, tinygltf::Mesh &mesh)
    {
        for (size_t i = 0; i < mesh.primitives.size(); ++i)
        {
            tinygltf::Primitive primitive = mesh.primitives[i];
            tinygltf::Accessor indexAccessor = model.accessors[primitive.indices];

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos.at(indexAccessor.bufferView));

            glDrawElements(primitive.mode, indexAccessor.count,
                        indexAccessor.componentType,
                        BUFFER_OFFSET(indexAccessor.byteOffset));
        }
    }

    // recursively draw node and children nodes of model
    void Model::DrawModelNodes(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos, tinygltf::Model &model, tinygltf::Node &node)
    {
        if ((node.mesh >= 0) && (static_cast<size_t>(node.mesh) < model.meshes.size()))
        {
            DrawMesh(vaoAndEbos.second, model, model.meshes[node.mesh]);
        }
        for (size_t i = 0; i < node.children.size(); i++)
        {
            DrawModelNodes(vaoAndEbos, model, model.nodes[node.children[i]]);
        }
    }

    void Model::Draw(const std::pair<GLuint, std::map<int, GLuint>> &vaoAndEbos, tinygltf::Model &model)
    {
        glBindVertexArray(vaoAndEbos.first);

        const tinygltf::Scene &scene = model.scenes[model.defaultScene];
        for (size_t i = 0; i < scene.nodes.size(); ++i)
        {
            DrawModelNodes(vaoAndEbos, model, model.nodes[scene.nodes[i]]);
        }

        glBindVertexArray(0);
    }

}