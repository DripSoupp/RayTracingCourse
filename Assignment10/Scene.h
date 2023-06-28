#ifndef SCENE_H
#define SCENE_H

#include "Parser.h"


struct SceneParams {
    CRTCamera camera;
    std::vector<TriangleMesh> objects;
    std::vector<PointLight> lights;
    std::vector<Material> materials;
    SceneSettings settings;
};

class Scene {
public:
    Scene() = delete;

    Scene(const SceneParams& sceneParams)
        : camera(std::move(sceneParams.camera)),
        sceneObjects(std::move(sceneParams.objects)),
        sceneLights(std::move(sceneParams.lights)),
        materials(std::move(sceneParams.materials)),
        settings(std::move(sceneParams.settings)) {}

    bool intersect(const CRTRay& ray, InfoIntersect& info) const {
        bool hasIntersect = false;
        InfoIntersect closestPrim;
        for (const auto& mesh : sceneObjects) {
            if (mesh.intersect(ray, info)) {
                if (info.t < closestPrim.t) {
                    closestPrim = info;
                }
                hasIntersect = true;
            }
        }

        if (hasIntersect)
            info = closestPrim;

        return hasIntersect;
    }


    bool intersectPrim(const CRTRay& ray) const {
        InfoIntersect closestPrim;
        for (const auto& mesh : sceneObjects) {
            if (mesh.intersectPrim(ray, closestPrim) &&
                materials[closestPrim.materialIdx].type != MaterialType::Refractive) {
                return true;
            }
        }
        return false;
    }

    const Colorf& getBackground() const { return settings.backgrColor; }

    const SceneDimensions& getSceneDimensions() const { return settings.sceneDimensions; }

    const CRTCamera& getCamera() const { return camera; }

    const std::vector<PointLight>& getLights() const { return sceneLights; }

    const std::vector<TriangleMesh>& getObjects() const { return sceneObjects; }

    const std::vector<Material>& getMaterials() const { return materials; }

private:
    CRTCamera camera;
    const std::vector<TriangleMesh> sceneObjects;
    const std::vector<PointLight> sceneLights;
    const std::vector<Material> materials;
    const SceneSettings settings;

};
    inline static int32_t parseSceneParams(std::string_view inputFile, SceneParams& sceneParams) {
        if (Parser::parseCameraParameters(inputFile, sceneParams.camera) != EXIT_SUCCESS) {
            std::cerr << "Scene parser failed." << std::endl;
            return EXIT_FAILURE;
        }
        else if (Parser::parseSceneObjects(inputFile, sceneParams.objects) != EXIT_SUCCESS) {
            std::cerr << "Scene parser failed." << std::endl;
            return EXIT_FAILURE;
        }
        else if (Parser::parseSceneLights(inputFile, sceneParams.lights) != EXIT_SUCCESS) {
            std::cerr << "Scene parser failed." << std::endl;
            return EXIT_FAILURE;
        }
        else if (Parser::parseMaterials(inputFile, sceneParams.materials) != EXIT_SUCCESS) {
            std::cerr << "Scene parser failed." << std::endl;
            return EXIT_FAILURE;
        }
        else if (Parser::parseSceneSettings(inputFile, sceneParams.settings) != EXIT_SUCCESS) {
            std::cerr << "Scene parser failed." << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }


#endif  