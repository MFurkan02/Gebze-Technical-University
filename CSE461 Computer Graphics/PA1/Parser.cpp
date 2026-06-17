#include "pugi/pugixml.hpp" // Include the pugixml library for XML parsing
#include <iostream> // For input/output operations
#include <vector> // For storing collections of data
#include <sstream> // For string stream operations
#include <tuple> // For storing pairs or tuples of data

// Struct to represent a 3D vector
struct Vector3 {
    float x, y, z; // Components of the vector
};

// Struct to represent camera properties
struct Camera {
    Vector3 position; // Camera position
    Vector3 gaze; // Camera gaze direction
    Vector3 up; // Camera up vector
    std::tuple<float, float, float, float> nearPlane; // Near plane bounds (left, right, bottom, top)
    float nearDistance; // Distance to the near plane
    std::tuple<int, int> imageResolution; // Image resolution (width, height)
};

// Struct to represent a light source
struct Light {
    std::string type; // Type of light (e.g., point, ambient)
    int id; // Light ID
    Vector3 position; // Position of the light (for point lights)
    Vector3 intensity; // Intensity of the light
};

// Struct to represent material properties
struct Material {
    int id; // Material ID
    Vector3 ambient; // Ambient reflectance
    Vector3 diffuse; // Diffuse reflectance
    Vector3 specular; // Specular reflectance
    Vector3 mirrorReflectance; // Mirror reflectance
    float phongExponent; // Phong exponent for specular highlights
};

// Struct to represent a face of a mesh
struct Face {
    std::vector<int> vertices; // Indices of vertices that form the face
    std::vector<int> textureCoords; // Indices of texture coordinates
    std::vector<int> normals; // Indices of normals
};

// Struct to represent a 3D mesh
struct Mesh {
    int id; // Mesh ID
    int materialId; // ID of the material applied to the mesh
    std::vector<Face> faces; // List of faces in the mesh
};

// Struct to represent texture data
struct Texture {
    std::vector<std::tuple<float, float>> coords; // Texture coordinates (u, v)
    std::string imagePath; // Path to the texture image
};

// Struct to represent normal data
struct Normal {
    std::vector<Vector3> normals; // List of normal vectors
};

int main() {
    pugi::xml_document doc; // Create an XML document object
    if (!doc.load_file("scene.xml")) { // Load the XML file
        std::cerr << "Error loading XML file." << std::endl; // Print error if loading fails
        return 1; // Exit with error code
    }

    // Extract camera data
    Camera camera;
    auto camera_node = doc.child("scene").child("camera");
    camera.position = { camera_node.child("position").text().as_float(),
                        camera_node.child("position").next_sibling("gaze").text().as_float(),
                        camera_node.child("position").next_sibling("gaze").next_sibling("up").text().as_float() };
    camera.gaze = { camera_node.child("gaze").text().as_float(),
                    camera_node.child("gaze").next_sibling("up").text().as_float(),
                    camera_node.child("gaze").next_sibling("up").next_sibling("nearPlane").text().as_float() };
    camera.up = { camera_node.child("up").text().as_float(),
                  camera_node.child("up").next_sibling("nearPlane").text().as_float(),
                  camera_node.child("up").next_sibling("nearPlane").next_sibling("neardistance").text().as_float() };
    std::istringstream near_plane_ss(camera_node.child("nearPlane").text().as_string());
    near_plane_ss >> std::get<0>(camera.nearPlane) >> std::get<1>(camera.nearPlane) >> std::get<2>(camera.nearPlane) >> std::get<3>(camera.nearPlane);
    camera.nearDistance = camera_node.child("neardistance").text().as_float();
    std::istringstream image_resolution_ss(camera_node.child("imageresolution").text().as_string());
    image_resolution_ss >> std::get<0>(camera.imageResolution) >> std::get<1>(camera.imageResolution);

    // Extract texture data
    Texture texture;
    auto texture_data_node = doc.child("scene").child("texturedata");
    for (pugi::xml_node coord_node = texture_data_node.first_child(); coord_node; coord_node = coord_node.next_sibling()) {
        float u = coord_node.child_value("u") ? std::stof(coord_node.child_value("u")) : 0.0f;
        float v = coord_node.child_value("v") ? std::stof(coord_node.child_value("v")) : 0.0f;
        texture.coords.push_back(std::make_tuple(u, v));
    }
    texture.imagePath = doc.child("scene").child("textureimage").text().as_string();

    // Extract normal data
    Normal normalData;
    auto normal_data_node = doc.child("scene").child("normaldata");
    for (pugi::xml_node normal_node = normal_data_node.first_child(); normal_node; normal_node = normal_node.next_sibling()) {
        Vector3 normal = {
            normal_node.child_value("x") ? std::stof(normal_node.child_value("x")) : 0.0f,
            normal_node.child_value("y") ? std::stof(normal_node.child_value("y")) : 0.0f,
            normal_node.child_value("z") ? std::stof(normal_node.child_value("z")) : 0.0f
        };
        normalData.normals.push_back(normal);
    }

    // Extract lights
    std::vector<Light> lights;
    for (pugi::xml_node light_node = doc.child("scene").child("lights").first_child(); light_node; light_node = light_node.next_sibling()) {
        Light light;
        light.type = light_node.name();
        if (light.type == "ambientlight") {
            light.intensity = { light_node.text().as_float(), 0.0f, 0.0f };
        }
        else {
            light.id = light_node.attribute("id").as_int();
            light.position = { light_node.child("position").text().as_float(),
                                light_node.child("position").next_sibling("intensity").text().as_float(),
                                light_node.child("position").next_sibling("intensity").next_sibling().text().as_float() };
            light.intensity = { light_node.child("intensity").text().as_float(),
                                light_node.child("intensity").next_sibling().text().as_float(),
                                light_node.child("intensity").next_sibling().next_sibling().text().as_float() };
        }
        lights.push_back(light);
    }

    // Extract materials
    std::vector<Material> materials;
    for (pugi::xml_node material_node = doc.child("scene").child("materials").first_child(); material_node; material_node = material_node.next_sibling()) {
        Material material;
        material.id = material_node.attribute("id").as_int();
        material.ambient = { material_node.child("ambient").text().as_float(),
                            material_node.child("ambient").next_sibling("diffuse").text().as_float(),
                            material_node.child("ambient").next_sibling("diffuse").next_sibling("specular").text().as_float() };
        material.diffuse = { material_node.child("diffuse").text().as_float(),
                            material_node.child("diffuse").next_sibling("specular").text().as_float(),
                            material_node.child("diffuse").next_sibling("specular").next_sibling("mirrorreflactance").text().as_float() };
        material.specular = { material_node.child("specular").text().as_float(),
                             material_node.child("specular").next_sibling("mirrorreflactance").text().as_float(),
                             material_node.child("specular").next_sibling("mirrorreflactance").next_sibling("phongexponent").text().as_float() };
        material.mirrorReflectance = { material_node.child("mirrorreflactance").text().as_float(),
                                      material_node.child("mirrorreflactance").next_sibling("phongexponent").text().as_float(),
                                      0.0f };
        material.phongExponent = material_node.child("phongexponent").text().as_float();
        materials.push_back(material);
    }

    // Extract vertex data
    std::vector<Vector3> vertices;
    std::istringstream vertex_data_ss(doc.child("scene").child("vertexdata").child("data").text().as_string());
    while (!vertex_data_ss.eof()) {
        Vector3 vertex;
        vertex_data_ss >> vertex.x >> vertex.y >> vertex.z;
        vertices.push_back(vertex);
    }

    // Extract objects
    std::vector<Mesh> meshes;
    for (pugi::xml_node mesh_node = doc.child("scene").child("objects").first_child(); mesh_node; mesh_node = mesh_node.next_sibling()) {
        Mesh mesh;
        mesh.id = mesh_node.attribute("id").as_int();
        mesh.materialId = mesh_node.child("materialid").text().as_int();
        for (pugi::xml_node face_node = mesh_node.child("faces").first_child(); face_node; face_node = face_node.next_sibling()) {
            Face face;
            std::istringstream face_ss(face_node.text().as_string());
            while (!face_ss.eof()) {
                int vertex_index;
                face_ss >> vertex_index;
                face.vertices.push_back(vertex_index);
            }
            mesh.faces.push_back(face);
        }
        meshes.push_back(mesh);
    }

    // Output extracted data
    std::cout << "\nTexture Data:" << std::endl;
    for (const auto& coord : texture.coords) {
        std::cout << "  " << std::get<0>(coord) << " " << std::get<1>(coord) << std::endl;
    }
    std::cout << "Texture Image Path: " << texture.imagePath << std::endl;

    std::cout << "\nNormal Data:" << std::endl;
    for (const auto& normal : normalData.normals) {
        std::cout << "  " << normal.x << " " << normal.y << " " << normal.z << std::endl;
    }

    return 0;
}