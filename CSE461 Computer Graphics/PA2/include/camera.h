#ifndef CAMERA_H
#define CAMERA_H

// Include OpenGL and GLM dependencies
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// Enumeration to define movement directions for the camera
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Axis-Aligned Bounding Box (AABB) used for basic collision detection
struct AABB {
    glm::vec3 min; // Minimum corner (x, y, z)
    glm::vec3 max; // Maximum corner (x, y, z)

    // Check if a point is within this AABB
    bool contains(const glm::vec3& point) const {
        return point.x >= min.x && point.x <= max.x &&
               point.y >= min.y && point.y <= max.y &&
               point.z >= min.z && point.z <= max.z;
    }
};

// Main Camera class
class Camera {
public:
    // Camera attributes
    glm::vec3 Position;  // Current position of the camera in world space
    glm::vec3 Front;     // Direction the camera is facing
    glm::vec3 Up;        // Up direction relative to the camera's orientation
    glm::vec3 Right;     // Right direction relative to the camera
    glm::vec3 WorldUp;   // Global up direction (usually (0, 1, 0))

    // Euler angles (for orientation)
    float Yaw;           // Left-right rotation
    float Pitch;         // Up-down rotation

    // Camera control settings
    float MovementSpeed;     // Units per second
    float MouseSensitivity;  // Mouse movement multiplier
    float Zoom;              // Field of view angle for perspective projection

    // Constructor with default values
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
           glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw          = -90.0f,
           float pitch        = 0.0f)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          MovementSpeed(2.5f),
          MouseSensitivity(0.01f),
          Zoom(45.0f)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors(); // Calculate Right and Up vectors based on initial orientation
    }

    // Returns the camera view matrix using the LookAt transformation
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // Keyboard movement processing with collision detection
    void ProcessKeyboard(Camera_Movement direction, float deltaTime, const std::vector<AABB>& obstacles) {
        float velocity = MovementSpeed * deltaTime; // Frame rate–independent movement

        // Calculate direction vectors only in the XZ plane (no flying up/down)
        glm::vec3 frontXZ = glm::normalize(glm::vec3(Front.x, 0.0f, Front.z));
        glm::vec3 rightXZ = glm::normalize(glm::vec3(Right.x, 0.0f, Right.z));

        glm::vec3 proposedPosition = Position; // Start with current position

        // Modify proposed position based on input
        if (direction == FORWARD)
            proposedPosition += frontXZ * velocity;
        if (direction == BACKWARD)
            proposedPosition -= frontXZ * velocity;
        if (direction == LEFT)
            proposedPosition -= rightXZ * velocity;
        if (direction == RIGHT)
            proposedPosition += rightXZ * velocity;

        proposedPosition.y = Position.y; // Lock height (no flying)

        // Collision check with all obstacles
        bool collided = false;
        for (const auto& obstacle : obstacles) {
            if (obstacle.contains(proposedPosition)) {
                collided = true;
                break;
            }
        }

        // Only move if no collision detected
        if (!collided)
            Position = proposedPosition;
    }

    // Mouse movement processing (used to rotate camera view)
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
        // Apply sensitivity to offsets
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        // Update yaw and pitch
        Yaw   += xoffset;
        Pitch += yoffset;

        // Clamp pitch to prevent gimbal lock (looking straight up/down)
        if (constrainPitch) {
            if (Pitch > 89.0f)  Pitch = 89.0f;
            if (Pitch < -89.0f) Pitch = -89.0f;
        }

        updateCameraVectors(); // Recalculate Front, Right, and Up vectors
    }

    // Mouse scroll processing (used to zoom in/out)
    void ProcessMouseScroll(float yoffset) {
        Zoom -= yoffset * 0.5f; // Change zoom based on scroll
        if (Zoom < 1.0f)  Zoom = 1.0f;
        if (Zoom > 45.0f) Zoom = 45.0f;
    }

private:
    // Updates camera vectors based on current yaw and pitch angles
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front); // New look direction

        // Recalculate Right and Up vectors
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};

#endif
