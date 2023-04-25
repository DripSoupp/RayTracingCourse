#include"Mesh.h"
#include"CRTTriangle.h"
#include <assert.h>

namespace ChaosCourse {

	Mesh::Mesh(int vertexHint, int triangleHint) {
		vertexList.reserve(vertexHint);
		triIdxList.reserve(triangleHint);
	}

	Mesh::Mesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles) : 
		vertexList(vertices), triIdxList(triangles) {}

	void Mesh::pushVertex(const CRTVector& vert) {
		vertexList.push_back(vert);
	}

	void Mesh::pushTriangle(const TriProxy& tri) {
		triIdxList.push_back(tri);
	}

	float Mesh::intersection(const CRTRay& ray, CRTVector& intersection, int& triIdx) const {
		float closestDist = FLT_MAX;
		CRTVector closestIntersect;

		int index = 0;
			for (TriProxy tri : triIdxList) {
				int vertexCount = vertexList.size();
				assert(tri.v0 < vertexCount && tri.v1 < vertexCount && tri.v2 < vertexCount);

				CRTTriangle realTri(vertexList.at(tri.v0), vertexList.at(tri.v1), vertexList.at(tri.v2));
				float dist = realTri.intersect(ray, closestIntersect);

				// Keep only closest intersection
				if (dist > 0.0 && dist < closestDist) {
					closestDist = dist;
					intersection = closestIntersect;
					triIdx = index;
				}
				index++;
			}
			if (closestDist < FLT_MAX) {
				return closestDist;

			}
			else {
				return -1.0f;
			}
			return 0.0f;
	}
	int Mesh::getNumTriangles() const {
		return triIdxList.size();
	}
}