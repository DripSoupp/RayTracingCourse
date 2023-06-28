#include <iostream>
#include <thread>
#include "CRTTriangle.h"


// Function to calculate the surface normal of a triangle
inline static Normalf calcSurfaceNormal(const Triangle& triangle) {
    const CRTVectorf& A = triangle.mesh->vertPositions[triangle.indices[0]];
    const CRTVectorf& B = triangle.mesh->vertPositions[triangle.indices[1]];
    const CRTVectorf& C = triangle.mesh->vertPositions[triangle.indices[2]];
    const CRTVectorf E0 = B - A;
    const CRTVectorf E1 = C - A;
    return Normalf(cross(E0, E1).normalize());
}

// Function to generate a ray from screen coordinates
inline static CRTRay getScreenRay(const int row, const int col) {
    const float ndcX = (col + 0.5f) / IMG_WIDTH;
    const float ndcY = (row + 0.5f) / IMG_HEIGHT;
    const float screenX = (2.f * ndcX - 1.f) * ASPECT_RATIO;
    const float screenY = (1.f - 2.f * ndcY);
    return CRTRay{ {0, 0, 0}, (CRTVectorf(screenX, screenY, -1)).normalize() };
}

// Function to convert degrees to radians
inline static float deg2Radians(const float degrees) { return degrees * (PI / 180.f); }

// Function to calculate the area of a sphere
inline static float calcSphereArea(const float sphereR) { return 4 * PI * sphereR * sphereR; }

// Function to clamp a value within a range
inline static float clamp(const float low, const float high, const float value) {
    return std::max(low, std::min(high, value));
}

// Function to calculate the reflection direction of an incident ray
template <typename T>
inline static CRTVector<T> reflect(const CRTVector<T>& incRayDir, const CRTVector<T>& surfNormal) {
    return (incRayDir - 2.f * dot(incRayDir, surfNormal) * surfNormal).normalize();
}

// Function to calculate the refraction direction of an incident ray
inline static bool refract(const CRTVectorf& incRayDir, const CRTVectorf& surfNormal, const float eta,
    const float cosThetaI, CRTVectorf* refrRayDir) {
    const float cos2ThetaT = 1 - eta * eta * (1 - cosThetaI * cosThetaI);
    if (cos2ThetaT < EPSILON)
        return false;

    const float cosThetaT = sqrtf(cos2ThetaT);
    *refrRayDir = eta * incRayDir + (eta * cosThetaI - cosThetaT) * surfNormal;
    refrRayDir->normalize();

    return true;
}

// Function to calculate the Fresnel term for reflection
inline static float fresnel(const CRTVectorf& incRayDir, const Normalf& surfNormal) {
    return 0.5f * powf(1.f + dot(incRayDir, surfNormal), 5.f);
}

// Function to get the number of hardware threads available
inline static unsigned getHardwareThreads() {
    return std::max<unsigned>(std::thread::hardware_concurrency() - 1, 1);
}

// Function to generate the PPM file name from the input file name
inline static std::string getPpmFileName(const std::string& inputFile) {
    const size_t start = inputFile.rfind("/");
    const size_t end = inputFile.rfind(".");
    if (start > end)
        return inputFile.substr(0, end) + ".ppm";
    return inputFile.substr(start + 1, end - start - 1) + ".ppm";
}
