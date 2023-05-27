#include"ColorRGB.h"
#include"CRTVector.h"
#include"MathUtil.h"
#include<assert.h>

namespace ChaosCourse {
    ColorRGB::ColorRGB(const CRTVector& colorFloat) {
        //components are between 0.0 and 1.0
        int xColorInt = static_cast<int>(colorFloat.x * maxColorComponents);
        int yColorInt = static_cast<int>(colorFloat.y * maxColorComponents);
        int zColorInt = static_cast<int>(colorFloat.z * maxColorComponents);
        if (xColorInt > 255) xColorInt = 255;
        if (yColorInt > 255) yColorInt = 255;
        if (zColorInt > 255) zColorInt = 255;

        r = static_cast<unsigned char>(xColorInt);
        g = static_cast<unsigned char>(yColorInt);
        b = static_cast<unsigned char>(zColorInt);
    }
}