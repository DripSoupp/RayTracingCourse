#ifndef CRTRAY_H
#define CRTRAY_H

#include "CRTVector.h"

struct CRTRay {
    CRTVectorf dir;
    int depth = 0;
    mutable float tMax = MAX_FLOAT;
    Pointf origin;

    CRTRay() : origin(0), dir(0, 0, -1) {}

    CRTRay(const Pointf& org, const CRTVectorf& d) : origin(org), dir(d) {}

    Pointf at(const float t) const { return origin + t * dir; }
};

#endif  
