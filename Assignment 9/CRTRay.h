#pragma once
#include "CRTVector.h"

namespace ChaosCourse {
    //Forward declaration
    class CRTCamera;
    enum class RayType {
        camera,
        invalid,
        shadow,
        reflective,
        refractive
    };

    class CRTRay {
    public:
        //Default: ray in the -z direction starting from the origin
        CRTRay() : origin(), dir(0.0, 0.0, -1.0) {}
        CRTRay(const CRTVector& origin, const CRTVector& direction) : origin(origin), dir(direction) {
            dir.normalize();
        };

        //Getters
        CRTVector getOrigin() const { return origin; }
        CRTVector getDirection() const { return dir; }

        //Get a point lying on the ray, based on parameter t in the range [0;+inf). Negative values forbidden!
        CRTVector getPointOnRay(float t) const;

    private:
        CRTVector origin;
        CRTVector dir;
    };

    //Non-member to compute the camera ray through the centre of pixel (x,y).
    //Pixel (0,0) assumed in top left corner.
    CRTRay computeCameraRay(int xIndex, int yIndex, int imageHeight, float aspectRatio, const CRTCamera& cam);

    //Compute the reflected ray at an intersection point.
    //Ray direction of incoming ray is pointing TOWARDS the intersection point.
    CRTRay computeReflectedRay(const CRTVector& incomingRay, const CRTVector& intersectionPoint, const CRTVector& normal);
}