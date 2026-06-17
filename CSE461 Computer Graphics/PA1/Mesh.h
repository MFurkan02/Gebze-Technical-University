#ifndef MESH_H
#define MESH_H

#include "hittable.h" // Include the base Hittable class for ray-object intersection logic
#include "Vector3.h" // Include the Vector3 class for 3D vector operations
#include <vector> // Include vector for storing vertices and faces

// Class to represent a 3D mesh object
class Mesh : public Hittable {
public:
    // Constructor to initialize the mesh with vertices and faces
    Mesh(const std::vector<Vector3>& vertices, const std::vector<std::vector<int>>& faces) 
        : vertices(vertices), faces(faces) {}

    // Override the hit method to check for ray intersections with the mesh
    bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const override {
        bool hit_anything = false; // Flag to indicate if the ray hits any face
        double closest_so_far = ray_tmax; // Track the closest intersection point

        // Iterate through each face of the mesh
        for (const auto& face : faces) {
            // Get the vertices of the current face
            const Vector3& v0 = vertices[face[0]];
            const Vector3& v1 = vertices[face[1]];
            const Vector3& v2 = vertices[face[2]];

            // Compute the edges of the triangle
            Vector3 e1 = v1 - v0;
            Vector3 e2 = v2 - v0;

            // Compute the determinant
            Vector3 h = cross(r.get_direction(), e2);
            double a = dot(e1, h);

            // If the determinant is near zero, the ray is parallel to the triangle
            if (a > -0.000001 && a < 0.000001)
                continue;

            double f = 1 / a; // Compute the inverse determinant
            Vector3 s = r.get_origin() - v0; // Vector from ray origin to the first vertex
            double u = f * dot(s, h); // Compute the barycentric coordinate u

            // Check if the intersection lies outside the triangle
            if (u < 0 || u > 1)
                continue;

            Vector3 q = cross(s, e1); // Compute the cross product for v
            double v = f * dot(r.get_direction(), q); // Compute the barycentric coordinate v

            // Check if the intersection lies outside the triangle
            if (v < 0 || u + v > 1)
                continue;

            double t = f * dot(e2, q); // Compute the ray parameter t

            // Check if the intersection is within the valid range
            if (t < ray_tmin || t > closest_so_far)
                continue;

            // Update the closest intersection point
            closest_so_far = t;
            hit_anything = true;

            // Record hit information
            rec.t = t; // Ray parameter at the intersection
            rec.p = r.at(t); // Intersection point
            Vector3 outward_normal = unit_vector(cross(e1, e2)); // Compute the surface normal
            rec.set_face_normal(r, outward_normal); // Set the correct normal direction
            rec.hit_surface_p = this; // Store a pointer to the hit surface
        }

        return hit_anything; // Return whether any face was hit
    }

private:
    std::vector<Vector3> vertices; // List of vertices in the mesh
    std::vector<std::vector<int>> faces; // List of faces (each face is a list of vertex indices)
};

#endif