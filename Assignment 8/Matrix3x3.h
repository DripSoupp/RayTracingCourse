#pragma once

namespace ChaosCourse {
    //Forward-declaration
    class CRTVector;

   
    class Matrix3x3 {
    public:

        //Default constructor - zero matrix
        Matrix3x3();

        //The matrix entries are initialized to a set of values.
        //   entries are encoded as 'a[row][col]' - row0: <a00, a01, a02>, row1: <a10,a11,a12>, row2: <a20,a21,a22>
        Matrix3x3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);

        //The matrix entries are initialized to an array.
        // (the way they are stored in memory).
        Matrix3x3(float* entries);

        //The matrix is initialized to another matrix. (Copy constructor)
        Matrix3x3(const Matrix3x3& m);


        /* ARITHMETIC OPERATORS */

        Matrix3x3 operator+(const Matrix3x3& m) const;
        Matrix3x3 operator-(const Matrix3x3& m) const;
        Matrix3x3 operator*(float s) const;
     
        //Convention was adopted in accordance with the A*x=b equation in linear algebra.
        CRTVector operator*(const CRTVector& v) const;
        //NOTE: Multiplication proceeds right-to-left, i.e. [Result] = [Matrix 2] * [Matrix 1]
        Matrix3x3 operator*(const Matrix3x3& m) const;
        Matrix3x3& operator=(const Matrix3x3& m);


        //Get the a[i][j] entry in the matrix. Note that indices must be in [0,2] range.
        float getEntry(int row, int col) const;

        //Get the i-th row, where i is in the [0,2] range.
        CRTVector row(int row) const;

        //Get the j-th col, where j is in the [0,2] range.
        CRTVector col(int col) const;

        //Get transposed matrix
        Matrix3x3 getTranspose() const;


        
        //Sets a column of the matrix. Col index must be in [0,2] range.
        void setCol(int col, CRTVector& v);

        //Perform Gram-Schmidt Orthogonalization to make the columns of the matrix orthogonal vectors.
        //Then, normalize the columns to ensure an orthogonal matrix (columns are orthonormal).
        void orthogonalize();

    private:
        //Entries will be encoded in col-major fashion.
        float entry[9];
    };

    //Create a rotation matrix about the X-axis. Angle must be given in radians.
    Matrix3x3 createRotationX(float angle);

    //Create a rotation matrix about the Y-axis. Angle must be given in radians.
    Matrix3x3 createRotationY(float angle);

    //Create a rotation matrix about the Z-axis. Angle must be given in radians.
    Matrix3x3 createRotationZ(float angle);
}