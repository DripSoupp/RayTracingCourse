#include "Matrix3x3.h"
#include "CRTVector.h"
#include <ostream>

// Matrix multiplication
Matrix3x3 operator*(const Matrix3x3& m1, const Matrix3x3& m2) {
    Matrix3x3 res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res.m[i][j] =
                m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j];
        }
    }
    return res;
}

// Vector-matrix multiplication
CRTVectorf operator*(const CRTVectorf& v, const Matrix3x3& m) {
    CRTVectorf res;
    res.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0];
    res.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1];
    res.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2];
    return res;
}

// Transpose of matrix
Matrix3x3 transpose(const Matrix3x3& m) {
    Matrix3x3 res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            res.m[i][j] = m.m[j][i];
        }
    }
    return res;
}

// Determinant of matrix
float determinant(const Matrix3x3& m) {
    return m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] +
        m.m[0][2] * m.m[1][0] * m.m[2][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] -
        m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][2] * m.m[1][1] * m.m[2][0];
}

// Calculate inverse matrix of _m_ by using minor matrices
Matrix3x3 inverse(const Matrix3x3& m) {
    const float det = determinant(m);

    // inverse matrix does not exist if determinant == 0
    if (fabs(det) < EPSILON)
        return m;

    const float rDet = 1 / det;

    Matrix3x3 res;
    res.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2]) * rDet;
    res.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * rDet;
    res.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * rDet;

    res.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * rDet;
    res.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * rDet;
    res.m[1][2] = (m.m[1][0] * m.m[0][2] - m.m[0][0] * m.m[1][2]) * rDet;

    res.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[2][0] * m.m[1][1]) * rDet;
    res.m[2][1] = (m.m[2][0] * m.m[0][1] - m.m[0][0] * m.m[2][1]) * rDet;
    res.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[1][0] * m.m[0][1]) * rDet;

    return res;
}

std::ostream& operator<<(std::ostream& out, const Matrix3x3& m) {
    out << "[ [" << m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << "]\n";
    out << "  [" << m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << "]\n";
    out << "  [" << m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << "] ]";
    return out;
}