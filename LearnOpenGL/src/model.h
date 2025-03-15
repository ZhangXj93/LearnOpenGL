#ifndef MODEL_H
#define MODEL_H

#include "glm/glm.hpp"
#include "shader.hpp"
#include <string>
#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type; // 储存纹理的id和它的类型，比如diffuse纹理或者specular纹理。
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

private:
    void setupMesh();
    
private:
    unsigned int _VAO;
    unsigned int _VBO;
    unsigned int _EBO;
};

class Model
{
public:
    Model(const std::string &path);

    void Draw(Shader &shader);
private:
    void loadModel(const std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string &directory);
    
private:
    std::vector<Mesh> meshes;
    std::string directory;

};

#endif



