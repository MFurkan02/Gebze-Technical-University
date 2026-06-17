#include "model.h"
#include <iostream>

// Constructor: Loads and sets up the model from the given file path
Model::Model(const std::string& path) {
    loadModel(path);   // Load mesh data from file
    setupMesh();       // Create VAO/VBO/EBO and send data to GPU
}

// Loads mesh data from the specified file using Assimp
void Model::loadModel(const std::string& path) {
    Assimp::Importer importer;

    // Import the 3D model file and process it:
    // Triangulate: convert all faces to triangles
    // Flip UVs: flip texture coordinates vertically
    // Generate normals: if not provided in model
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    // Error handling
    if (!scene || !scene->HasMeshes()) {
        std::cerr << "ERROR::ASSIMP: " << importer.GetErrorString() << std::endl;
        return;
    }

    // We're only loading the first mesh in the scene
    aiMesh* mesh = scene->mMeshes[0];

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // Get position
        vertex.Position = glm::vec3(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );

        // Get normal
        vertex.Normal = glm::vec3(
            mesh->mNormals[i].x,
            mesh->mNormals[i].y,
            mesh->mNormals[i].z
        );

        // Get texture coordinates if present
        if (mesh->HasTextureCoords(0)) {
            vertex.TexCoords = glm::vec2(
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            );
        } else {
            vertex.TexCoords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process indices from mesh faces
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
}

// Sets up OpenGL buffers (VAO, VBO, EBO) for rendering the mesh
void Model::setupMesh() {
    // Generate VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO (vertex array object)
    glBindVertexArray(VAO);

    // Copy vertex data into VBO (vertex buffer object)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Copy index data into EBO (element buffer object)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Vertex attribute: position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // Vertex attribute: normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    // Vertex attribute: texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    // Unbind the VAO to avoid accidental modifications
    glBindVertexArray(0);
}

// Draws the model using the bound shader program
void Model::Draw(Shader& shader) {
    glBindVertexArray(VAO); // Bind the VAO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Draw mesh using indices
    glBindVertexArray(0); // Unbind
}
