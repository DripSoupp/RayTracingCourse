#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace ChaosCourse {

	class Scene;
	class CRTRay;
	class Mesh;
	class CRTVector;

	class Render {
	public:
	Render(){}
	
	void render(const Scene& scene, const std::string& filename);

	private:

	float findIntersection(const CRTRay& ray, const std::vector<Mesh>& meshes, CRTVector intersection, int& triIdx);


	};
}
