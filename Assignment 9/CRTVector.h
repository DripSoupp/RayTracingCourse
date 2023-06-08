#pragma once
#include<utility>

namespace ChaosCourse {
    /*
    * Class to represent a vector in 3 - dimensional space
    */
    class CRTVector {
    public:
        float x;
        float y;
        float z;

        //Zero vector is the default
        CRTVector() : x(0.0), y(0.0), z(0.0) {}
        CRTVector(float x, float y, float z) : x(x), y(y), z(z) {}
        CRTVector(const CRTVector& v) : x(v.x), y(v.y), z(v.z) {}

        //Arithmetics
        CRTVector operator+(const CRTVector& v) const { return CRTVector(x + v.x, y + v.y, z + v.z); }
        CRTVector operator-(const CRTVector& v) const { return CRTVector(x - v.x, y - v.y, z - v.z); }
        CRTVector operator*(float s) const { return CRTVector(x * s, y * s, z * s); }
        CRTVector operator-() const { return CRTVector(-x, -y, -z); }
        //Component-wise multiplication
        CRTVector compMult(const CRTVector& v) const { return CRTVector(x * v.x, y * v.y, z * v.z); }

        //Dot product
        float dot(const CRTVector& v) const;

        //Cross product
        CRTVector cross(const CRTVector& v) const;

        //Get the vector magnitude (euclidian norm)
        float getLen() const { return sqrtf(x * x + y * y + z * z); }

        //Get the square of the vector magnitude to avoid sqrt() operation.
        float getLenSquared() const { return x * x + y * y + z * z; }

        //Get a unit vector pointing in the same direction as the current vector
        void normalize();
    };

    //Non-member functin to handle (scalar)*(vector3) operation. Encapsulated in the ChaosCampAM namespace.
    CRTVector operator*(float s, const CRTVector& v);
}