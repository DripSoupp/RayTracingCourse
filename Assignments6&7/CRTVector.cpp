#include "CRTVector.h"
#include "MathUtil.h"
#include <assert.h>
#include <utility>

namespace ChaosCourse {

    float CRTVector::dot(const CRTVector& v) const { return x * v.x + y * v.y + z * v.z; }

    CRTVector CRTVector::cross(const CRTVector& v) const {
        return CRTVector(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    void CRTVector::normalize() {
        float len = this->getLen();

        //handle division by zero
        assert(abs(len) > EPSILON);

        float coeff = 1 / len;
        x = x * coeff;
        y = y * coeff;
        z = z * coeff;
    }

    CRTVector operator*(float s, const CRTVector& v) { return v * s; }
}