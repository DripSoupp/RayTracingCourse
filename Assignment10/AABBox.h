#ifndef AABBOX_H
#define AABBOX_H

#include "CRTRay.h"

// Finds and returns the component-wise minimum values of points _p1_ and _p2_
template <typename T>
inline static CRTVector<T> minPoint(const CRTVector<T>& p1, const CRTVector<T>& p2) {
    using std::min;
    return CRTVector<T>(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
}

// Finds and returns the component-wise maximum values of points _p1_ and _p2_
template <typename T>
inline static CRTVector<T> maxPoint(const CRTVector<T>& p1, const CRTVector<T>& p2) {
    using std::max;
    return CRTVector<T>(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
}

// Computes at compile time gamma coefficient that is used to bound
// the inherent floating-point arithmetic rounding error
// source https://github.com/mmp/pbrt-v3/blob/master/src/core/pbrt.h
inline constexpr float gamma(int n) { return (n * EPSILON) / (1 - n * EPSILON); }

/// @brief Axis aligned bounding box represented by min & max corner points in 3D
struct BBox {
    CRTVectorf min;  ///< Vertex with minimum coordinate values
    CRTVectorf max;  ///< Vertex with maximum coordinate values

    BBox() : min(MAX_FLOAT), max(MIN_FLOAT) {}

    BBox(const CRTVectorf& p1, const CRTVectorf& p2) : min(minPoint(p1, p2)), max(maxPoint(p1, p2)) {}

    // Extends the bounds of the box by another box _otherBox_
    void unionWith(const BBox& otherBox) {
        min = minPoint(min, otherBox.min);
        max = maxPoint(max, otherBox.max);
    }

    // Extends the bounds of the box by point _p_
    template <typename T>
    void expandBy(const CRTVector<T>& p) {
        min = minPoint(min, p);
        max = maxPoint(max, p);
    }

    //Verifies if ray intersects with the box using Kay and Kajiya’s 
    // slab method
    // source https://github.com/mmp/pbrt-v3/blob/master/src/core/geometry.h
    bool intersect(const CRTRay& ray) const {
        float t0 = 0, t1 = MAX_FLOAT;
        for (int i = 0; i < 3; i++) {
            float invRayDir = 1 / ray.dir[i];
            float tNear = (min[i] - ray.origin[i]) * invRayDir;
            float tFar = (max[i] - ray.origin[i]) * invRayDir;

            if (tNear > tFar) {
                std::swap(tNear, tFar);
            }

            tFar *= 1 + 2 * gamma(3);  // Update tFar to ensure robust ray–bbox intersection
            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar < t1 ? tFar : t1;
            if (t0 > t1) {
                return false;
            }
        }

        return true;
    }
};

#endif  