#pragma once

#include <glad/glad.h>     // OpenGL function loader
#include <glm/glm.hpp>     // GLM for matrix and vector operations
#include <string>          // Standard C++ string class
#include <fstream>         // File input stream
#include <sstream>         // String stream to read file contents
#include <iostream>        // Standard I/O for error logging

// Shader class handles loading, compiling, linking and using shaders in OpenGL
class Shader {
public:
    unsigned int ID;  // Shader program ID

    // Constructor: Takes paths to vertex and fragment shader source files
    Shader(const char* vertexPath, const char* fragmentPath) {
        // Shader source code strings
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // Enable exceptions on file streams
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // Open shader source files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);

            // Read file contents into string streams
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();  // Read vertex file into stream
            fShaderStream << fShaderFile.rdbuf();  // Read fragment file into stream

            // Close file streams
            vShaderFile.close();
            fShaderFile.close();

            // Convert string streams to strings
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (std::ifstream::failure& e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n";
        }

        // Convert to raw C strings
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // Compile vertex shader
        unsigned int vertex;
        int success;
        char infoLog[512];

        vertex = glCreateShader(GL_VERTEX_SHADER);               // Create vertex shader object
        glShaderSource(vertex, 1, &vShaderCode, NULL);           // Attach shader source
        glCompileShader(vertex);                                 // Compile shader
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);      // Check compilation success
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        // Compile fragment shader
        unsigned int fragment;
        fragment = glCreateShader(GL_FRAGMENT_SHADER);           // Create fragment shader object
        glShaderSource(fragment, 1, &fShaderCode, NULL);         // Attach shader source
        glCompileShader(fragment);                               // Compile shader
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);    // Check compilation success
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << "\n";
        }

        // Link shaders into a program
        ID = glCreateProgram();                     // Create shader program
        glAttachShader(ID, vertex);                 // Attach compiled vertex shader
        glAttachShader(ID, fragment);               // Attach compiled fragment shader
        glLinkProgram(ID);                          // Link the program
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        }

        // Delete shaders after linking (no longer needed separately)
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    // Use/activate the shader program
    void use() const {
        glUseProgram(ID);
    }

    // Utility functions to set shader uniform variables of various types

    // Set a boolean uniform
    void setBool(const std::string& name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    // Set an integer uniform
    void setInt(const std::string& name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    // Set a float uniform
    void setFloat(const std::string& name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    // Set a vec3 (3D vector) uniform
    void setVec3(const std::string& name, const glm::vec3& vec) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec[0]);
    }

    // Set a mat4 (4x4 matrix) uniform
    void setMat4(const std::string& name, const glm::mat4& mat) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
};
