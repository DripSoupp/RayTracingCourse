#pragma once
#include"Settings.h"
#include"Camera.h"
#include"Mesh.h"
#include"PointLight.h"
#include<vector>
#include<string>

namespace ChaosCourse {
    //A scene object holding all the data for a scene:
    // - Geometry
    // - Camera
    // - Scene settings
    // - (to be added) lighting, materials, etc.
    //
    //This is a scene description only! All rendering functionality is in the dedicated Renderer class.

    class Scene {
    public:
        Scene();

        //Getters

        const std::vector<Mesh>& getMeshes() const;
        const Camera& getCamera() const;
        const Settings& getSettings() const;
        const std::vector<PointLight>& getPointLights() const;

        //Setters

        void setCamera(const Camera& newCam);
        void setSettings(const Settings& newSettings);

        //It is advisable not to use this function, as it may cause a lot of copying.
        void setMeshes(const std::vector<Mesh>& newMeshes);
        //It is advisable not to use this function, as it may cause a lot of copying.
        void setPointLights(const std::vector<PointLight>& newPointLights);

        //Add an existing mesh to the scene.
        void addMesh(const Mesh& mesh);
        //Add a mesh to the scene. Mesh constructed in place.
        void addMesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles);
        //Add an existing point light to the scene.
        void addPointLight(const PointLight& pointLight);

        //Allocate memory for the given number of meshes.
        void reserveMeshes(int numMeshes);
        //Allocate memory for the given number of point lights
        void reservePointLights(int numPointLights);

    private:
        std::vector<Mesh> meshes;
        std::vector<PointLight> pointLights;
        Camera cam;
        Settings settings;
    };
}