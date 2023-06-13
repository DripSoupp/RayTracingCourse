#include "Matrix3x3.h"
#include "CRTVector.h"
#include <assert.h>
#include <utility>

namespace ChaosCourse {
    Matrix3x3::Matrix3x3() {
        for (int i = 0; i < 9; i++) {
            entry[i] = 0.0f;
        }
    }

    Matrix3x3::Matrix3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22) {
        //Note that input is ordered in row-major fashion (first three input parameters = first row, etc.) but
        //they are stored in memory in column-major order.

        //col 0
        entry[0] = a00;
        entry[1] = a10;
        entry[2] = a20;

        //col 1
        entry[3] = a01;
        entry[4] = a11;
        entry[5] = a21;

        //col 2
        entry[6] = a02;
        entry[7] = a12;
        entry[8] = a22;
    }

    Matrix3x3::Matrix3x3(float* entries) {
        //Assume valid input
        for (int i = 0; i < 9; i++) {
            entry[i] = entries[i];
        }
    }

    Matrix3x3::Matrix3x3(const Matrix3x3& m) {
        for (int i = 0; i < 9; i++) {
            entry[i] = m.entry[i];
        }
    }

    Matrix3x3 Matrix3x3::operator+(const Matrix3x3& m) const {
        Matrix3x3 result;

        for (int i = 0; i < 9; i++) {
            result.entry[i] = entry[i] + m.entry[i];
        }

        return result;
    }

    Matrix3x3 Matrix3x3::operator-(const Matrix3x3& m) const {
        Matrix3x3 result;

        for (int i = 0; i < 9; i++) {
            result.entry[i] = entry[i] - m.entry[i];
        }

        return result;
    }

    Matrix3x3 Matrix3x3::operator*(float s) const {
        return Matrix3x3(
            s * entry[0], s * entry[3], s * entry[6],
            s * entry[1], s * entry[4], s * entry[7],
            s * entry[2], s * entry[5], s * entry[8]);
    }

    CRTVector Matrix3x3::operator*(const CRTVector& v) const {
        return CRTVector(
            entry[0] * v.x + entry[3] * v.y + entry[6] * v.z,
            entry[1] * v.x + entry[4] * v.y + entry[7] * v.z,
            entry[2] * v.x + entry[5] * v.y + entry[8] * v.z);
    }

    Matrix3x3 Matrix3x3::operator*(const Matrix3x3& m) const {
        float matEntry[9] = { 0.0f };

        matEntry[0] = entry[0] * m.entry[0] + entry[3] * m.entry[1] + entry[6] * m.entry[2];
        matEntry[1] = entry[1] * m.entry[0] + entry[4] * m.entry[1] + entry[7] * m.entry[2];
        matEntry[2] = entry[2] * m.entry[0] + entry[5] * m.entry[1] + entry[8] * m.entry[2];

        matEntry[3] = entry[0] * m.entry[3] + entry[3] * m.entry[4] + entry[6] * m.entry[5];
        matEntry[4] = entry[1] * m.entry[3] + entry[4] * m.entry[4] + entry[7] * m.entry[5];
        matEntry[5] = entry[2] * m.entry[3] + entry[5] * m.entry[4] + entry[8] * m.entry[5];

        matEntry[6] = entry[0] * m.entry[6] + entry[3] * m.entry[7] + entry[6] * m.entry[8];
        matEntry[7] = entry[1] * m.entry[6] + entry[4] * m.entry[7] + entry[7] * m.entry[8];
        matEntry[8] = entry[2] * m.entry[6] + entry[5] * m.entry[7] + entry[8] * m.entry[8];

        return Matrix3x3(matEntry);
    }

    Matrix3x3& Matrix3x3::operator=(const Matrix3x3& m) {
        for (int i = 0; i < 9; i++) {
            entry[i] = m.entry[i];
        }
        return *this;
    }

    float Matrix3x3::getEntry(int row, int col) const {
        assert(row >= 0 && row <= 2 && col >= 0 && col <= 2);
        return entry[col * 3 + row];
    }

    CRTVector Matrix3x3::row(int row) const {
        assert(row >= 0 && row <= 2);
        return CRTVector(entry[row], entry[row + 3], entry[row + 6]);
    }

    CRTVector Matrix3x3::col(int col) const
    {
        return CRTVector(entry[col * 3], entry[col * 3 + 1], entry[col * 3 + 2]);
    }

    Matrix3x3 Matrix3x3::getTranspose() const {
        //current_mat_col_i = transposed_mat_row_i, for 0<=i<=2
        return Matrix3x3(entry[0], entry[1], entry[2],
            entry[3], entry[4], entry[5],
            entry[6], entry[7], entry[8]);
    }

    void Matrix3x3::setCol(int col, CRTVector& v) {
        assert(col >= 0 && col <= 2);
        entry[col * 3] = v.x;
        entry[col * 3 + 1] = v.y;
        entry[col * 3 + 2] = v.z;
    }

    void Matrix3x3::orthogonalize() {
        CRTVector col0 = col(0);
        CRTVector col1 = col(1);
        CRTVector col2 = col(2);
        //Gram-Schmidt
        col1 = col1 - (col1.dot(col0) / col0.getLenSquared()) * col0;
        col2 = col2 - ((col2.dot(col0)) / col0.getLenSquared()) * col0;
        col2 = col2 - ((col2.dot(col1)) / col1.getLenSquared()) * col1;

        //Additional normalization
        col0.normalize();
        col1.normalize();
        col2.normalize();

        setCol(0, col0);
        setCol(1, col1);
        setCol(2, col2);
    }

    Matrix3x3 createRotationX(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return Matrix3x3(
            1.0f, 0.0f, 0.0f,
            0.0f, c, -s,
            0.0f, s, c);
    }

    Matrix3x3 createRotationY(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return Matrix3x3(
            c, 0.0f, s,
            0.0f, 1.0f, 0.0f,
            -s, 0.0f, c);
    }

    Matrix3x3 createRotationZ(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return Matrix3x3(
            c, -s, 0.0f,
            s, c, 0.0f,
            0.0f, 0.0f, 1.0f);
    }
}
