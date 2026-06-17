#include "Light.h" // Include the base Light class for light source functionality
#include "Vector3.h" // Include the Vector3 class for 3D vector operations
#include <iostream> // Include iostream for debugging or logging

// Class to represent an ambient light source
class AmbientLight: public Light {
  
    private:
        Color intensity; // Intensity of the ambient light (color and strength)

    public:

        // Constructor to initialize the ambient light with a given intensity
        AmbientLight(const Color& intens) : intensity(intens) {}

        // Static method to create an AmbientLight object from an RGB color vector
        static AmbientLight from_rgb(const Vector3& rgb) {
            // Normalize the RGB values (0-255) to the range [0, 1]
            return AmbientLight(Color(rgb.x()/255.0, rgb.y()/255.0, rgb.z()/255.0));
        }

        // Override the dir method from the Light base class
        // Ambient light has no direction, so it always returns a zero vector
        Vector3 dir(const Vector3& point) override {
            return Vector3(0, 0, 0); // No direction for ambient light
        }

        // Override the illuminate method from the Light base class
        // Calculates the ambient light contribution to the illumination
        Color illuminate(const Ray& r, const Hit_record& rec) override {
            // Retrieve the ambient color property of the hit surface's material
            Color ambient = rec.hit_surface_p->material->ambient;
            // Multiply the ambient light intensity with the material's ambient color
            return intensity * ambient;
        }
};