#include "Scene.h"
#include <fstream>
#include <iostream>

namespace ChaosCourse {
    Scene::Scene() {}

    const std::vector<Mesh>& Scene::getMeshes() const {
        return meshes;
    }

    const Camera& Scene::getCamera() const {
        return cam;
    }

    const Settings& Scene::getSettings() const {
        return settings;
    }

    const std::vector<PointLight>& Scene::getPointLights() const {
        return pointLights;
    }

    void Scene::setCamera(const Camera& newCam) {
        cam = newCam;
    }

    void Scene::setSettings(const Settings& newSettings) {
        settings = newSettings;
    }

    void Scene::setMeshes(const std::vector<Mesh>& newMeshes) {
        meshes = newMeshes;
    }

    void Scene::setPointLights(const std::vector<PointLight>& newPointLights) {
        pointLights = newPointLights;
    }

    void Scene::addMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
    }
    void Scene::addMesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles) {
        meshes.emplace_back(vertices, triangles);
    }

    void Scene::addPointLight(const PointLight& pointLight) {
        pointLights.push_back(pointLight);
    }

    void Scene::reserveMeshes(int numMeshes) {
        meshes.reserve(numMeshes);
    }

    void Scene::reservePointLights(int numPointLights) {
        pointLights.reserve(numPointLights);
    }
}