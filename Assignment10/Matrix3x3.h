#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include <cmath>
#include <cstring>
#include "MathUtil.h"
#include "Constants.h"

struct Matrix3x3 {
    Matrix3x3() = default;

    // Constructor to initialize the matrix with a diagonal value
    Matrix3x3(const float diagonal) {
        m[0][0] = m[1][1] = m[2][2] = diagonal;
    }

    // Constructor to initialize the matrix with row vectors
    Matrix3x3(const CRTVectorf& r0, const CRTVectorf& r1, const CRTVectorf& r2) {
        // Assign the values from the row vectors to the matrix elements
        m[0][0] = r0.x, m[0][1] = r0.y, m[0][2] = r0.z;
        m[1][0] = r1.x, m[1][1] = r1.y, m[1][2] = r1.z;
        m[2][0] = r2.x, m[2][1] = r2.y, m[2][2] = r2.z;
    }

    // Overloaded indexing operator to access rows of the matrix
    CRTVectorf operator[](const int idx) const {
        Assert(0 <= idx && idx < 3);
        return CRTVectorf(m[idx][0], m[idx][1], m[idx][2]);
    }

    // Overloaded multiplication operator to multiply two matrices
    friend Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2);

    // Overloaded multiplication operator to multiply a vector by a matrix
    friend CRTVectorf operator*(const CRTVectorf& v, const Matrix3x3& m);

    // Function to calculate the transpose of a matrix
    friend Matrix3x3 transpose(const Matrix3x3& m);

    // Function to calculate the determinant of a matrix
    friend float determinant(const Matrix3x3& m);

    // Function to calculate the inverse of a matrix
    friend Matrix3x3 inverse(const Matrix3x3& m);

    // Overloaded output stream operator to print the matrix
    friend std::ostream& operator<<(std::ostream& out, const Matrix3x3& m);

    float m[3][3]{}; // Elements of the matrix
};

// Function to rotate a matrix around the X-axis
inline static Matrix3x3 rotateX(const float theta) {
    const float thetaRad = deg2Radians(theta);
    const float cosThetaRad = cosf(thetaRad);
    const float sinThetaRad = sinf(thetaRad);
    const CRTVectorf r0{ 1.f, 0.f, 0.f };
    const CRTVectorf r1{ 0.f, cosThetaRad, -sinThetaRad };
    const CRTVectorf r2{ 0.f, sinThetaRad, cosThetaRad };
    return Matrix3x3{ r0, r1, r2 };
}

// Function to rotate a matrix around the Y-axis
inline static Matrix3x3 rotateY(const float theta) {
    const float thetaRad = deg2Radians(theta);
    const float cosThetaRad = cosf(thetaRad);
    const float sinThetaRad = sinf(thetaRad);
    const CRTVectorf r0{ cosThetaRad, 0.f, sinThetaRad };
    const CRTVectorf r1{ 0.f, 1.f, 0.f };
    const CRTVectorf r2{ -sinThetaRad, 0.f, cosThetaRad };
    return Matrix3x3{ r0, r1, r2 };
}

// Function to rotate a matrix around the Z-axis
inline static Matrix3x3 rotateZ(const float theta) {
    const float thetaRad = deg2Radians(theta);
    const float cosThetaRad = cosf(thetaRad);
    const float sinThetaRad = sinf(thetaRad);
    const CRTVectorf r0{ cosThetaRad, -sinThetaRad, 0.f };
    const CRTVectorf r1{ sinThetaRad, cosThetaRad, 0.f };
    const CRTVectorf r2{ 0.f, 0.f, 1.f };
    return Matrix3x3{ r0, r1, r2 };
}

#endif
