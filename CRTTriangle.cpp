#include "CRTTriangle.h"
#include "CRTRay.h"
#include "MathUtil.h"
#include <assert.h>

namespace ChaosCourse {
    CRTTriangle::CRTTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2) : vert{ v0,v1,v2 }, n((v1 - v0).cross(v2 - v0)) {
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
        float det = e0.dot(p);

        //If det is 0, then ray direction is parallel to triangle plane
        //If det is negative, then triangle is back-facing
        if (det < EPSILON) {
            return -1.0;
        }

        //t - distance from vertex 0 to ray origin
        CRTVector t = ray.getOrigin() - vert[0];

        //The 'U' barycentric coordinate - the one associated with v1 (yet unnormalised) 
        float uCoord = t.dot(p);
        if (uCoord<0.0 || uCoord > det) {
            return -1.0;
        }

        //The 'V' barycentric coordinate - the one associated with v2 (yet unnormalised)
        CRTVector q = t.cross(e0);
        float vCoord = dir.dot(q);
        if (vCoord<0.0 || uCoord + vCoord > det) {
            return -1.0;
        }

        //Calculate distance to the intersection point and calculate intersection itself.
        float dist = e1.dot(q);
        float invDet = 1 / det;
        dist *= invDet;
        uCoord *= invDet;
        vCoord *= invDet;

        intersection = ray.getPointOnRay(dist);
        return dist;
    }
}