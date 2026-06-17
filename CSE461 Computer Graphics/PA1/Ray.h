#ifndef RAY_H
#define RAY_H

#include "Vector3.h" // Include the Vector3 class for 3D vector operations
#include <cmath> // Include cmath for mathematical operations

// Define constants for infinity and pi
const double infinity = std::numeric_limits<double>::infinity(); // Represents positive infinity
const double pi = 3.14159265358979; // Value of pi

// Utility function to convert degrees to radians
inline double degrees_to_radians(double degree) {
    return (degree * pi) / 180.0; // Convert degrees to radians
}

// Class to represent a ray in 3D space
class Ray {
public:
    Vector3 origin; // Origin point of the ray
    Vector3 direction; // Direction vector of the ray

    // Default constructor initializes an empty ray
    Ray() {}

    // Constructor to initialize a ray with a given origin and direction
    Ray(const Vector3& orig, const Vector3& dir) : origin(orig), direction(dir) {}

    /* Alternative constructor with normalization (commented out)
    Ray(const Vector3& origin, const Vector3& direction)  : origin(origin), direction(direction) {
        this->direction.normalize(); // Normalize the direction vector
    }*/

    // Getter method to retrieve the origin of the ray
    const Vector3& get_origin() const {
        return origin;
    }

    // Getter method to retrieve the direction of the ray
    const Vector3& get_direction() const {
        return direction;
    }

    // Method to compute a point along the ray at parameter t
    Vector3 at(double t) const {
        return origin + (direction * t); // Compute the point using the parametric equation of the ray
    }
};

#endif // RAY_H