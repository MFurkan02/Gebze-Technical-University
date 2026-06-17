#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Color.h" // Include the Color class for color representation
#include "Light.h" // Include the base Light class for light source functionality
#include "Vector3.h" // Include the Vector3 class for 3D vector operations
#include <cmath> // Include cmath for mathematical operations
#include <iostream> // Include iostream for debugging or logging

// Class to represent a point light source
class PointLight : public Light {

    private:
        Color intensity; // Intensity of the light (color and strength)
        Vector3 point; // Position of the light in 3D space

    public:

        // Constructor to initialize the point light with intensity and position
        PointLight(const Color& i, const Vector3& p) : intensity(i), point(p) {}

        // Static method to create a PointLight object from intensity values (assumed to be in the range [0, 1000])
        static PointLight fromIntensity(const Vector3& p, const Vector3& intensity) {
            // Normalize the intensity values to the range [0, 1]
            return PointLight(Color(intensity.x()/1000.0, intensity.y()/1000.0, intensity.z()/1000.0), p);
        }

        // Override the dir method to compute the direction of the light from a given point
        Vector3 dir(const Vector3& pointv) override {
            return point - pointv; // Direction is the vector from the given point to the light source
        }

        // Override the illuminate method to calculate the light contribution at a given point
        Color illuminate(const Ray& r, const Hit_record& rec) override {

            Color E = Color(0, 0, 0); // Initialize the emitted light intensity to zero
            Color k = Color(0, 0, 0); // Initialize the material reflectance to zero

            Vector3 hit_point = rec.p; // Get the intersection point of the ray with the surface

            Vector3 p_min_x = point - hit_point; // Vector from the hit point to the light source

            auto p_min_x_len = p_min_x.length(); // Compute the distance between the hit point and the light source

            Vector3 light_direction = p_min_x / p_min_x_len; // Normalize the light direction vector

            Vector3 n = rec.normal; // Get the surface normal at the hit point

            // Compute the emitted light intensity if the distance is non-zero
            if (p_min_x_len != 0)
                E = std::max(0.0, dot(n, light_direction)) * intensity / pow(p_min_x_len, 2); // Apply the inverse square law

            Vector3 v = -r.get_direction(); // Compute the view direction (opposite to the ray direction)

            // Evaluate the material's reflectance if the hit surface and its material are valid
            if (rec.hit_surface_p && rec.hit_surface_p->material)
                k = rec.hit_surface_p->material->evaluate(light_direction, v, n); // Compute the material's response to the light

            return E * k; // Return the total light contribution (emitted light multiplied by material reflectance)
        }

};

#endif