#ifndef RENDERER_H
#define RENDERER_H

#include "PPMImage.h"
#include "Scene.h"
#include "ThreadPool.h"
#include "Timer.h"

struct RenderSettings {
    const unsigned numThreads = getHardwareThreads();  // Number of threads to use for rendering
    const size_t numPixelsPerThread = PIXELS_PER_THREAD;  // Number of pixels processed by each thread
};

// Performs ray tracing for a given ray in the scene and returns the computed color
static Colorf rayTrace(const CRTRay& ray, const Scene* scene) {
    InfoIntersect infoIntersect;
    if (scene->intersect(ray, infoIntersect)) {
        // If there is an intersection, compute the shading for the intersection point using the material
        return scene->getMaterials()[infoIntersect.materialIdx].shade(ray, scene, infoIntersect);
    }
    // If no intersection occurs, return the background color of the scene
    return scene->getBackground();
}

class Renderer {
public:
    Renderer() = delete;

    // Constructor that takes a reference to a PPMImage and a pointer to a Scene
    Renderer(PPMImageI& _ppmImage, const Scene* _scene) : ppmImage(_ppmImage), scene(_scene) {}

    // Render function that performs the rendering process
    void render(const size_t threadId, const size_t threadCount, const size_t chunkSize = 1) {
        const SceneDimensions& dimens = scene->getSceneDimensions();
        const CRTCamera& camera = scene->getCamera();
        for (size_t i = (chunkSize * threadId); i < ppmImage.data.size();
            i += (chunkSize * threadCount)) {
            for (size_t c = 0; c < chunkSize; c++) {
                const int row = (i + c) / dimens.width;
                const int col = (i + c) % dimens.width;
                const CRTRay cameraRay = camera.getRay(row, col);
                const Colorf currPixelColor = rayTrace(cameraRay, scene);
                ppmImage.data[i + c].color = Colori(clamp(0.f, 1.f, currPixelColor.x) * 255,
                    clamp(0.f, 1.f, currPixelColor.y) * 255,
                    clamp(0.f, 1.f, currPixelColor.z) * 255);
            }
        }
    }

private:
    PPMImageI& ppmImage;  // Reference to the PPMImage to store the rendered image
    const Scene* scene;   // Pointer to the Scene object containing the scene data
};

#endif
