#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h" // Include the Ray class for ray operations
#include "material.h" // Include the Material class for material properties
#include <memory> // Include memory for smart pointers

// Forward declaration of the Hittable class
class Hittable;

// Class to store information about a ray-object intersection
class Hit_record {
  public:
    Vector3 p; // Point of intersection
    Vector3 normal; // Surface normal at the intersection point
    double t; // Ray parameter at the intersection point

    const Hittable* hit_surface_p; // Pointer to the hit surface

    bool front_face; // Indicates whether the ray hit the front face of the surface

    // Method to set the surface normal direction based on the ray's direction
    void set_face_normal(const Ray& r, const Vector3& outward_normal) {
        // Determine if the ray is hitting the front face
        front_face = dot(r.get_direction(), outward_normal) < 0;
        // Set the normal to point against the ray if hitting the front face, otherwise invert it
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// Abstract base class for all hittable objects
class Hittable {
  public:
    virtual ~Hittable() = default; // Virtual destructor for proper cleanup of derived classes

    // Pure virtual method to check if a ray hits the object
    // Must be implemented by all derived classes
    virtual bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const = 0;

    std::shared_ptr<Material> material; // Pointer to the material of the hittable object
};

#endif