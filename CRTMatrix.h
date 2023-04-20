#pragma once

namespace ChaosCourse {
    //Forward-declaration
    class CRTVector;

    class CRTMatrix {
    public:

        //Default constructor - zero matrix
        CRTMatrix();

        CRTMatrix(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
        CRTMatrix(float* entries);

        CRTMatrix(const CRTMatrix& m);



        CRTMatrix operator+(const CRTMatrix& m) const;
        CRTMatrix operator-(const CRTMatrix& m) const;
        CRTMatrix operator*(float s) const;
        CRTVector operator*(const CRTVector& v) const;
        CRTMatrix operator*(const CRTMatrix& m) const;
        CRTMatrix& operator=(const CRTMatrix& m);



        //Get the a[i][j] entry in the matrix. Note that indices must be in [0,2] range.
        float getEntry(int row, int col) const;

        //Get the i-th row, where i is in the [0,2] range.
        CRTVector row(int row) const;

        //Get the j-th col, where j is in the [0,2] range.
        CRTVector col(int col) const;

        //Get transposed matrix
        CRTMatrix getTranspose() const;



        void setCol(int col, CRTVector& v);

        void orthogonalize();

    private:
        float entry[9];
    };

    //Create a rotation matrix about the X-axis. 
    CRTMatrix createRotationX(float angle);

    //Create a rotation matrix about the Y-axis. 
    CRTMatrix createRotationY(float angle);

    //Create a rotation matrix about the Z-axis.
    CRTMatrix createRotationZ(float angle);
}
