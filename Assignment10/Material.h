#ifndef MATERIAL_H
#define MATERIAL_H

#include "CRTTriangle.h"
#include <string>

struct Scene;

enum class MaterialType : uint8_t { Diffuse, Reflective, Refractive, Constant, Undefined};

union MaterialProperty {
    Colorf albedo = Colorf(0);
    float ior;
};


struct Material {
    const MaterialProperty property;
    bool smoothShading = false;
    const MaterialType type = MaterialType::Undefined;

    Material(const MaterialProperty& _property, bool _smoothShading, const MaterialType _type)
        : property(_property), smoothShading(_smoothShading), type(_type) {}

    Colorf shade(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect) const;
};

inline static Material makeMaterial(std::string_view materialType, const MaterialProperty& property,
    bool smoothShading) {
    MaterialType mtype = MaterialType::Undefined;
    if (materialType == "diffuse") {
        mtype = MaterialType::Diffuse;
    }
    else if (materialType == "reflective") {
        mtype = MaterialType::Reflective;
    }
    else if (materialType == "refractive") {
        mtype = MaterialType::Refractive;
    }
    else if (materialType == "constant") {
        mtype = MaterialType::Constant;
    }
    else {
        Assert(mtype != MaterialType::Undefined &&
            "makeMaterial() recieved unsupported material type.");
    }
    return Material(property, smoothShading, mtype);
}

Colorf shadeDiffuse(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect);

Colorf shadeReflective(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect);

Colorf shadeRefractive(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect);

Colorf shadeConstant(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect);

#endif  