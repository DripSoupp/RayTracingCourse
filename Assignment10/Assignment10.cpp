#include "Renderer.h"

static int32_t runRenderer(const std::string& inputFile, ThreadPool& pool,
    const RenderSettings& settings) {
    const std::string ppmFileName = getPpmFileName(inputFile);
    std::ofstream ppmImageFile(ppmFileName, std::ios::out | std::ios::binary);
    if (!ppmImageFile.good()) {
        std::cout << "Input file " << inputFile << " not good.\n";
        return EXIT_FAILURE;
    }

    SceneParams sceneParams;
    if (parseSceneParams(inputFile, sceneParams) != EXIT_SUCCESS) {
        std::cerr << "Failed to parse " << inputFile << " file." << std::endl;
        return EXIT_FAILURE;
    }

    // initialize scene
    const Scene scene(sceneParams);

    // initialize image
    const SceneDimensions dimens = scene.getSceneDimensions();
    PPMImageI ppmImage(dimens.width, dimens.height);

    // initialize renderer
    Renderer renderer(ppmImage, &scene);

    std::cout << "Loading " << ppmFileName << "...\nGenerating data...\n";
    {
        Timer timer;

        for (size_t threadId = 0; threadId < settings.numThreads; threadId++) {
            auto renderTask = std::bind(&Renderer::render, renderer, threadId, settings.numThreads,
                settings.numPixelsPerThread);
            pool.scheduleTask(renderTask);
        }

        pool.completeTasks();

        std::cout << ppmFileName << " generated in ["
            << Timer::toMilliSec<float>(timer.getElapsedNanoSec()) << "ms] on "
            << settings.numThreads << " threads\n";
    }

    serializePPMImage(ppmImageFile, ppmImage);
    ppmImageFile.close();

    return EXIT_SUCCESS;
}

int main() {
    const std::vector<std::string> inputFiles = {
        "input/scene0.crtscene"/*, "input/scene1.crtscene", "input/scene2.crtscene",
        "input/scene3.crtscene", "input/scene4.crtscene", "input/scene5.crtscene",
        "input/scene6.crtscene", "input/scene7.crtscene", "input/scene8.crtscene"*/ };

    RenderSettings renderSettings;

    ThreadPool pool(renderSettings.numThreads);
    pool.start();

    for (const auto& file : inputFiles) {
        if (runRenderer(file, pool, renderSettings) != EXIT_SUCCESS) {
            std::cerr << "Failed to render file - " << file << std::endl;
            return EXIT_FAILURE;
        }
    }

    pool.stop();

    return 0;
}