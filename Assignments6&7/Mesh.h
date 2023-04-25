#pragma once
#include<vector>
#include "CRTVector.h"


namespace ChaosCourse {

	class CRTRay;

	struct TriProxy {
		int v0;
		int v1;
		int v2;
		TriProxy(int vert0, int vert1, int vert2) : v0(vert0), v1(vert1), v2(vert2) {}
	};

	class Mesh {
	public:
		Mesh(int vertexHint = 100, int triangleHint = 100);

		Mesh(const std::vector < CRTVector>& vertices, const std::vector<TriProxy>& triangles);
		 
		void pushVertex(const CRTVector& vert);

		void pushTriangle(const TriProxy& tri);

		float intersection(const CRTRay& ray, CRTVector& intersection, int& triIdx) const;

		int getNumTriangles() const;

	private:
		std::vector<CRTVector> vertexList;
		std::vector<TriProxy> triIdxList;
	};
}