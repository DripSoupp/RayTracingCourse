#pragma once
#include"CRTVector.h"

namespace ChaosCourse {
    
    enum class MaterialType { Diffuse, Reflective };

    struct Material {
        MaterialType type;
        CRTVector albedo;
        bool smoothShading;

        Material(MaterialType type, const CRTVector& albedo, bool smoothShading) :
            type(type), albedo(albedo), smoothShading(smoothShading) {}
    };
}