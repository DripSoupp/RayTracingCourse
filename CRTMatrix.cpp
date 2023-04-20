#include "CRTMatrix.h"
#include "CRTVector.h"
#include <assert.h>
#include <utility>

namespace ChaosCourse {
    CRTMatrix::CRTMatrix() {
        for (int i = 0; i < 9; i++) {
            entry[i] = 0.0f;
        }
    }

    CRTMatrix::CRTMatrix(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22) {
        

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

    CRTMatrix::CRTMatrix(float* entries) {
        
        for (int i = 0; i < 9; i++) {
            entry[i] = entries[i];
        }
    }

    CRTMatrix::CRTMatrix(const CRTMatrix& m) {
        for (int i = 0; i < 9; i++) {
            entry[i] = m.entry[i];
        }
    }

    CRTMatrix CRTMatrix::operator+(const CRTMatrix& m) const {
        CRTMatrix result;

        for (int i = 0; i < 9; i++) {
            result.entry[i] = entry[i] + m.entry[i];
        }

        return result;
    }

    CRTMatrix CRTMatrix::operator-(const CRTMatrix& m) const {
        CRTMatrix result;

        for (int i = 0; i < 9; i++) {
            result.entry[i] = entry[i] - m.entry[i];
        }

        return result;
    }

    CRTMatrix CRTMatrix::operator*(float s) const {
        return CRTMatrix(
            s * entry[0], s * entry[3], s * entry[6],
            s * entry[1], s * entry[4], s * entry[7],
            s * entry[2], s * entry[5], s * entry[8]);
    }

    CRTVector CRTMatrix::operator*(const CRTVector& v) const {
        return CRTVector(
            entry[0] * v.x + entry[3] * v.y + entry[6] * v.z,
            entry[1] * v.x + entry[4] * v.y + entry[7] * v.z,
            entry[2] * v.x + entry[5] * v.y + entry[8] * v.z);
    }

    CRTMatrix CRTMatrix::operator*(const CRTMatrix& m) const {
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

        return CRTMatrix(matEntry);
    }

    CRTMatrix& CRTMatrix::operator=(const CRTMatrix& m) {
        for (int i = 0; i < 9; i++) {
            entry[i] = m.entry[i];
        }
        return *this;
    }

    float CRTMatrix::getEntry(int row, int col) const {
        assert(row >= 0 && row <= 2 && col >= 0 && col <= 2);
        return entry[col * 3 + row];
    }

    CRTVector CRTMatrix::row(int row) const {
        assert(row >= 0 && row <= 2);
        return CRTVector(entry[row], entry[row + 3], entry[row + 6]);
    }

    CRTVector CRTMatrix::col(int col) const
    {
        return CRTVector(entry[col * 3], entry[col * 3 + 1], entry[col * 3 + 2]);
    }

    CRTMatrix CRTMatrix::getTranspose() const {
        
        return CRTMatrix(entry[0], entry[1], entry[2],
            entry[3], entry[4], entry[5],
            entry[6], entry[7], entry[8]);
    }

    void CRTMatrix::setCol(int col, CRTVector& v) {
        assert(col >= 0 && col <= 2);
        entry[col * 3] = v.x;
        entry[col * 3 + 1] = v.y;
        entry[col * 3 + 2] = v.z;
    }

    void CRTMatrix::orthogonalize() {
        CRTVector col0 = col(0);
        CRTVector col1 = col(1);
        CRTVector col2 = col(2);
   
        col1 = col1 - (col1.dot(col0) / col0.getLenSquared()) * col0;
        col2 = col2 - ((col2.dot(col0)) / col0.getLenSquared()) * col0;
        col2 = col2 - ((col2.dot(col1)) / col1.getLenSquared()) * col1;

        
        col0.normalize();
        col1.normalize();
        col2.normalize();

        setCol(0, col0);
        setCol(1, col1);
        setCol(2, col2);
    }

    CRTMatrix createRotationX(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return CRTMatrix(
            1.0f, 0.0f, 0.0f,
            0.0f, c, -s,
            0.0f, s, c);
    }

    CRTMatrix createRotationY(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return CRTMatrix(
            c, 0.0f, s,
            0.0f, 1.0f, 0.0f,
            -s, 0.0f, c);
    }

    CRTMatrix createRotationZ(float angle) {
        float c = cosf(angle);
        float s = sinf(angle);
        return CRTMatrix(
            c, -s, 0.0f,
            s, c, 0.0f,
            0.0f, 0.0f, 1.0f);
    }
}