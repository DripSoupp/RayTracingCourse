#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "CRTVector.h"
#include <cstdint>

class PointLight {
public:
    PointLight(const Pointf& _position, const int32_t _intensity)
        : position(_position), intensity(_intensity) {}

    Pointf getPosition() const { return position; }

    int32_t getIntensity() const { return intensity; }

private:
    const Pointf position;   ///< Light source position in world space
    const int32_t intensity;  ///< Light source intensity
};

#endif 