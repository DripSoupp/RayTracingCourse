#pragma once
#include "CRTVector.h"

namespace ChaosCourse {
    //Forward declaration
    class CRTRay;
    struct InfoIntersect;

    // Triangle in 3D space. Vectices are ordered in counter-clockwise manner.
    class CRTTriangle {
    public:
        CRTTriangle(const CRTVector& v0, const CRTVector& v1, const CRTVector& v2);

    
        CRTVector v(int index) const;

        CRTVector e(int index) const;
        
        CRTVector normal() const;

       
        float intersect(const CRTRay& ray, InfoIntersect& intersectInfo) const;

        //Set a vertex of the triangle (v0/v1/v2 = newV) and update normal. Indices outside the [0;2] range are forbidden.
        void setVertex(int index, const CRTVector& newV);

    private:
        //Vertices
        CRTVector vert[3];

    };

    //Utility structure to store all intersection information
    struct InfoIntersect {
        CRTVector triNormal;
        CRTVector intersectionPoint;
        float coords[3]; //barycentric coordinates of the intersection point
        bool hasIntersection;

        //Default constructor - no intersection yet, so fill with null values
        InfoIntersect() :
            hasIntersection(false) {

            coords[0] = 0.0f;
            coords[1] = 0.0f;
            coords[2] = 0.0f;
        }
    };

}
