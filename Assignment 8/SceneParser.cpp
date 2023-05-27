#include "SceneParser.h"
#include "Scene.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Constants.h"

#include "rapidjson/istreamwrapper.h"

namespace ChaosCourse {
    void SceneParser::parse(const std::string& filename, Scene& scene) {
        rapidjson::Document doc = getJsonDoc(filename);

        parseSettings(scene, doc);
        parseCamera(scene, doc);
        parseObjects(scene, doc);
        parseLights(scene, doc);
    }

    rapidjson::Document SceneParser::getJsonDoc(const std::string& filename) {
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

    void SceneParser::parseSettings(Scene& scene, const rapidjson::Document& doc) {
        const rapidjson::Value& settingsVal = doc.FindMember(STR_SETTINGS)->value;

        if (!settingsVal.IsNull() && settingsVal.IsObject()) {
            Settings settings;

            //Extract Background colour
            const rapidjson::Value& bgColVal = settingsVal.FindMember(STR_BG_COLOR)->value;
            assert(!bgColVal.IsNull() && bgColVal.IsArray());
            settings.setBgColor(ColorRGB(loadVector(bgColVal.GetArray())));

            //Extract width and height
            const rapidjson::Value& imgSettingsVal = settingsVal.FindMember(STR_IMG_SETTINGS)->value;
            assert(!imgSettingsVal.IsNull() && imgSettingsVal.IsObject());
            const rapidjson::Value& imgWidthVal = imgSettingsVal.FindMember(STR_WIDTH)->value;
            const rapidjson::Value& imgHeightVal = imgSettingsVal.FindMember(STR_HEIGHT)->value;
            assert(!imgWidthVal.IsNull() && imgWidthVal.IsInt() && !imgHeightVal.IsNull() && imgHeightVal.IsInt());
            settings.setWidth(imgWidthVal.GetInt());
            settings.setHeight(imgHeightVal.GetInt());

            scene.setSettings(settings);
        }
    }

    void SceneParser::parseCamera(Scene& scene, const rapidjson::Document& doc) {
        const rapidjson::Value& camVal = doc.FindMember(STR_CAMERA)->value;
        if (!camVal.IsNull() && camVal.IsObject()) {
            Camera cam;

            //Extract position
            const rapidjson::Value& camPosVal = camVal.FindMember(STR_POS)->value;
            assert(!camPosVal.IsNull() && camPosVal.IsArray());
            cam.setPosition(loadVector(camPosVal.GetArray()));

            //Extract orientation
            const rapidjson::Value& camOrientVal = camVal.FindMember(STR_MATRIX)->value;
            assert(!camOrientVal.IsNull() && camOrientVal.IsArray());
            cam.setOrientation(loadMatrix(camOrientVal.GetArray()));

            scene.setCamera(cam);
        }
    }

    void SceneParser::parseLights(Scene& scene, const rapidjson::Document& doc) {
        const rapidjson::Value& lightVal = doc.FindMember(STR_LIGHTS)->value;
        if (!lightVal.IsNull() && lightVal.IsArray()) {
            scene.reservePointLights(lightVal.Size());

            for (int i = 0; i < lightVal.Size(); i++) { //For each mesh
                //Extract intensity
                const rapidjson::Value& intensityVal = lightVal[i].FindMember(STR_LIGHT_INTENSITY)->value;
                assert(!intensityVal.IsNull() && intensityVal.IsNumber());
                float intensity = intensityVal.GetFloat();

                //Extract vertices
                const rapidjson::Value& posVal = lightVal[i].FindMember(STR_POS)->value;
                assert(!posVal.IsNull() && posVal.IsArray());
                CRTVector position = loadVector(posVal.GetArray());

                scene.addPointLight(PointLight(position, intensity));
            }
        }
    }

    void SceneParser::parseObjects(Scene& scene, const rapidjson::Document& doc) {
        const rapidjson::Value& objVal = doc.FindMember(STR_OBJECTS)->value;
        if (!objVal.IsNull() && objVal.IsArray()) {
            scene.reserveMeshes(objVal.Size());

            for (int i = 0; i < objVal.Size(); i++) { //For each mesh
                //Extract vertices
                const rapidjson::Value& verticesVal = objVal[i].FindMember(STR_VERTICES)->value;
                assert(!verticesVal.IsNull() && verticesVal.IsArray());
                std::vector<CRTVector> vertices;
                loadVertices(verticesVal.GetArray(), vertices);

                //Extract triangles
                const rapidjson::Value& trianglesVal = objVal[i].FindMember(STR_TRIANGLES)->value;
                assert(!trianglesVal.IsNull() && trianglesVal.IsArray());
                std::vector<TriProxy> triangles;
                loadTriangles(trianglesVal.GetArray(), triangles);

                scene.addMesh(vertices, triangles);
            }
        }
    }

    CRTVector SceneParser::loadVector(const rapidjson::Value::ConstArray& arr) {
        assert(arr.Size() == 3);
        CRTVector vec(arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat());
        return vec;
    }

    Matrix3x3 SceneParser::loadMatrix(const rapidjson::Value::ConstArray& arr) {
        assert(arr.Size() == 9);
        Matrix3x3 mat(
            arr[0].GetFloat(), arr[1].GetFloat(), arr[2].GetFloat(),//row 0
            arr[3].GetFloat(), arr[4].GetFloat(), arr[5].GetFloat(),//row 1
            arr[6].GetFloat(), arr[7].GetFloat(), arr[8].GetFloat() //row 2
        );
        return mat;
    }

    void SceneParser::loadVertices(const rapidjson::Value::ConstArray& arr, std::vector<CRTVector>& vertices) {
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

    void SceneParser::loadTriangles(const rapidjson::Value::ConstArray& arr, std::vector<TriProxy>& triangles) {
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