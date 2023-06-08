#pragma once
#include"CRTVector.h"

namespace ChaosCourse {

    struct PointLight {
        CRTVector pos;
        float intensity;//intensity coefficient, no specific unit of measurement
        PointLight(const CRTVector& lightPos, float lightIntensity) :
            pos(lightPos), intensity(lightIntensity) {}
    };

}