#ifndef TRIANGULARLIGHT_H
#define TRIANGULARLIGHT_H

#include "Light.h" // Include the base Light class for light source functionality
#include "Vector3.h" // Include the Vector3 class for 3D vector operations

// Class to represent a triangular light source
class TriangularLight: public Light {
  
    private:
        Color intensity; // Intensity of the light (color and strength)
        Vector3 direction; // Direction of the light (normal to the triangle)

    public:

        // Constructor to initialize the triangular light with intensity and direction
        TriangularLight(const Color& ins, const Vector3& dir) : intensity(ins), direction(dir) {}

        // Static method to create a TriangularLight object from triangle vertices and intensity
        static TriangularLight fromTriangle(Vector3 vertex1, Vector3 vertex2, Vector3 vertex3, Vector3 intensity) {
            // Compute the direction of the light as the cross product of two edges of the triangle
            Vector3 direction = cross(vertex1 - vertex2, vertex1 - vertex3);
            // Normalize the intensity values to the range [0, 1]
            Vector3 intensityFixed = Vector3(intensity.x() / 1000.0, intensity.y() / 1000.0, intensity.z() / 1000.0);
    
            return TriangularLight(intensityFixed, direction);
        }
    
        // Override the dir method to return the direction of the light
        Vector3 dir(const Vector3& point) override {
            return direction; // The direction is constant for a triangular light
        }
    
        // Override the illuminate method to calculate the light contribution at a given point
        Color illuminate(const Ray& r, const Hit_record& rec) override {
            Color E = Color(0, 0, 0); // Initialize the emitted light intensity to zero
            Color k = Color(0, 0, 0); // Initialize the material reflectance to zero
    
            Vector3 n = rec.normal; // Get the surface normal at the hit point
    
            // Compute the emitted light intensity based on the angle between the normal and light direction
            E = std::max(0.0, dot(n, direction)) * intensity;

            // If the light direction is opposite to the surface normal, return no light contribution
            if (dot(n, direction) < 0)
                return Color(0, 0, 0);
    
            Vector3 v = -r.get_direction(); // Compute the view direction (opposite to the ray direction)
    
            // Evaluate the material's reflectance if the hit surface and its material are valid
            if (rec.hit_surface_p && rec.hit_surface_p->material)
                k = rec.hit_surface_p->material->evaluate(direction, v, n);
    
            return E * k; // Return the total light contribution (emitted light multiplied by material reflectance)
        }
};

#endif