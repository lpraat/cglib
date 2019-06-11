#pragma once

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader_program.h>
#include "texture_loader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

namespace glp {

template <typename T = float32>
class Model
{
private:
    std::vector<Node<T>> nodes;
    std::string directory;

public:
    Model(std::string const &path)
    {
        loadModel(path);
    }

    // Draw all the model's meshes
    void draw(const ShaderProgram& shaderProgram, const Mat4<T>& projection, const Mat4<T>& view)
    {
        for (uint32 i = 0; i < nodes.size(); i++) {
            for(uint32 j = 0; j < nodes[i].meshes.size(); j++) {
                nodes[i].meshes[j].draw(shaderProgram, projection, view);
            }
        }
    }

    void _updateModelMatrices(Node<T>* root, const Mat4<T>& curr) {
        const Mat4<T> model = curr.dot(root->model);
        root->model = model;

        for (uint32 i = 0; i < root->children.size(); i++) {
            _updateModelMatrices(root->children[i], model);
        }
    }

    void updateModelMatrices() {
        _updateModelMatrices(&nodes[0], Mat4<T>::identity());
    }

    void resetModelMatrices() {
        for (uint32 i = 0; i < nodes.size(); i++) {
            nodes[i].model = Mat4<T>::identity();
        }
    }

    // Loads a support assimp extension from file.
    void loadModel(const std::string &path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        // Check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        // Count how many nodes are present
        uint32 numNodes = 0;
        countNodes(scene->mRootNode, &numNodes);

        // Process assimp data structure
        nodes = std::vector<Node<T>>(numNodes);
        uint32 idx = 0;
        processNode(scene->mRootNode, scene, nullptr, &idx);
    }

    void countNodes(aiNode *node, uint32* numNodes) {
        (*numNodes)++;

        for(uint32 i = 0; i < node->mNumChildren; i++)
        {
            countNodes(node->mChildren[i], numNodes);
        }
    }

    void processNode(aiNode *node, const aiScene *scene, Node<T>* parent, uint32* nodeIndex)
    {
        Node<T>* newNode = &(nodes[*nodeIndex]);
        (*nodeIndex)++;

        newNode->name = node->mName.C_Str();

        // Process each mesh at this node
        for (uint32 i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            newNode->meshes.push_back(processMesh(newNode, mesh, scene));
        }

        if (parent != nullptr) {
            parent->children.push_back(newNode);
        }

        // Process all the children of this node
        for(uint32 i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene, newNode, nodeIndex);
        }
    }

    Mesh<T> processMesh(Node<T>* node, aiMesh *mesh, const aiScene *scene)
    {
        std::vector<Vertex<T>> vertices;
        std::vector<uint32> indices;
        std::vector<Texture> textures;

        for(uint32 i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex<T> vertex;
            Vec3<T> vector;

            // Positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            // Normals
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            // Texture coordinates
            if(mesh->mTextureCoords[0]) {
                Vec2<T> vec {0, 0};

                // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else {
                vertex.TexCoords = Vec2<T> {0, 0};
            }

            // Tangents
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangents
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;


            vertices.push_back(vertex);
        }

        // For each face get the indices
        for(uint32 i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(uint32 j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        std::vector<Texture> alreadyLoaded;

        // Diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", alreadyLoaded);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // Specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", alreadyLoaded);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // Normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", alreadyLoaded);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // Height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", alreadyLoaded);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        return Mesh<T>(node, mesh->mName.C_Str(), vertices, indices, textures);
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName, std::vector<Texture>& alreadyLoaded)
    {
        std::vector<Texture> textures;
        for (uint32 i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
            bool skip = false;
            for(uint32 j = 0; j < alreadyLoaded.size(); j++)
            {
                if(std::strcmp(alreadyLoaded[j].path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(alreadyLoaded[j]);
                    skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                    break;
                }
            }
            if (!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                std::string texturePath = this->directory + '/' + str.C_Str();
                texture.id = TextureLoader::textureFromFile(texturePath);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                alreadyLoaded.push_back(texture);
            }
        }
        return textures;
    }

    std::vector<Node<T>>& getNodes() {
        return nodes;
    }

    void print() {
        std::cout << "Model name: " << directory << std::endl;
        for (uint32 i = 0; i < nodes.size(); i++) {
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            std::cout << "Node: " << nodes[i].name << std::endl;
            for (uint32 j = 0; j < nodes[i].meshes.size(); j++) {
                nodes[i].meshes[j].print();
            }
        }
    }
};

}; // namespace glp