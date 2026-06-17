#ifndef HITTABLE_OBJS_H
#define HITTABLE_OBJS_H

#include "Hittable.h" // Include the base Hittable class for ray-object intersection logic

#include <memory> // Include memory for smart pointers
#include <vector> // Include vector for storing multiple hittable objects

using std::make_shared; // Use make_shared for creating shared pointers
using std::shared_ptr;  // Use shared_ptr for managing object lifetimes

// Class to represent a collection of hittable objects
class HittableObjs : public Hittable {
  public:
    std::vector<shared_ptr<Hittable>> objects; // Vector to store shared pointers to hittable objects

    // Default constructor (creates an empty collection)
    HittableObjs() {}

    // Constructor to initialize the collection with a single hittable object
    HittableObjs(shared_ptr<Hittable> object) { add(object); }

    // Method to clear all objects from the collection
    void clear() { objects.clear(); }

    // Method to add a hittable object to the collection
    void add(shared_ptr<Hittable> object) {
        objects.push_back(object); // Add the object to the vector
    }

    // Override the hit method to check for ray intersections with any object in the collection
    bool hit(const Ray& r, double ray_tmin, double ray_tmax, Hit_record& rec) const override {
        Hit_record temp_rec; // Temporary record to store intersection details
        bool hit_anything = false; // Flag to indicate if any object was hit
        auto closest_so_far = ray_tmax; // Track the closest intersection point

        // Iterate through all objects in the collection
        for (const auto& object : objects) {
            // Check if the ray hits the current object
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true; // Update the flag if an intersection is found
                closest_so_far = temp_rec.t; // Update the closest intersection distance
                rec = temp_rec; // Update the hit record with the closest intersection details
            }
        }

        return hit_anything; // Return whether any object was hit
    }
};

#endif