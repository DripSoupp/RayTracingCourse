#pragma once
#include"Settings.h"
#include"CRTCamera.h"
#include"Mesh.h"
#include<vector>
#include<string>

#include"rapidjson/document.h"

namespace ChaosCourse {

	class Scene {
	public:
		Scene(const std::string& filename);

		const std::vector<Mesh>& getMesh() const;
		const CRTCamera& getCamera() const;
		const Settings& getSettings() const;

		void setCamera(const CRTCamera& newCam);
		void setSettings(const Settings& newSettings);

	private:
		std::vector<Mesh> meshes;
		CRTCamera cam;
		Settings settings;

		rapidjson::Document getJsonDoc(const std::string& filename);

		void parseSettings(const rapidjson::Document& doc);

		void parseCamera(const rapidjson::Document& doc);

		void parseObjects(const rapidjson::Document& doc);

		CRTVector loadVector(const rapidjson::Value::ConstArray& arr);
		CRTMatrix loadMatrix(const rapidjson::Value::ConstArray& arr);
		ColorRGB loadColorRGB(const rapidjson::Value::ConstArray& arr);
		void loadVertices(const  rapidjson::Value::ConstArray& arr, std::vector<CRTVector>& vertiices);
		void loadTriangles(const rapidjson::Value::ConstArray& arr, std::vector<TriProxy>& triangles);

	};
}