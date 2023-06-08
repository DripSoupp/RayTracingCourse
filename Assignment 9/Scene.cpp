#include "Scene.h"
#include <fstream>
#include <iostream>

namespace ChaosCourse {
    Scene::Scene() {}

    const std::vector<Mesh>& Scene::getMeshes() const {
        return meshes;
    }

    const std::vector<Material>& Scene::getMaterials() const {
        return materials;
    }

    const CRTCamera& Scene::getCamera() const {
        return cam;
    }

    const Settings& Scene::getSettings() const {
        return settings;
    }

    const std::vector<PointLight>& Scene::getPointLights() const {
        return pointLights;
    }

    void Scene::setCamera(const CRTCamera& newCam) {
        cam = newCam;
    }

    void Scene::setSettings(const Settings& newSettings) {
        settings = newSettings;
    }

    void Scene::setMeshes(const std::vector<Mesh>& newMeshes) {
        meshes = newMeshes;
    }

    void Scene::setMaterials(const std::vector<Material>& newMaterials) {
        materials = newMaterials;
    }

    void Scene::setPointLights(const std::vector<PointLight>& newPointLights) {
        pointLights = newPointLights;
    }

    void Scene::addMesh(const Mesh& mesh) {
        meshes.push_back(mesh);
    }
    void Scene::addMesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles, int matIndex) {
        meshes.emplace_back(vertices, triangles, matIndex);
    }

    void Scene::addMaterial(const Material& mat) {
        materials.push_back(mat);
    }

    void Scene::addPointLight(const PointLight& pointLight) {
        pointLights.push_back(pointLight);
    }

    void Scene::reserveMeshes(int numMeshes) {
        meshes.reserve(numMeshes);
    }

    void Scene::reserveMaterials(int numMaterials) {
        materials.reserve(numMaterials);
    }

    void Scene::reservePointLights(int numPointLights) {
        pointLights.reserve(numPointLights);
    }

    std::vector<Material>& Scene::getMaterials() {
        return materials;
    }
}