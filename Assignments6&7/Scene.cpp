#include "Scene.h"
#include <fstream>
#include <iostream>
#include "rapidjson/istreamwrapper.h"

namespace ChaosCourse {
    Scene::Scene(const std::string& filename) {
        rapidjson::Document doc = getJsonDoc(filename);

        parseSettings(doc);
        parseCamera(doc);
        parseObjects(doc);
    }

    const std::vector<Mesh>& Scene::getMesh() const {
        return meshes;
    }

    const CRTCamera& Scene::getCamera() const {
        return cam;
    }

    const Settings& Scene::getSettings() const {
        return settings;
    }

    void Scene::setCamera(const CRTCamera& newCam) {
        cam = newCam;
    }

    void Scene::setSettings(const Settings& newSettings) {
        settings = newSettings;
    }

    rapidjson::Document Scene::getJsonDoc(const std::string& filename) {
        std::ifstream input(filename);
        assert(input.is_open());

        //Parse to rapidjson DOM
        rapidjson::IStreamWrapper inStream(input);
        rapidjson::Document doc;
        doc.ParseStream(inStream);

        if (doc.HasParseError()) {
            std::cout << "Parse Error: " << doc.GetParseError() << "\n";
            std::cout << "Error Offset: " << doc.GetErrorOffset() << "\n";
            assert(false);
        }

        assert(doc.IsObject());
        return doc;
    }

    void Scene::parseSettings(const rapidjson::Document& doc) {
        const rapidjson::Value& settingsVal = doc.FindMember("settings")->value;

        if (!settingsVal.IsNull() && settingsVal.IsObject()) {
            //Extract Background colour
            const rapidjson::Value& bgColVal = settingsVal.FindMember("background_color")->value;
            assert(!bgColVal.IsNull() && bgColVal.IsArray());
            settings.setBgColor(loadColorRGB(bgColVal.GetArray()));

            //Extract width and height
            const rapidjson::Value& imgSettingsVal = settingsVal.FindMember("image_settings")->value;
            assert(!imgSettingsVal.IsNull() && imgSettingsVal.IsObject());
            const rapidjson::Value& imgWidthVal = imgSettingsVal.FindMember("width")->value;
            const rapidjson::Value& imgHeightVal = imgSettingsVal.FindMember("height")->value;
            assert(!imgWidthVal.IsNull() && imgWidthVal.IsInt() && !imgHeightVal.IsNull() && imgHeightVal.IsInt());
            settings.setWidth(imgWidthVal.GetInt());
            settings.setHeight(imgHeightVal.GetInt());
        }
    }

    void Scene::parseCamera(const rapidjson::Document& doc) {
        const rapidjson::Value& camVal = doc.FindMember("camera")->value;
        if (!camVal.IsNull() && camVal.IsObject()) {
            //Extract position
            const rapidjson::Value& camPosVal = camVal.FindMember("position")->value;
            assert(!camPosVal.IsNull() && camPosVal.IsArray());
            cam.setPosition(loadVector(camPosVal.GetArray()));

            //Extract orientation
            const rapidjson::Value& camOrientVal = camVal.FindMember("matrix")->value;
            assert(!camOrientVal.IsNull() && camOrientVal.IsArray());
            cam.setOrientation(loadMatrix(camOrientVal.GetArray()));
        }
    }

    void Scene::parseObjects(const rapidjson::Document& doc) {
        const rapidjson::Value& objVal = doc.FindMember("objects")->value;
        if (!objVal.IsNull() && objVal.IsArray()) {
            for (int i = 0; i < objVal.Size(); i++) { //For each mesh
                //Extract vertices
                const rapidjson::Value& verticesVal = objVal[i].FindMember("vertices")->value;
                assert(!verticesVal.IsNull() && verticesVal.IsArray());
                std::vector<CRTVector> vertices;
                loadVertices(verticesVal.GetArray(), vertices);

                //Extract triangles
                const rapidjson::Value& trianglesVal = objVal[i].FindMember("triangles")->value;
                assert(!trianglesVal.IsNull() && trianglesVal.IsArray());
                std::vector<TriProxy> triangles;
                loadTriangles(trianglesVal.GetArray(), triangles);

                meshes.emplace_back(vertices, triangles);
            }
        }
    }

    CRTVector Scene::loadVector(const rapidjson::Value::ConstArray& arr) {
        assert(arr.Size() == 3);
        CRTVector vec(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
        return vec;
    }

    CRTMatrix Scene::loadMatrix(const rapidjson::Value::ConstArray& arr) {
        assert(arr.Size() == 9);
        CRTMatrix mat(
            arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(),//row 0
            arr[3].GetFloat(), arr[4].GetFloat(), arr[5].GetFloat(),//row 1
            arr[6].GetFloat(), arr[7].GetFloat(), arr[8].GetFloat() //row 2
        );
        return mat;
    }

    ColorRGB Scene::loadColorRGB(const rapidjson::Value::ConstArray& arr) {
        assert(arr.Size() == 3);
        ColorRGB col(
            static_cast<unsigned char>(arr[0].GetFloat() * ColorRGB::maxColorComponent),
            static_cast<unsigned char>(arr[1].GetFloat() * ColorRGB::maxColorComponent),
            static_cast<unsigned char>(arr[2].GetFloat() * ColorRGB::maxColorComponent));
        return col;
    }

    void Scene::loadVertices(const rapidjson::Value::ConstArray& arr, std::vector<CRTVector>& vertices) {
        int arrSize = arr.Size();
        assert(arrSize % 3 == 0);

        //Empty vector and allocate required memory all at once
        vertices.clear();
        vertices.reserve(arrSize / 3);

        //Add all vertices to the list
        for (int i = 0; i < arrSize / 3; i++) {
            vertices.emplace_back(
                arr[3 * i].GetFloat(),
                arr[3 * i + 1].GetFloat(),
                arr[3 * i + 2].GetFloat()
            );
        }
    }

    void Scene::loadTriangles(const rapidjson::Value::ConstArray& arr, std::vector<TriProxy>& triangles) {
        int arrSize = arr.Size();
        assert(arrSize % 3 == 0);

        //Empty vector and allocate required memory all at once
        triangles.clear();
        triangles.reserve(arrSize / 3);

        for (int i = 0; i < arrSize / 3; i++) {
            triangles.emplace_back(
                arr[3 * i].GetInt(),
                arr[3 * i + 1].GetInt(),
                arr[3 * i + 2].GetInt()
            );
        }
    }
}