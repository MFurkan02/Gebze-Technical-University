#ifndef Vector3_H
#define Vector3_H

#include <cmath> // Include cmath for mathematical operations
#include <iostream> // Include iostream for output operations

// Class to represent a 3D vector
class Vector3 {
  private:
    double x_, y_, z_; // Components of the vector

  public:
    // Default constructor initializes the vector to (0, 0, 0)
    Vector3() : x_(0), y_(0), z_(0) {}

    // Constructor to initialize the vector with specific values
    Vector3(double x, double y, double z) : x_(x), y_(y), z_(z) {}

    // Getter methods for the vector components
    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }

    // Setter methods for the vector components
    void set_x(double x) { x_ = x; }
    void set_y(double y) { y_ = y; }
    void set_z(double z) { z_ = z; }

    // Negation operator to return the negative of the vector
    Vector3 operator-() const {
        return Vector3(-x_, -y_, -z_);
    }

    // Access operator to get a specific component by index
    double operator[](int i) const {
        if (i == 0) return x_;
        if (i == 1) return y_;
        return z_;
    }

    // Access operator to modify a specific component by index
    double& operator[](int i) {
        if (i == 0) return x_;
        if (i == 1) return y_;
        return z_;
    }

    // Compound addition operator
    Vector3& operator+=(const Vector3& v) {
        x_ += v.x_;
        y_ += v.y_;
        z_ += v.z_;
        return *this;
    }

    // Compound multiplication operator (scalar multiplication)
    Vector3& operator*=(double t) {
        x_ *= t;
        y_ *= t;
        z_ *= t;
        return *this;
    }

    // Compound division operator (scalar division)
    Vector3& operator/=(double t) {
        return *this *= 1/t;
    }

    // Method to compute the length (magnitude) of the vector
    double length() const {
        return std::sqrt(length_squared());
    }

    // Method to compute the squared length of the vector
    double length_squared() const {
        return x_ * x_ + y_ * y_ + z_ * z_;
    }

    // Friend functions for vector operations
    friend std::ostream& operator<<(std::ostream& out, const Vector3& v);
    friend Vector3 operator+(const Vector3& u, const Vector3& v);
    friend Vector3 operator-(const Vector3& u, const Vector3& v);
    friend Vector3 operator*(const Vector3& u, const Vector3& v);
    friend Vector3 operator*(double t, const Vector3& v);
    friend Vector3 operator*(const Vector3& v, double t);
    friend Vector3 operator/(const Vector3& v, double t);
    friend double dot(const Vector3& u, const Vector3& v);
    friend Vector3 cross(const Vector3& u, const Vector3& v);
    friend Vector3 unit_vector(const Vector3& v);
    friend Vector3 reflect(const Vector3& incoming_direction, const Vector3& surface_normal);
};

// Utility Functions

// Output operator to print the vector components
inline std::ostream& operator<<(std::ostream& out, const Vector3& v) {
    return out << v.x_ << ' ' << v.y_ << ' ' << v.z_;
}

// Addition operator for two vectors
inline Vector3 operator+(const Vector3& u, const Vector3& v) {
    return Vector3(u.x_ + v.x_, u.y_ + v.y_, u.z_ + v.z_);
}

// Subtraction operator for two vectors
inline Vector3 operator-(const Vector3& u, const Vector3& v) {
    return Vector3(u.x_ - v.x_, u.y_ - v.y_, u.z_ - v.z_);
}

// Component-wise multiplication operator for two vectors
inline Vector3 operator*(const Vector3& u, const Vector3& v) {
    return Vector3(u.x_ * v.x_, u.y_ * v.y_, u.z_ * v.z_);
}

// Scalar multiplication operator
inline Vector3 operator*(double t, const Vector3& v) {
    return Vector3(t * v.x_, t * v.y_, t * v.z_);
}

// Scalar multiplication operator (reversed order)
inline Vector3 operator*(const Vector3& v, double t) {
    return t * v;
}

// Scalar division operator
inline Vector3 operator/(const Vector3& v, double t) {
    return (1 / t) * v;
}

// Dot product of two vectors
inline double dot(const Vector3& u, const Vector3& v) {
    return u.x_ * v.x_ + u.y_ * v.y_ + u.z_ * v.z_;
}

// Cross product of two vectors
inline Vector3 cross(const Vector3& u, const Vector3& v) {
    return Vector3(
        u.y_ * v.z_ - u.z_ * v.y_,
        u.z_ * v.x_ - u.x_ * v.z_,
        u.x_ * v.y_ - u.y_ * v.x_
    );
}

// Normalize a vector to unit length
inline Vector3 unit_vector(const Vector3& v) {
    return v / v.length();
}

// Reflect a vector around a surface normal
inline Vector3 reflect(const Vector3& incoming_direction, const Vector3& surface_normal) {
    return incoming_direction - 2.0 * dot(incoming_direction, surface_normal) * surface_normal;
}

#endif