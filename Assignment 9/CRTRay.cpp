#include "CRTRay.h"
#include "CRTCamera.h"
#include "Constants.h"
#include<assert.h>

namespace ChaosCourse {

    CRTVector CRTRay::getPointOnRay(float t) const {
        //We assume direction is normalised
        return origin + t * dir;
    }

    CRTRay computeCameraRay(int xIndex, int yIndex, int imageHeight, float aspectRatio, const CRTCamera& cam) {
        //Calculate normalised coordinates of pixel centre
        float x = (float)xIndex + 0.5f;
        float y = (float)yIndex + 0.5f;
        x /= (float)imageHeight * aspectRatio;
        y /= imageHeight;

        //Calculate screen-space coordinates
        x = (2.0f * x) - 1.0f;
        y = 1.0f - (2.0f * y);
        x *= aspectRatio;

        //Extract normalised ray through pixel. Normalisation happens in Ray constructor.
        return CRTRay(cam.getPosition(), cam.getOrientation() * CRTVector(x, y, -1.0));
    }

    CRTRay computeReflectedRay(const CRTVector& incomingRay, const CRTVector& intersectionPoint, const CRTVector& normal) {
        CRTVector reflectedDir = incomingRay - 2 * incomingRay.dot(normal) * normal;
        return CRTRay(intersectionPoint + normal * REFLECTION_RAY_BIAS, reflectedDir);
    }
}