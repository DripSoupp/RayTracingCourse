#pragma once
#include <string>
#include <vector>
#include"Mesh.h"

#include"rapidjson/document.h"

namespace ChaosCourse {
    //Forward-declarations
    class Scene;
    class CRTVector;
    class Matrix3x3;
    class TriProxy;

    //Takes the scenefile parsing functionality away from the Scene class.
    //Responsible for reading a .crtscene json file and 
    //initialising a Scene object with the parsed data.
    class SceneParser {
    public:
        SceneParser() {}

        //Takes the filename of a .crtscene json file and attempts parsing it into a Scene class
        void parse(const std::string& filename, Scene& scene);

    private:
        //Extract a rapidjson document from the scene file given
        rapidjson::Document getJsonDoc(const std::string& filename);

        //Extract scene settings from the rapidjson document
        void parseSettings(Scene& scene, const rapidjson::Document& doc);

        //Extract scene camera from the rapidjson document
        void parseCamera(Scene& scene, const rapidjson::Document& doc);

        //Extract scene lights from the rapidjson document
        void parseLights(Scene& scene, const rapidjson::Document& doc);

        //Extract object (meshes) form the rapidjson document
        void parseObjects(Scene& scene, const rapidjson::Document& doc);

        //Convert a vector object (geometric 3D vector) from rapidjson array to a local Vector3 object.
        CRTVector loadVector(const rapidjson::Value::ConstArray& arr);

        //Convert a matrix object from rapidjson array to a local Matrix3x3 object.
        Matrix3x3 loadMatrix(const rapidjson::Value::ConstArray& arr);

        //Convert a vertex list from rapidjson array to a local list of Vector3 objects.
        void loadVertices(const rapidjson::Value::ConstArray& arr, std::vector<CRTVector>& vertices);

        //Convert a triangle list from rapidjson array to a local list of TriProxy objects.
        void loadTriangles(const rapidjson::Value::ConstArray& arr, std::vector<TriProxy>& triangles);
    };
}