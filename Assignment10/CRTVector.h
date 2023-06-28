#ifndef CRTVECTOR_H
#define CRTVECTOR_H

#include <cfloat>
#include <cmath>
#include "Constants.h"

// Templated class that represents a vector in 3D.
// It's also used to represent normals and points in 3D.
template <typename T>
struct CRTVector {
    // The vector components
    union alignas(4 * alignof(T)) {
        struct {
            T x, y, z;
        };
        T v3[3];
    };

    CRTVector() : x(0), y(0), z(0) {}  // Default constructor initializes all components to zero

    CRTVector(const T p1) : x(p1), y(p1), z(p1) {}  // Constructor that sets all components to a single value

    CRTVector(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) {}  // Constructor that sets individual components

    T& operator[](const int idx) {
        Assert(idx >= 0 && idx < 3);  // Assertion to check the index is within valid range
        return v3[idx];  // Return the reference to the indexed component
    }

    const T& operator[](const int idx) const {
        Assert(idx >= 0 && idx < 3);  // Assertion to check the index is within valid range
        return v3[idx];  // Return the reference to the indexed component (read-only)
    }

    CRTVector<T> operator+(const CRTVector<T>& vec3) const {
        return CRTVector(x + vec3.x, y + vec3.y, z + vec3.z);  // Addition of two vectors
    }

    CRTVector<T>& operator+=(const CRTVector<T>& vec3) {
        for (int i = 0; i < 3; i++) {
            v3[i] += vec3.v3[i];  // Component-wise addition of two vectors
        }
        return *this;
    }

    CRTVector<T> operator-(const CRTVector<T>& vec3) const {
        return CRTVector(x - vec3.x, y - vec3.y, z - vec3.z);  // Subtraction of two vectors
    }

    CRTVector<T>& operator-=(const CRTVector<T>& vec3) {
        for (int i = 0; i < 3; i++) {
            v3[i] -= vec3.v3[i];  // Component-wise subtraction of two vectors
        }
        return *this;
    }

    template <typename U>
    CRTVector<T> operator*(U s) const {
        return CRTVector<T>(x * s, y * s, z * s);  // Scalar multiplication of a vector
    }

    template <typename U>
    CRTVector<T>& operator*=(U s) {
        for (int i = 0; i < 3; i++) {
            v3[i] *= s;  // Component-wise scalar multiplication of a vector
        }
        return *this;
    }

    template <typename U>
    CRTVector<T> operator/(U f) const {
        float invF = (float)1 / f;  // Calculate the inverse of the scalar
        return CRTVector(x * invF, y * invF, z * invF);  // Scalar division of a vector
    }

    template <typename U>
    CRTVector<T>& operator/=(U f) {
        float invF = (float)1 / f;  // Calculate the inverse of the scalar
        for (int i = 0; i < 3; i++) {
            v3[i] *= invF;  // Component-wise scalar division of a vector
        }
        return *this;
    }

    bool operator==(const CRTVector<T>& vec3) const {
        return x == vec3.x && y == vec3.y && z == vec3.z;  // Equality comparison of two vectors
    }

    bool operator!=(const CRTVector<T>& vec3) const {
        return !(*this == vec3);  // Inequality comparison of two vectors
    }

    CRTVector<T> operator-() const {
        return CRTVector<T>(-x, -y, -z);  // Negation of a vector
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;  // Calculate the squared length of a vector
    }

    float length() const {
        return sqrtf(lengthSquared());  // Calculate the length of a vector
    }

    CRTVector<T> normalize() {
        Assert(length() != 0 && "zero divisor");  // Assertion to avoid division by zero
        return *this /= length();  // Normalize the vector (divide each component by its length)
    }
};

template <typename T, typename U>
inline static CRTVector<T> operator*(U s, const CRTVector<T>& v) {
    return v * s;  // Scalar multiplication of a vector (scalar on the left-hand side)
}

template <typename T>
inline static CRTVector<T> operator*(const CRTVector<T>& v1, const CRTVector<T>& v2) {
    return CRTVector<T>(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);  // Component-wise multiplication of two vectors
}

template <typename T>
inline static CRTVector<T> normalize(CRTVector<T>& vec3) {
    Assert(vec3.length() != 0 && "zero divisor");  // Assertion to avoid division by zero
    return vec3 / vec3.length();  // Normalize the vector (divide each component by its length)
}

//Calculates and returns the dot product of the vectors @v1 and @v2
template <typename T>
inline static T dot(const CRTVector<T>& v1, const CRTVector<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;  // Dot product of two vectors
}

// Calculates and returns the cross product of the vectors _v1_ and _v2_
template <typename T>
inline static CRTVector<T> cross(const CRTVector<T>& v1, const CRTVector<T>& v2) {
    return CRTVector<T>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);  // Cross product of two vectors
}

typedef CRTVector<float> CRTVectorf;
typedef CRTVector<float> Pointf;
typedef CRTVector<float> Colorf;
typedef CRTVector<float> Normalf;
typedef CRTVector<int> Colori;
typedef CRTVector<int> Normali;

#endif
