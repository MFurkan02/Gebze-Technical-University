#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

#include "Camera.h"
#include "shader.h"

// Callbacks and input handling declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f)); // moved back a bit for all objects
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Define obstacle objects with their positions and bounding boxes
struct SceneObject {
    glm::vec3 position;
    AABB boundingBox;
};

std::vector<SceneObject> sceneObjects;

// Function to create AABB for an object
AABB createBoundingBox(glm::vec3 position, glm::vec3 min, glm::vec3 max) {
    AABB box;
    box.min = position + min;
    box.max = position + max;
    return box;
}

int main()
{
    // GLFW: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "3D Bounded World - Multiple Objects", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Capture the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // GLAD: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Enable depth testing (z-buffering)
    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    float cubeVertices[] = {
    // Back face (z = -1)
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    // Front face (z = 1)
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

    // Top face (y = 1)
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,

    // Bottom face (y = -1)
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,

    // Left face (x = -1)
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,

    // Right face (x = 1)
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
};


 // Pyramid with normals and texture coordinates
float pyramidVertices[] = {
    // Positions           // Normals           // Texture Coords
    // Base (square) - 2 triangles
    -1.0f, 0.0f, -1.0f,    0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
     1.0f, 0.0f, -1.0f,    0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
     1.0f, 0.0f,  1.0f,    0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
     1.0f, 0.0f,  1.0f,    0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
    -1.0f, 0.0f,  1.0f,    0.0f, -1.0f, 0.0f,   0.0f, 1.0f,
    -1.0f, 0.0f, -1.0f,    0.0f, -1.0f, 0.0f,   0.0f, 0.0f,

    // Side 1
    -1.0f, 0.0f, -1.0f,   -0.894f, 0.447f, 0.0f,   0.0f, 0.0f,
     1.0f, 0.0f, -1.0f,    0.894f, 0.447f, 0.0f,   1.0f, 0.0f,
     0.0f, 1.5f, 0.0f,     0.0f, 0.447f, -0.894f,  0.5f, 1.0f,

    // Side 2
     1.0f, 0.0f, -1.0f,    0.0f, 0.447f, -0.894f,  0.0f, 0.0f,
     1.0f, 0.0f,  1.0f,    0.0f, 0.447f, 0.894f,   1.0f, 0.0f,
     0.0f, 1.5f, 0.0f,     0.894f, 0.447f, 0.0f,   0.5f, 1.0f,

    // Side 3
     1.0f, 0.0f,  1.0f,    0.894f, 0.447f, 0.0f,   0.0f, 0.0f,
    -1.0f, 0.0f,  1.0f,   -0.894f, 0.447f, 0.0f,   1.0f, 0.0f,
     0.0f, 1.5f, 0.0f,     0.0f, 0.447f, 0.894f,   0.5f, 1.0f,

    // Side 4
    -1.0f, 0.0f,  1.0f,    0.0f, 0.447f, 0.894f,   0.0f, 0.0f,
    -1.0f, 0.0f, -1.0f,    0.0f, 0.447f, -0.894f,  1.0f, 0.0f,
     0.0f, 1.5f, 0.0f,     -0.894f, 0.447f, 0.0f,  0.5f, 1.0f
};

        // Tetrahedron with normals and texture coordinates
    float tetrahedronVertices[] = {
        // Positions           // Normals           // Texture Coords
        // Face 1
        0.0f,  1.0f,  0.0f,    0.0f, 0.816f, -0.577f,  0.5f, 1.0f,
        -1.0f, -1.0f,  1.0f,   -0.816f, -0.333f, 0.471f, 0.0f, 0.0f,
        1.0f, -1.0f,  1.0f,    0.816f, -0.333f, 0.471f, 1.0f, 0.0f,

        // Face 2
        0.0f,  1.0f,  0.0f,    0.816f, 0.333f, -0.471f, 0.5f, 1.0f,
        1.0f, -1.0f,  1.0f,    0.0f, -0.333f, -0.943f, 0.0f, 0.0f,
        0.0f, -1.0f, -1.0f,    -0.816f, -0.333f, 0.471f, 1.0f, 0.0f,

        // Face 3
        0.0f,  1.0f,  0.0f,    -0.816f, 0.333f, -0.471f, 0.5f, 1.0f,
        0.0f, -1.0f, -1.0f,    0.0f, -0.333f, -0.943f, 0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   0.816f, -0.333f, 0.471f, 1.0f, 0.0f,

        // Base
        -1.0f, -1.0f,  1.0f,   0.0f, -1.0f, 0.0f,     0.0f, 1.0f,
        1.0f, -1.0f,  1.0f,    0.0f, -1.0f, 0.0f,     1.0f, 1.0f,
        0.0f, -1.0f, -1.0f,    0.0f, -1.0f, 0.0f,     0.5f, 0.0f
    };

    // Ground plane with normals and texture coordinates
    float groundVertices[] = {
        // Positions           // Normals           // Texture Coords
        -10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
        10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f,   10.0f, 10.0f,
        10.0f, -1.0f,  10.0f, 0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
        10.0f, -1.0f,  10.0f, 0.0f, 1.0f, 0.0f,   10.0f, 0.0f,
        -10.0f, -1.0f,  10.0f, 0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        -10.0f, -1.0f, -10.0f, 0.0f, 1.0f, 0.0f,   0.0f, 10.0f
    };

    // Wall with normals and texture coordinates
    float wallVertices[] = {
        // Positions          // Normals          // Texture Coords
        -1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
        1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
        1.0f,  2.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        1.0f,  2.0f, -1.0f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
        -1.0f,  2.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
        -1.0f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f
    };

    float coneVertices[] = {
    // Side triangles (tip at 0,1,0)
    // Face 1
     0.0f, 1.0f, 0.0f,    0.0f, 0.894f, -0.447f,     0.5f, 1.0f,   // tip
     1.0f, 0.0f, 0.0f,    0.0f, 0.894f, -0.447f,     0.0f, 0.0f,   // base point 1
     0.866f, 0.0f, 0.5f,  0.0f, 0.894f, -0.447f,     0.083f, 0.0f, // base point 2

    // Face 2
     0.0f, 1.0f, 0.0f,    0.433f, 0.866f, -0.25f,    0.5f, 1.0f,
     0.866f, 0.0f, 0.5f,  0.433f, 0.866f, -0.25f,    0.083f, 0.0f,
     0.5f, 0.0f, 0.866f,  0.433f, 0.866f, -0.25f,    0.167f, 0.0f,

    // Face 3
     0.0f, 1.0f, 0.0f,    0.75f, 0.661f, 0.047f,     0.5f, 1.0f,
     0.5f, 0.0f, 0.866f,  0.75f, 0.661f, 0.047f,     0.167f, 0.0f,
     0.0f, 0.0f, 1.0f,    0.75f, 0.661f, 0.047f,     0.25f, 0.0f,

    // Face 4
     0.0f, 1.0f, 0.0f,    0.866f, 0.5f, 0.0f,        0.5f, 1.0f,
     0.0f, 0.0f, 1.0f,    0.866f, 0.5f, 0.0f,        0.25f, 0.0f,
    -0.5f, 0.0f, 0.866f,  0.866f, 0.5f, 0.0f,        0.333f, 0.0f,

    // Face 5
     0.0f, 1.0f, 0.0f,    0.75f, 0.433f, 0.5f,       0.5f, 1.0f,
    -0.5f, 0.0f, 0.866f,  0.75f, 0.433f, 0.5f,       0.333f, 0.0f,
    -0.866f, 0.0f, 0.5f,  0.75f, 0.433f, 0.5f,       0.417f, 0.0f,

    // Face 6
     0.0f, 1.0f, 0.0f,    0.433f, 0.25f, 0.866f,     0.5f, 1.0f,
    -0.866f, 0.0f, 0.5f,  0.433f, 0.25f, 0.866f,     0.417f, 0.0f,
    -1.0f, 0.0f, 0.0f,    0.433f, 0.25f, 0.866f,     0.5f, 0.0f,

    // Face 7
     0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,          0.5f, 1.0f,
    -1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,          0.5f, 0.0f,
    -0.866f, 0.0f, -0.5f, 0.0f, 0.0f, 1.0f,          0.583f, 0.0f,

    // Face 8
     0.0f, 1.0f, 0.0f,    -0.433f, 0.25f, 0.866f,    0.5f, 1.0f,
    -0.866f, 0.0f, -0.5f, -0.433f, 0.25f, 0.866f,    0.583f, 0.0f,
    -0.5f, 0.0f, -0.866f, -0.433f, 0.25f, 0.866f,    0.667f, 0.0f,

    // Face 9
     0.0f, 1.0f, 0.0f,    -0.75f, 0.433f, 0.5f,      0.5f, 1.0f,
    -0.5f, 0.0f, -0.866f, -0.75f, 0.433f, 0.5f,      0.667f, 0.0f,
    0.0f, 0.0f, -1.0f,    -0.75f, 0.433f, 0.5f,      0.75f, 0.0f,

    // Face 10
     0.0f, 1.0f, 0.0f,    -0.866f, 0.5f, 0.0f,       0.5f, 1.0f,
     0.0f, 0.0f, -1.0f,   -0.866f, 0.5f, 0.0f,       0.75f, 0.0f,
     0.5f, 0.0f, -0.866f, -0.866f, 0.5f, 0.0f,       0.833f, 0.0f,

    // Face 11
     0.0f, 1.0f, 0.0f,    -0.75f, 0.661f, -0.047f,   0.5f, 1.0f,
     0.5f, 0.0f, -0.866f, -0.75f, 0.661f, -0.047f,   0.833f, 0.0f,
     0.866f, 0.0f, -0.5f, -0.75f, 0.661f, -0.047f,   0.917f, 0.0f,

    // Face 12
     0.0f, 1.0f, 0.0f,    -0.433f, 0.866f, 0.25f,    0.5f, 1.0f,
     0.866f, 0.0f, -0.5f, -0.433f, 0.866f, 0.25f,    0.917f, 0.0f,
     1.0f, 0.0f, 0.0f,    -0.433f, 0.866f, 0.25f,    1.0f, 0.0f,

    // Base triangles (circle at y=0)
    // Base Face 1
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,   // center
     0.866f, 0.0f, 0.5f,   0.0f, -1.0f, 0.0f,        0.933f, 0.75f,
     1.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        1.0f, 0.5f,

    // Base Face 2
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
     0.5f, 0.0f, 0.866f,   0.0f, -1.0f, 0.0f,        0.75f, 0.933f,
     0.866f, 0.0f, 0.5f,   0.0f, -1.0f, 0.0f,        0.933f, 0.75f,

    // Base Face 3
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
     0.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f,        0.5f, 1.0f,
     0.5f, 0.0f, 0.866f,   0.0f, -1.0f, 0.0f,        0.75f, 0.933f,

    // Base Face 4
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
    -0.5f, 0.0f, 0.866f,   0.0f, -1.0f, 0.0f,        0.25f, 0.933f,
     0.0f, 0.0f, 1.0f,     0.0f, -1.0f, 0.0f,        0.5f, 1.0f,

    // Base Face 5
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
    -0.866f, 0.0f, 0.5f,   0.0f, -1.0f, 0.0f,        0.067f, 0.75f,
    -0.5f, 0.0f, 0.866f,   0.0f, -1.0f, 0.0f,        0.25f, 0.933f,

    // Base Face 6
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
    -1.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.0f, 0.5f,
    -0.866f, 0.0f, 0.5f,   0.0f, -1.0f, 0.0f,        0.067f, 0.75f,

    // Base Face 7
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
    -0.866f, 0.0f, -0.5f,  0.0f, -1.0f, 0.0f,        0.067f, 0.25f,
    -1.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.0f, 0.5f,

    // Base Face 8
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
    -0.5f, 0.0f, -0.866f,  0.0f, -1.0f, 0.0f,        0.25f, 0.067f,
    -0.866f, 0.0f, -0.5f,  0.0f, -1.0f, 0.0f,        0.067f, 0.25f,

    // Base Face 9
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
     0.0f, 0.0f, -1.0f,    0.0f, -1.0f, 0.0f,        0.5f, 0.0f,
    -0.5f, 0.0f, -0.866f,  0.0f, -1.0f, 0.0f,        0.25f, 0.067f,

    // Base Face 10
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
     0.5f, 0.0f, -0.866f,  0.0f, -1.0f, 0.0f,        0.75f, 0.067f,
     0.0f, 0.0f, -1.0f,    0.0f, -1.0f, 0.0f,        0.5f, 0.0f,

    // Base Face 11
     0.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        0.5f, 0.5f,
     1.0f, 0.0f, 0.0f,     0.0f, -1.0f, 0.0f,        1.0f, 0.5f,
     0.5f, 0.0f, -0.866f,  0.0f, -1.0f, 0.0f,        0.75f, 0.067f,
};



    // Initialize scene objects with their positions and bounding boxes
    SceneObject cube1;
    cube1.position = glm::vec3(-4.0f, -1.0f, 0.0f);
    cube1.boundingBox = createBoundingBox(cube1.position, glm::vec3(-1.2f, -1.2f, -1.2f), glm::vec3(1.2f, 1.2f, 1.2f));
    sceneObjects.push_back(cube1);

    // Cube 2 (smaller cube, different position)
    SceneObject cube2;
    cube2.position = glm::vec3(0.0f, -0.5f, -3.0f);
    cube2.boundingBox = createBoundingBox(cube2.position, glm::vec3(-0.85f, -0.85f, -0.85f), glm::vec3(0.85f, 0.85f, 0.85f));
    sceneObjects.push_back(cube2);

    // Cube 3 (larger cube, different position - this will rotate)
    SceneObject cube3;
    cube3.position = glm::vec3(1.0f, 5.0f, 2.0f);
    cube3.boundingBox = createBoundingBox(
    cube3.position,
    glm::vec3(-3.54f, -2.5f, -3.54f),
    glm::vec3(3.54f, 2.5f, 3.54f)
);

    sceneObjects.push_back(cube3);

    SceneObject pyramid;
    pyramid.position = glm::vec3(-1.0f, -1.5f, 0.0f);
    pyramid.boundingBox = createBoundingBox(pyramid.position, glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(1.0f, 1.5f, 1.0f));
    sceneObjects.push_back(pyramid);

    SceneObject tetrahedron;
    tetrahedron.position = glm::vec3(2.5f, -1.0f, 0.0f);
    tetrahedron.boundingBox = createBoundingBox(tetrahedron.position, glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    sceneObjects.push_back(tetrahedron);

    SceneObject ground;
    ground.position = glm::vec3(0.0f, -1.0f, 0.0f);
    ground.boundingBox = createBoundingBox(ground.position, glm::vec3(-10.0f, -1.0f, -10.0f), glm::vec3(10.0f, 0.0f, 10.0f));
    sceneObjects.push_back(ground);

        // WALL 1 - Back wall
    SceneObject wallBack;
    wallBack.position = glm::vec3(0.0f, -2.0f, -10.0f);
    wallBack.boundingBox = createBoundingBox(wallBack.position, glm::vec3(-10.0f, 0.0f, -0.1f), glm::vec3(10.0f, 2.0f, 0.1f));
    sceneObjects.push_back(wallBack);

    // WALL 2 - Front wall
    SceneObject wallFront;
    wallFront.position = glm::vec3(0.0f, -2.0f, 10.0f);
    wallFront.boundingBox = createBoundingBox(wallFront.position, glm::vec3(-10.0f, 0.0f, -0.1f), glm::vec3(10.0f, 2.0f, 0.1f));
    sceneObjects.push_back(wallFront);

    // WALL 3 - Left wall
    SceneObject wallLeft;
    wallLeft.position = glm::vec3(-10.0f, -2.0f, 0.0f);
    wallLeft.boundingBox = createBoundingBox(wallLeft.position, glm::vec3(-0.1f, 0.0f, -10.0f), glm::vec3(0.1f, 2.0f, 10.0f));
    sceneObjects.push_back(wallLeft);

    // WALL 4 - Right wall
    SceneObject wallRight;
    wallRight.position = glm::vec3(10.0f, -2.0f, 0.0f);
    wallRight.boundingBox = createBoundingBox(wallRight.position, glm::vec3(-0.1f, 0.0f, -10.0f), glm::vec3(0.1f, 2.0f, 10.0f));
    sceneObjects.push_back(wallRight);

    // CONE example
    SceneObject cone;
    cone.position = glm::vec3(6.5f, -1.5f, -1.0f);  // base center position on ground

    // Bounding box: from base bottom to tip top
    // Min corner: base center - radius on x and z, and y is base y (0.0)
    // Max corner: base center + radius on x and z, and y is base y + height (4.0)
   cone.boundingBox = createBoundingBox(
    cone.position,                      // center at base
    glm::vec3(-1.5f, -4.0f, -1.5f),    // min corner: base radius on x,z, base y
    glm::vec3(1.5f, 4.0f, 1.5f)       // max corner: same radius x,z, tip y = 4
);



    sceneObjects.push_back(cone);


    // --- Setup VAOs and VBOs ---

    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);


        // === WALL ===
    unsigned int wallVAO, wallVBO;
    glGenVertexArrays(1, &wallVAO);
    glGenBuffers(1, &wallVBO);

    glBindVertexArray(wallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, wallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    // Position attribute (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (nx, ny, nz)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute (u, v)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // === PYRAMID ===
    unsigned int pyramidVAO, pyramidVBO;
    glGenVertexArrays(1, &pyramidVAO);
    glGenBuffers(1, &pyramidVBO);

    glBindVertexArray(pyramidVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // === TETRAHEDRON ===
    unsigned int tetraVAO, tetraVBO;
    glGenVertexArrays(1, &tetraVAO);
    glGenBuffers(1, &tetraVBO);

    glBindVertexArray(tetraVAO);
    glBindBuffer(GL_ARRAY_BUFFER, tetraVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tetrahedronVertices), tetrahedronVertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // === GROUND ===
    unsigned int groundVAO, groundVBO;
    glGenVertexArrays(1, &groundVAO);
    glGenBuffers(1, &groundVBO);

    glBindVertexArray(groundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertices), groundVertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // CONE

    unsigned int coneVAO, coneVBO;
    glGenVertexArrays(1, &coneVAO);
    glGenBuffers(1, &coneVBO);

    glBindVertexArray(coneVAO);
    glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coneVertices), coneVertices, GL_STATIC_DRAW);

    // Position attribute (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute (location = 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // === Unbind VAO ===
    glBindVertexArray(0);




    // TEXTURES 

    // First texture (already done)
    unsigned int cubeTexture;
    glGenTextures(1, &cubeTexture);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);

    // Set texture wrapping/filtering options for texture 1
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image for texture 1
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("texture.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 1" << std::endl;
    }
    stbi_image_free(data);

    // **Second texture**
    unsigned int cubeTexture2;
    glGenTextures(1, &cubeTexture2);
    glBindTexture(GL_TEXTURE_2D, cubeTexture2);

    // Set texture wrapping/filtering options for texture 2
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image for texture 2
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("nuts.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    stbi_image_free(data);


    // Flip textures on Y axis (do this once)
    stbi_set_flip_vertically_on_load(true);

    // === Pyramid Texture ===
    unsigned int pyramidTexture;
    glGenTextures(1, &pyramidTexture);
    glBindTexture(GL_TEXTURE_2D, pyramidTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("texture2.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load pyramid texture" << std::endl;
    }
    stbi_image_free(data);

    // === Tetrahedron Texture ===
    unsigned int tetraTexture;
    glGenTextures(1, &tetraTexture);
    glBindTexture(GL_TEXTURE_2D, tetraTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("texture3.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load tetrahedron texture" << std::endl;
    }
    stbi_image_free(data);

    // === Ground Texture ===
    unsigned int groundTexture;
    glGenTextures(1, &groundTexture);
    glBindTexture(GL_TEXTURE_2D, groundTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("texture4.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load ground texture" << std::endl;
    }
    stbi_image_free(data);

    // === Wall Texture ===
    unsigned int wallTexture;
    glGenTextures(1, &wallTexture);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("texture5.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load wall texture" << std::endl;
    }
    stbi_image_free(data);


    // === Cone Texture ===
    unsigned int ConeTexture;
    glGenTextures(1, &ConeTexture);
    glBindTexture(GL_TEXTURE_2D, ConeTexture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("baklava.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load wall texture" << std::endl;
    }
    stbi_image_free(data);






    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Per-frame timing
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear depth buffer 

        shader.use();

        // View/projection matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)SCR_WIDTH/(float)SCR_HEIGHT, 0.1f, 100.0f);

        glm::mat4 view = camera.GetViewMatrix();

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        shader.use();

        // Material 0 - Shiny Gold
        shader.setVec3("materials[0].ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
        shader.setVec3("materials[0].diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
        shader.setVec3("materials[0].specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
        shader.setFloat("materials[0].shininess", 32.0f);

        // Material 1 - Red Plastic
        shader.setVec3("materials[1].ambient", glm::vec3(0.3f, 0.0f, 0.0f));
        shader.setVec3("materials[1].diffuse", glm::vec3(0.6f, 0.0f, 0.0f));
        shader.setVec3("materials[1].specular", glm::vec3(0.8f, 0.6f, 0.6f));
        shader.setFloat("materials[1].shininess", 32.0f);

        // Material 2 - Emerald
        shader.setVec3("materials[2].ambient", glm::vec3(0.415f, 0.4745f, 0.415f));
        shader.setVec3("materials[2].diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
        shader.setVec3("materials[2].specular", glm::vec3(0.633f, 0.727811f, 0.633f));
        shader.setFloat("materials[2].shininess", 76.8f);

        // Material 3 - Cyan Rubber
        shader.setVec3("materials[3].ambient", glm::vec3(0.1f, 0.15f, 0.15f));
        shader.setVec3("materials[3].diffuse", glm::vec3(0.4f, 0.5f, 0.5f));
        shader.setVec3("materials[3].specular", glm::vec3(0.04f, 0.7f, 0.7f));
        shader.setFloat("materials[3].shininess", 10.0f);

        // Material 4 - Bronze
        shader.setVec3("materials[4].ambient", glm::vec3(0.2125f, 0.1275f, 0.054f));
        shader.setVec3("materials[4].diffuse", glm::vec3(0.714f, 0.4284f, 0.18144f));
        shader.setVec3("materials[4].specular", glm::vec3(0.393548f, 0.271906f, 0.166721f));
        shader.setFloat("materials[4].shininess", 85.6f);

        // Material 5 - Obsidian
        shader.setVec3("materials[5].ambient", glm::vec3(0.05375f, 0.05f, 0.06625f));
        shader.setVec3("materials[5].diffuse", glm::vec3(0.18275f, 0.17f, 0.22525f));
        shader.setVec3("materials[5].specular", glm::vec3(0.332741f, 0.328634f, 0.346435f));
        shader.setFloat("materials[5].shininess", 38.4f);

        // Material 6 - Jade
        shader.setVec3("materials[6].ambient", glm::vec3(0.135f, 0.2225f, 0.1575f));
        shader.setVec3("materials[6].diffuse", glm::vec3(0.54f, 0.89f, 0.63f));
        shader.setVec3("materials[6].specular", glm::vec3(0.316228f, 0.316228f, 0.316228f));
        shader.setFloat("materials[6].shininess", 12.8f);

        // Material 7 - Chrome
        shader.setVec3("materials[7].ambient", glm::vec3(0.25f, 0.25f, 0.25f));
        shader.setVec3("materials[7].diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        shader.setVec3("materials[7].specular", glm::vec3(0.774597f, 0.774597f, 0.774597f));
        shader.setFloat("materials[7].shininess", 76.8f);


        // Directional Light
        shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // Point Light
        shader.setVec3("pointLight.position", glm::vec3(-1.0f, 2.0f, 0.0f));
        shader.setVec3("pointLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
        shader.setVec3("pointLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
        shader.setVec3("pointLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setFloat("pointLight.constant", 1.0f);
        shader.setFloat("pointLight.linear", 0.09f);
        shader.setFloat("pointLight.quadratic", 0.032f);

        // Spotlight (Flashlight style, follows the camera)

        shader.setVec3("spotLight.position", camera.Position); // or glm::vec3(0.0f, 3.0f, 3.0f);
        shader.setVec3("spotLight.direction", camera.Front);   // or glm::vec3(0.0f, -1.0f, -1.0f);

        shader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
        shader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        // Attenuation (same as point light)
        shader.setFloat("spotLight.constant", 1.0f);
        shader.setFloat("spotLight.linear", 0.09f);
        shader.setFloat("spotLight.quadratic", 0.032f);

        // Cutoff angles
        shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

        shader.setVec3("viewPos", camera.Position);




        // Bind all textures to their respective texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, cubeTexture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, cubeTexture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, pyramidTexture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, tetraTexture);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, groundTexture);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, ConeTexture);

        // Set the texture sampler uniforms once 
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);
        shader.setInt("texture3", 2);
        shader.setInt("texture4", 3);
        shader.setInt("texture5", 4);
        shader.setInt("texture6", 5);
        shader.setInt("texture7", 6);
    
        // Draw cube at (-4,0,0) - 
        shader.setInt("activeMaterial", 3); // cyan 
        shader.setInt("activeTexture", 0); // uses texture3 (unit 2)
        shader.setBool("useBlending", true);
        shader.setFloat("blendFactor",0.5);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[0].position);
        shader.setMat4("model", model);
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

         // Draw cube2 (smaller cube)
        shader.setInt("activeMaterial", 0); // Shiny Gold
        shader.setInt("activeTexture", 0); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        // shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f)); // Green
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[1].position); // Note index may vary based on your sceneObjects order
        model = glm::scale(model, glm::vec3(0.5f)); // Scale down to half size
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw cube3 (larger rotating cube)
        shader.setInt("activeMaterial", 6); // jade
        shader.setInt("activeTexture", 0); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        // shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f)); // Blue
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[2].position);
        model = glm::scale(model, glm::vec3(2.5f)); // Scale up to 1.5x size
        // Add rotation based on time
        float rotationAngle = (float)glfwGetTime() * 50.0f; // Rotate 50 degrees per second
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y-axis
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // Draw pyramid at (-1,0,0) - 
        shader.setInt("activeMaterial", 4);  // bronze
        shader.setInt("activeTexture", 1); // uses texture3
        shader.setBool("useBlending", false);
        //shader.setVec3("objectColor", glm::vec3(0.0f, 1.0f, 0.0f)); // Green
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[3].position);
        shader.setMat4("model", model);
        glBindVertexArray(pyramidVAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

        // Draw tetrahedron at (2.5,0,0) - 
        shader.setInt("activeMaterial", 1); // red
        shader.setInt("activeTexture", 3); // uses texture3
        shader.setBool("useBlending", false);
        //shader.setVec3("objectColor", glm::vec3(0.0f, 0.0f, 1.0f)); // Blue
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[4].position);
        shader.setMat4("model", model);
        glBindVertexArray(tetraVAO);
        glDrawArrays(GL_TRIANGLES, 0, 12);

        // Draw ground at (0,-1,0) - 
        shader.setInt("activeMaterial", 2); // obsidian 
        shader.setInt("activeTexture", 5); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        //shader.setVec3("objectColor", glm::vec3(0.5f, 0.0f, 0.5f)); // Gray
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[5].position);
        shader.setMat4("model", model);
        glBindVertexArray(groundVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // North wall
        shader.setInt("activeMaterial", 4); // Bronze
        shader.setInt("activeTexture",5); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        //shader.setVec3("objectColor", glm::vec3(0.3f, 0.2f, 0.5f)); // Gray
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[6].position); // Adjust index based on your sceneObjects order
        model = glm::scale(model, glm::vec3(10.0f, 5.0f, 0.01f)); // Scale to match ground size
        shader.setMat4("model", model);
        glBindVertexArray(wallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // South wall
        shader.setInt("activeMaterial", 4); // Bronze
        shader.setInt("activeTexture", 5); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        // shader.setVec3("objectColor", glm::vec3(0.5f, 0.8f, 0.5f)); // Gra
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[7].position);
        model = glm::scale(model, glm::vec3(10.0f, 5.0f, 0.01f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // East wall
        shader.setInt("activeMaterial", 4); // Bronze
        shader.setInt("activeTexture",5); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        // shader.setVec3("objectColor", glm::vec3(0.5f, 0.0f, 0.3f)); // Gray
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[8].position);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate to face X direction
        model = glm::scale(model, glm::vec3(11.0f, 5.0f, 0.01f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // West wall
        shader.setInt("activeMaterial", 4); // Bronze
        shader.setInt("activeTexture", 5); // uses texture3 (unit 2)
        shader.setBool("useBlending", false);
        // shader.setVec3("objectColor", glm::vec3(0.2f, 0.0f, 0.5f)); // Gray   
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[9].position);
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate to face X direction
        model = glm::scale(model, glm::vec3(10.0f, 5.0f, 0.01f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Draw cone at its position - example material and texture
        shader.setInt("activeMaterial", 6);  // chrome
        shader.setInt("activeTexture", 6);   
        shader.setBool("useBlending", false);
        model = glm::mat4(1.0f);
        model = glm::translate(model, sceneObjects[10].position);
        model = glm::scale(model, glm::vec3(1.8f)); // Scale up to 1.5x size
        shader.setMat4("model", model);
        glBindVertexArray(coneVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);  



        glBindVertexArray(0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup VBOs and VAOs
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteVertexArrays(1, &pyramidVAO);
    glDeleteBuffers(1, &pyramidVBO);
    glDeleteVertexArrays(1, &tetraVAO);
    glDeleteBuffers(1, &tetraVBO);
    glDeleteVertexArrays(1, &groundVAO);
    glDeleteBuffers(1, &groundVBO);
    glDeleteVertexArrays(1, &wallVAO);
    glDeleteBuffers(1, &wallVBO);

    glfwTerminate();
    return 0;
}

// Handles keyboard input every frame for movement and exiting the application
void processInput(GLFWwindow* window)
{
    // Close the window when Escape is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Gather all scene object bounding boxes for collision detection
    std::vector<AABB> obstacles;
    for (const auto& obj : sceneObjects) {
        obstacles.push_back(obj.boundingBox);
    }

    // Camera movement input handling (with collision-aware movement)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime, obstacles);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime, obstacles);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime, obstacles);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime, obstacles);
}

// GLFW callback function: called whenever the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Adjust the viewport to the new window size
    glViewport(0, 0, width, height);
}

// GLFW callback function: called whenever the mouse moves
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = (float)xposIn;
    float ypos = (float)yposIn;

    // On the first frame, initialize last mouse position
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Calculate mouse movement offset
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Y is inverted (top to bottom)

    // Update last positions
    lastX = xpos;
    lastY = ypos;

    // Update the camera's direction based on mouse movement
    camera.ProcessMouseMovement(xoffset, yoffset);
}

// GLFW callback function: called when the mouse scroll wheel is used
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Zoom in/out by adjusting the camera's FOV
    camera.ProcessMouseScroll((float)yoffset);
}
