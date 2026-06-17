#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h" // Include the Vector3 class for 3D vector operations
#include "Ray.h"     // Include the Ray class for ray generation

// Define the Camera class
class Camera {
public:
    Vector3 position; // Camera position in 3D space
    Vector3 gaze;     // Normalized viewing direction (where the camera is looking)
    Vector3 up;       // Normalized up vector (defines the "up" direction for the camera)
    Vector3 right;    // Normalized right vector (orthogonal to gaze and up)

    // Image plane bounds (defines the view frustum in camera space)
    double left, rightPlane, bottom, top;
    double nearDistance; // Distance from the camera to the near clipping plane
    int nx, ny;          // Image resolution (number of pixels in x and y directions)
    Vector3 u;           // Camera's local x-axis (right vector)
    Vector3 v;           // Camera's local y-axis (up vector)
    Vector3 w;           // Camera's local z-axis (negative gaze direction)

    // Default constructor (initializes the camera with default values)
    Camera()
        : position(0.0f, 0.0f, 0.0f), // Default position at the origin
          gaze(0.0f, 0.0f, 1.0f),    // Default gaze along the negative Z-axis
          up(0.0f, 1.0f, 0.0f),      // Default up vector along the Y-axis
          right(1.0f, 0.0f, 0.0f),   // Default right vector along the X-axis
          left(-1.0), rightPlane(1.0), bottom(-1.0), top(1.0), // Default image plane bounds
          nearDistance(1.0), nx(1024), ny(768) // Default near plane distance and resolution
    {
        // Calculate orthonormal basis vectors (u, v, w) for the camera
        w = -unit_vector(gaze); // Negative gaze direction (camera's local z-axis)
        u = unit_vector(cross(w, up)); // Cross product of w and up gives the right vector
        v = unit_vector(cross(w, u)); // Cross product of w and u gives the up vector
    }

    // Constructor with parameters (allows explicit initialization of camera properties)
    Camera(const Vector3& position, const Vector3& gaze, const Vector3& upVector,
           double left, double rightBound, double bottom, double top,
           double nearDistance, int nx, int ny)
        : position(position), // Initialize position
          gaze(gaze),         // Initialize gaze direction
          up(upVector),       // Initialize up vector
          left(left), rightPlane(rightBound), bottom(bottom), top(top), // Initialize image plane bounds
          nearDistance(nearDistance), nx(nx), ny(ny) // Initialize near plane distance and resolution
    {
        // Calculate orthonormal basis vectors (u, v, w) for the camera
        w = -unit_vector(gaze); // Negative gaze direction (camera's local z-axis)
        u = unit_vector(cross(w, up)); // Cross product of w and up gives the right vector
        v = unit_vector(cross(w, u)); // Cross product of w and u gives the up vector
    }

    // Generate a ray from the camera through a specific pixel (commented out for now)
    /*Ray generateRay(float x, float y) const {
        float u = left + (rightPlane - left) * (x + 0.5f) / static_cast<float>(nx); // Calculate horizontal offset
        float v = bottom + (top - bottom) * (y + 0.5f) / static_cast<float>(ny);   // Calculate vertical offset

        Vector3 direction = (gaze * nearDistance) + (right * u) + (up * v); // Compute ray direction
        return Ray(position, direction.normalized()); // Return a normalized ray
    }*/

    // Getter for the horizontal resolution (nx)
    int get_nx(){
        return nx;
    }

    // Getter for the vertical resolution (ny)
    int get_ny(){
        return ny;
    }

    // Getter for the camera position
    const Vector3& get_position() const  { return position; }

    // Getter for the gaze direction
    const Vector3& get_gaze() const  { return gaze; }

    // Getter for the up vector
    const Vector3& get_up() const  { return up; }

    // Getter for the left bound of the image plane
    double get_left() const { 
        return left;
    }

    // Getter for the right bound of the image plane
    double get_rightPlane() const { 
        return rightPlane;
    }

    // Getter for the bottom bound of the image plane
    double get_bottom() const { 
        return bottom;
    }

    // Getter for the top bound of the image plane
    double get_top() const { 
        return top;
    }

    // Getter for the camera's local x-axis (u)
    const Vector3& get_u() const  { return u; }
    
    // Getter for the camera's local y-axis (v)
    const Vector3& get_v() const  { return v; }
    
    // Getter for the camera's local z-axis (w)
    const Vector3& get_w() const  { return w; }

    // Getter for the near plane distance
    double neardistance() const{return nearDistance;}

    // Move the camera along the local x-axis (u)
    void go_u(double amount){position += amount * u;}

    // Move the camera along the local y-axis (v)
    void go_v(double amount){position += amount * v;}

    // Move the camera along the local z-axis (w)
    void go_w(double amount){position += amount * w;}

    // Virtual destructor (default implementation)
    virtual ~Camera() = default;
};

#endif // CAMERA_H