#ifndef COLOR_H
#define COLOR_H

#include "Vector3.h" // Include the Vector3 class for 3D vector operations

#include <iostream> // Include iostream for output operations

// Define Color as an alias for Vector3 (to represent RGB color values)
using Color = Vector3;

// Function to write a color to an output stream in the [0, 255] range
void write_color(std::ostream& out, const Color& pixel_color) {
    auto r = pixel_color.x(); // Extract the red component
    auto g = pixel_color.y(); // Extract the green component
    auto b = pixel_color.z(); // Extract the blue component

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r); // Scale and convert red to an integer
    int gbyte = int(255.999 * g); // Scale and convert green to an integer
    int bbyte = int(255.999 * b); // Scale and convert blue to an integer

    // Write out the pixel color components in the format "R G B".
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

// Function to clamp the color components to the [0, 1] range
void fix_color(Color& pixel_color) {
    // Clamp the red component
    if(pixel_color.x() > 1)
        pixel_color.set_x(1); // Set to 1 if greater than 1
    else if(pixel_color.x() < 0)
        pixel_color.set_x(0); // Set to 0 if less than 0

    // Clamp the green component
    if(pixel_color.y() > 1)
        pixel_color.set_y(1); // Set to 1 if greater than 1
    else if(pixel_color.y() < 0)
        pixel_color.set_y(0); // Set to 0 if less than 0

    // Clamp the blue component
    if(pixel_color.z() > 1)
        pixel_color.set_z(1); // Set to 1 if greater than 1
    else if(pixel_color.z() < 0)
        pixel_color.set_z(0); // Set to 0 if less than 0
}

#endif