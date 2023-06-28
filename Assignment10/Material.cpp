#include "Material.h"
#include "Scene.h"

Colorf Material::shade(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect) const {
    Colorf shadeColor;
    const MaterialType& hittedMaterialType = scene->getMaterials()[infoIntersect.materialIdx].type;
    if (hittedMaterialType == MaterialType::Diffuse) {
        shadeColor = shadeDiffuse(ray, scene, infoIntersect);
    }
    else if (hittedMaterialType == MaterialType::Reflective) {
        shadeColor = shadeReflective(ray, scene, infoIntersect);
    }
    else if (hittedMaterialType == MaterialType::Refractive) {
        shadeColor = shadeRefractive(ray, scene, infoIntersect);
    }
    else if (hittedMaterialType == MaterialType::Constant) {
        shadeColor = shadeConstant(ray, scene, infoIntersect);
    }
    else {
        Assert(false && "Material:: unsupported material type.");
    }
    return shadeColor;
}

Colorf shadeDiffuse([[maybe_unused]] const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect) {
    // Calculates the diffuse shading for a hit point with a diffuse material
    Colorf hitColor;
    const std::vector<PointLight>& lights = scene->getLights();
    const Colorf& albedo = scene->getMaterials()[infoIntersect.materialIdx].property.albedo;
    bool smoothShading = scene->getMaterials()[infoIntersect.materialIdx].smoothShading;
    const Normalf intersectNormal = smoothShading ? infoIntersect.smoothNormal : infoIntersect.faceNormal;
    for (const PointLight& light : lights) {
        // Calculate the lighting contribution from each point light
        CRTVectorf lightDir = light.getPosition() - infoIntersect.pos;
        const float lightDist = lightDir.length();
        const float lightArea = calcSphereArea(lightDist);
        const CRTVectorf lightDirN = normalize(lightDir);
        const float cosTheta = std::max(0.f, dot(lightDirN, intersectNormal));
        const CRTRay shadowRay(infoIntersect.pos + intersectNormal * SHADOW_BIAS, lightDirN);
        shadowRay.tMax = lightDist;
        Colorf perLightColor;
        if (!scene->intersectPrim(shadowRay)) {
            // Check if the shadow ray is occluded by any objects in the scene
            perLightColor = Colorf(light.getIntensity() / lightArea * albedo * cosTheta);
        }
        hitColor += perLightColor;
    }

    return hitColor;
}

Colorf shadeReflective(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect) {
    // Calculates the shading for a hit point with a reflective material
    bool smoothShading = scene->getMaterials()[infoIntersect.materialIdx].smoothShading;
    Normalf surfNormal = smoothShading ? infoIntersect.smoothNormal : infoIntersect.faceNormal;
    const CRTVectorf reflectedDir = reflect(ray.dir, surfNormal);
    CRTRay reflectedRay = CRTRay(infoIntersect.pos + surfNormal * REFLECTION_BIAS, reflectedDir);
    const Colorf& albedo = scene->getMaterials()[infoIntersect.materialIdx].property.albedo;
    if (ray.depth <= MAX_RAY_DEPTH && scene->intersect(reflectedRay, infoIntersect)) {
        const Material* hittedMaterial = &scene->getMaterials()[infoIntersect.materialIdx];
        if (hittedMaterial->type == MaterialType::Reflective) {
            reflectedRay.depth = ray.depth + 1;
            return hittedMaterial->shade(reflectedRay, scene, infoIntersect);
        }
        else {
            return albedo * hittedMaterial->shade(reflectedRay, scene, infoIntersect);
        }
    }
    return albedo * scene->getBackground();
}

Colorf shadeRefractive(const CRTRay& ray, const Scene* scene, InfoIntersect& infoIntersect) {
    // Calculates the shading for a hit point with a refractive material
    if (ray.depth <= MAX_RAY_DEPTH) {
        bool smoothShading = scene->getMaterials()[infoIntersect.materialIdx].smoothShading;
        Normalf surfNormal = smoothShading ? infoIntersect.smoothNormal : infoIntersect.faceNormal;
        float cosThetaI = clamp(-1.f, 1.f, dot(ray.dir, surfNormal));
        const float ior = scene->getMaterials()[infoIntersect.materialIdx].property.ior;
        float etaI = 1.f, etaT = ior;
        bool rayLeaveTransparent = cosThetaI > 0.f;
        if (rayLeaveTransparent) {
            std::swap(etaI, etaT);
            surfNormal = -surfNormal;
        }
        else
            cosThetaI = -cosThetaI;

        Colorf refractColor, reflectColor;
        CRTVectorf refrRayDir;
        if (refract(ray.dir, surfNormal, etaI / etaT, cosThetaI, &refrRayDir)) {
            // construct and trace refraction ray in the scene
            CRTRay refractionRay = CRTRay(infoIntersect.pos + (-surfNormal * REFRACTION_BIAS), refrRayDir);
            InfoIntersect refrInfoIntersect;
            if (scene->intersect(refractionRay, refrInfoIntersect)) {
                refractionRay.depth = ray.depth + 1;
                const Material* hittedMaterial = &scene->getMaterials()[refrInfoIntersect.materialIdx];
                refractColor = hittedMaterial->shade(refractionRay, scene, refrInfoIntersect);
            }
            else {
                refractColor = scene->getBackground();
            }

            // construct and trace reflection ray in the scene
            const CRTVectorf reflRayDir = reflect(ray.dir, surfNormal);
            CRTRay reflectionRay = CRTRay(infoIntersect.pos + (surfNormal * REFLECTION_BIAS), reflRayDir);
            InfoIntersect reflInfoIntersect;
            if (scene->intersect(reflectionRay, reflInfoIntersect)) {
                reflectionRay.depth = ray.depth + 1;
                const Material* hittedMaterial = &scene->getMaterials()[reflInfoIntersect.materialIdx];
                reflectColor = hittedMaterial->shade(reflectionRay, scene, reflInfoIntersect);
            }
            else {
                reflectColor = scene->getBackground();
            }

            const float fres = fresnel(ray.dir, surfNormal);
            return fres * reflectColor + (1 - fres) * refractColor;
        }
        else {
            const CRTVectorf reflRayDir = reflect(ray.dir, surfNormal);
            CRTRay reflectionRay = CRTRay(infoIntersect.pos + (surfNormal * REFLECTION_BIAS), reflRayDir);
            InfoIntersect reflInfoIntersect;
            if (scene->intersect(reflectionRay, reflInfoIntersect)) {
                reflectionRay.depth = ray.depth + 1;
                const Material* hittedMaterial = &scene->getMaterials()[reflInfoIntersect.materialIdx];
                return hittedMaterial->shade(reflectionRay, scene, reflInfoIntersect);
            }
            else {
                return scene->getBackground();
            }
        }
    }

    return scene->getBackground();
}

Colorf shadeConstant([[maybe_unused]] const CRTRay& ray, const Scene* scene,
    InfoIntersect& infoIntersect) {
    // Calculates the shading for a hit point with a constant material
    return scene->getMaterials()[infoIntersect.materialIdx].property.albedo;
}
