#pragma once
#include<utility>

namespace ChaosCourse {
   
    class CRTVector {
    public:
        float x;
        float y;
        float z;

        CRTVector() : x(0.0), y(0.0), z(0.0) {}
        CRTVector(float x, float y, float z) : x(x), y(y), z(z) {}
        CRTVector(const CRTVector& v) : x(v.x), y(v.y), z(v.z) {}

        //Arithmetics
        CRTVector operator+(const CRTVector& v) const { return CRTVector(x + v.x, y + v.y, z + v.z); }
        CRTVector operator-(const CRTVector& v) const { return CRTVector(x - v.x, y - v.y, z - v.z); }
        CRTVector operator*(float s) const { return CRTVector(x * s, y * s, z * s); }
        CRTVector operator-() const { return CRTVector(-x, -y, -z); }

        float dot(const CRTVector& v) const;

        CRTVector cross(const CRTVector& v) const;

        float getLen() const { return sqrtf(x * x + y * y + z * z); }

        float getLenSquared() const { return x * x + y * y + z * z; }

        void normalize();
    };

    CRTVector operator*(float s, const CRTVector& v);
}