#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vector3.h" // Include the Vector3 class for 3D vector operations
#include "Color.h" // Include the Color class for color representation

#include <string> // Include string for material ID and texture file paths
#include <iostream> // Include iostream for logging or error messages
#include <cmath> // Include cmath for mathematical operations
#include <algorithm> // Include algorithm for utility functions like std::min
#include <vector> // Include vector for storing texture coordinates
#include <tuple> // Include tuple for texture coordinate pairs

// Class to represent material properties
class Material {
public:
    Vector3 ambient; // Ambient reflectance of the material
    Vector3 diffuse; // Diffuse reflectance of the material
    Vector3 specular; // Specular reflectance of the material
    Vector3 mirrorReflection; // Mirror reflection properties of the material

    double phongExponent; // Phong exponent for specular highlights
    double textureFactor; // Factor to blend texture with diffuse color

    std::string material_id; // Unique identifier for the material

    unsigned char* textureData = nullptr; // Pointer to texture data
    int texWidth = 0; // Width of the texture
    int texHeight = 0; // Height of the texture
    int texChannels = 0; // Number of color channels in the texture

    // Struct to represent texture information
    struct Texture {
        std::vector<std::tuple<float, float>> coords; // Texture coordinates
        std::string imagePath; // Path to the texture image file
    };

    Texture texture; // Texture data as a member

    // Constructor to initialize material properties
    Material(std::string material_id,
             Vector3 ambient = Vector3(0.0, 0.0, 0.0),
             Vector3 diffuse = Vector3(1.0, 1.0, 1.0),
             Vector3 specular = Vector3(1.0, 1.0, 1.0),
             Vector3 mirrorReflection = Vector3(0.0, 0.0, 0.0),
             double phongExponent = 32.0,
             double textureFactor = 0.0,
             std::string textureFilePath = "")
        : material_id(material_id), ambient(ambient), diffuse(diffuse), specular(specular),
          mirrorReflection(mirrorReflection), phongExponent(phongExponent),
          textureFactor(textureFactor)
    {
        if (!textureFilePath.empty()) {
            // Load the texture if a valid file path is provided
            texture.imagePath = textureFilePath;
            
            //tureData = stbi_load(textureFilePath.c_str(), &texWidth, &texHeight, &texChannels, 3);
            if (!textureData) {
                std::cerr << "Failed to load texture: " << textureFilePath << std::endl;
            }
        }
    }

    // Destructor to free texture data
    ~Material() {
        if (textureData) {
            // stbi_image_free(textureData);
            textureData = nullptr;
        }
    }

    // Method to sample a color from the texture using UV coordinates
    Color sampleTexture(float u, float v) const {
        if (!textureData || texWidth == 0 || texHeight == 0)
            return Color(1.0, 1.0, 1.0); // Return white if no texture is loaded

        // Wrap around UV coordinates if they are out of bounds
        u = fmod(u, 1.0f); if (u < 0) u += 1.0f;
        v = fmod(v, 1.0f); if (v < 0) v += 1.0f;

        // Flip the v-coordinate for image space
        v = 1.0f - v;

        // Calculate the pixel position in the texture
        int x = std::min(int(u * texWidth), texWidth - 1);
        int y = std::min(int(v * texHeight), texHeight - 1);
        int index = (y * texWidth + x) * texChannels;

        // Extract the RGB values from the texture data
        float r = textureData[index] / 255.0f;
        float g = textureData[index + 1] / 255.0f;
        float b = textureData[index + 2] / 255.0f;

        return Color(r, g, b); // Return the sampled color
    }

    // Method to evaluate the material's reflectance with optional texture mapping
    Color evaluate(Vector3 l, Vector3 v, Vector3 n, float u = 0.0f, float v_tex = 0.0f) {
        double diffuse_intensity = std::max(0.0, dot(n, l)); // Compute diffuse intensity
        Vector3 h = unit_vector(l + v); // Compute the halfway vector
        double specular_intensity = std::pow(std::max(0.0, dot(n, h)), phongExponent); // Compute specular intensity

        Color baseDiffuse = diffuse; // Base diffuse color

        // Blend diffuse color with texture if textureFactor > 0
        if (textureFactor > 0.0 && textureData) {
            Color texColor = sampleTexture(u, v_tex);
            baseDiffuse = (1.0 - textureFactor) * diffuse + textureFactor * texColor;
        }

        // Compute the diffuse and specular terms
        Color diffuse_term = baseDiffuse * diffuse_intensity;
        Color specular_term = specular * specular_intensity;

        // Combine ambient, diffuse, and specular terms for total reflectance
        Color total_reflectance = (1.0 / 3.0) * ambient +
                                  (1.0 / 3.0) * diffuse_term +
                                  (1.0 / 3.0) * specular_term;

        return total_reflectance; // Return the total reflectance
    }
};

#endif // MATERIAL_H