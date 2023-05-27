#include "CRTTriangle.h"
#include "CRTRay.h"
#include "MathUtil.h"
#include <assert.h>
#include <math.h>

namespace ChaosCourse {
    CRTTriangle::CRTTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2) : vert{ v0,v1,v2 } {
        //Cross product of two edges
        n = (v1 - v0).cross(v2 - v0);
        n.normalize();
    }

    CRTVector CRTTriangle::v(int index) const {
        assert(index >= 0 && index < 3);
        return vert[index];
    }

    CRTVector CRTTriangle::e(int index) const {
        assert(index >= 0 && index < 3);
        if (index == 0) {
            return vert[1] - vert[0];
        }
        else if (index == 1) {
            return vert[2] - vert[1];
        }
        return vert[0] - vert[2];
    }
    CRTVector CRTTriangle::normal() const {
        CRTVector normal = n;
        normal.normalize();
        return normal;
    }

    void CRTTriangle::setVertex(int index, const CRTVector& newV) {
        assert(index >= 0 && index < 3);
        vert[index] = newV;
        n = (vert[1] - vert[0]).cross(vert[2] - vert[0]);
        n.normalize();
    }

    
    float CRTTriangle::intersect(const CRTRay& ray, CRTVector& intersection) const {
        //E0 = edge from vertex 0 to vertex 1
        CRTVector e0 = e(0);
        //E1 = edge from vertex 0 to vertex 2
        CRTVector e1 = -e(2);

        //p - coefficient used when calculating the determinant, but also used later!
        CRTVector dir = ray.getDirection();
        CRTVector p = dir.cross(e1);

        //If det is negative, then triangle is back-facing- ALLOW THIS CASE, i.e. no back-face culling
        float det = e0.dot(p);

        //If det is 0, then ray direction is parallel to triangle plane
        if (det > -EPSILON && det < EPSILON) {
            return -1.0;
        }
        float invDet = 1 / det;

        //t - distance from vertex 0 to ray origin
        CRTVector t = ray.getOrigin() - vert[0];

        //The 'U' barycentric coordinate - the one associated with v1 (yet unnormalised) 
        float uCoord = t.dot(p) * invDet;
        if (uCoord < 0.0f || uCoord > 1.0f) {
            return -1.0;
        }

        //The 'V' barycentric coordinate - the one associated with v2 (yet unnormalised)
        CRTVector q = t.cross(e0);
        float vCoord = dir.dot(q) * invDet;
        if (vCoord < 0.0f || uCoord + vCoord > 1.0f) {
            return -1.0;
        }

        //Calculate distance (along the ray) to the intersection point and calculate intersection itself.
        float dist = e1.dot(q) * invDet;

        if (dist < -EPSILON) return -1.0;
        intersection = ray.getPointOnRay(dist);
        return dist;
    }
}