#ifndef LIGHT_H
#define LIGHT_H

#include "Color.h" // Include the Color class for color representation
#include "Ray.h" // Include the Ray class for ray operations
#include "Hittable.h" // Include the Hittable class for ray-object intersection details

// Abstract base class for all light sources
class Light {
public:

    // Pure virtual method to get the direction of the light at a given point
    // Must be implemented by all derived light classes
    virtual Vector3 dir(const Vector3& point) = 0;

    // Pure virtual method to calculate the illumination at a given point
    // Must be implemented by all derived light classes
    virtual Color illuminate(const Ray& r, const Hit_record& rec) = 0;

};

#endif