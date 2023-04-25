#pragma once
#include "CRTVector.h"

namespace ChaosCourse {
    class CRTRay;

    class CRTTriangle {
    public:
        CRTTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2);

        CRTVector v(int index) const;

        CRTVector e(int index) const;

        //Get the normal of the triangle. It is assumed that the vertices are ordered in counter-clockwise order.
        CRTVector normal() const { return this->n; }

        float intersect(const CRTRay& ray, CRTVector& intersection) const;

        void setVertex(int index, const CRTVector& newV);

    private:
        //Vertices
        CRTVector vert[3];

        //Normal vector
        CRTVector n;
    };

}