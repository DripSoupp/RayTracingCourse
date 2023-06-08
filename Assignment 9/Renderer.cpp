#include"Renderer.h"
#include"Scene.h"
#include"ColorRGB.h"
#include"CRTRay.h"
#include"Mesh.h"
#include"Material.h"
#include"CRTCamera.h"
#include"MathUtil.h"
#include"CRTVector.h"
#include"Constants.h"
#include"CRTTriangle.h"
#include<assert.h>
#include<algorithm>

#include<iostream>
void ChaosCourse::Renderer::render(const Scene& scene, const std::string& filename, const ShadingMode& shadingMode) {
    //Initial getters
    const Settings& settings = scene.getSettings();
    int imageWidth = settings.getWidth();
    int imageHeight = settings.getHeight();
    float aspectRatio = settings.getAspectRatio();
    const CRTCamera& cam = scene.getCamera();

    //Set up output file stream
    std::ofstream ppmFileStream(filename, std::ios::out | std::ios::binary);
    assert(ppmFileStream.is_open());
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << ColorRGB::maxColorComponents << "\n";

    //Loop through pixels and shoot camera rays
    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            CRTRay ray = computeCameraRay(colIdx, rowIdx, imageHeight, aspectRatio, cam);

            //Calculate pixel color by the method of ray-tracing
            ColorRGB pixelColor = ColorRGB(rayTrace(ray, 0, shadingMode, scene));

            ppmFileStream << (int)pixelColor.r << " " << (int)pixelColor.g << " " << (int)pixelColor.b << "\t";
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();
}

ChaosCourse::CRTVector ChaosCourse::Renderer::rayTrace(const CRTRay& ray, int depth, ShadingMode shadingMode, const Scene& scene) {
    //initial definitions
    CRTVector pixelColor = scene.getSettings().getBgColor();//default colour is background colour
    if (depth == MAX_TRACING_DEPTH) return pixelColor; //max depth reached - stop tracing
    const std::vector<Mesh>& meshes = scene.getMeshes();

    //intersect
    InfoIntersect intersectInfo;
    int meshIndex = 0;
    int triIndex = 0;
    float dist = findIntersection(ray, meshes, intersectInfo, meshIndex, triIndex);

    if (intersectInfo.hasIntersection) {
        //intersection occured - colour pixel based on shading mode
        if (shadingMode == ShadingMode::Light) {

            //extract material properties
            assert(scene.getMaterials().size() > 0);
            const Material& mat = scene.getMaterials()[meshes[meshIndex].getMatIndex()];
            CRTVector albedo = mat.albedo;

            CRTVector normal = mat.smoothShading ?
                extractHitNormal(meshes, intersectInfo, meshIndex, triIndex) : //if smooth shading is enabled, take hit normal
                intersectInfo.triNormal; //else take triangle normal

            //in case of diffuse material, do lambertian shading
            if (mat.type == MaterialType::Diffuse) {
                pixelColor = shadeLambertian(intersectInfo.intersectionPoint, normal, albedo, scene);
            }
            // in case of reflective material, trace a reflected ray
            else if (mat.type == MaterialType::Reflective) {
                CRTRay reflectedRay = computeReflectedRay(ray.getDirection(), intersectInfo.intersectionPoint, normal);
                pixelColor = rayTrace(reflectedRay, ++depth, shadingMode, scene).compMult(albedo);
            }
        }
        else if (shadingMode == ShadingMode::Barycentric) {
            pixelColor = shadeBarycentric(intersectInfo.coords);
        }
    }
    return pixelColor;
}

float ChaosCourse::Renderer::findIntersection(const CRTRay& ray, const std::vector<Mesh>& meshes,
    InfoIntersect& intersectInfo, int& meshIndex, int& triIndex) {
    //Closest intersection among all meshes
    float closestDist = FLT_MAX;
    InfoIntersect closestIntersect;
    intersectInfo.hasIntersection = false;

    //Loop through all meshes and find closest intersection

    int index = 0;
    int triIndexCurrent = 0;
    for (const Mesh& mesh : meshes) {
        float dist = mesh.intersect(ray, closestIntersect, triIndexCurrent);
        if (closestIntersect.hasIntersection && dist < closestDist) {
            closestDist = dist;
            intersectInfo = closestIntersect;
            meshIndex = index;
            triIndex = triIndexCurrent;
        }
        index++;
    }

    //If no intersection occurred, return negative distance
    return intersectInfo.hasIntersection ? closestDist : -1.0f;
}

ChaosCourse::CRTVector ChaosCourse::Renderer::extractHitNormal(const std::vector<Mesh>& meshes, const InfoIntersect& intersectInfo,
    int meshIndex, int triIndex) {

    //extract the triangles and vertex normals of the intersected mesh
    const std::vector<TriProxy>& triangles = meshes[meshIndex].getTriangles();
    const std::vector<CRTVector>& vertexNormals = meshes[meshIndex].getVertNormals();

    //find the vertex normals of the intersected triangle within the mesh
    CRTVector normal0 = vertexNormals[triangles[triIndex].v0];
    CRTVector normal1 = vertexNormals[triangles[triIndex].v1];
    CRTVector normal2 = vertexNormals[triangles[triIndex].v2];

    //calculate hit normal as a weighted sum of the three vertex normals
    return intersectInfo.coords[0] * normal0 + intersectInfo.coords[1] * normal1 + intersectInfo.coords[2] * normal2;
}

ChaosCourse::CRTVector ChaosCourse::Renderer::shadeLambertian(const CRTVector& point, const CRTVector& normal,
    const CRTVector& albedo, const Scene& scene) {
    CRTVector finalColor;
    for (PointLight pointLight : scene.getPointLights()) {
        //Direction from point to light
        CRTVector lightDir = pointLight.pos - point;
        //Sphere radius
        float rad = lightDir.getLen();
        //Normalise light direction
        lightDir = lightDir * (1 / rad);

        //Lambertian shading
        float cos = std::max(0.0f, lightDir.dot(normal));
        float sphereArea = 4 * PI * rad * rad;
       CRTRay shadowRay(point + normal * SHADOW_BIAS, lightDir);

        InfoIntersect intersectInfo;
        int meshIndexDummyVar = 0;
        int triIndexDummyVar = 0;
        float dist = findIntersection(shadowRay, scene.getMeshes(), intersectInfo, meshIndexDummyVar, triIndexDummyVar);
        if (!intersectInfo.hasIntersection) {
            //no intersection, i.e. no shadow
            float r = (pointLight.intensity * albedo.x * cos) / (sphereArea);
            float g = (pointLight.intensity * albedo.y * cos) / (sphereArea);
            float b = (pointLight.intensity * albedo.z * cos) / (sphereArea);

            finalColor = finalColor + CRTVector(r, g, b);
        }
    }
    return finalColor;
}

ChaosCourse::CRTVector ChaosCourse::Renderer::shadeBarycentric(float coords[3]) {
    assert(abs(coords[0] + coords[1] + coords[2] - 1.0f) < EPSILON_RELAXED);
    return CRTVector(coords[0], coords[1], coords[2]);
}