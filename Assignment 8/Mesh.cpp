#include"Mesh.h"
#include"CRTTriangle.h"
#include"MathUtil.h"
#include<assert.h>

namespace ChaosCourse {

    Mesh::Mesh(int vertexHint, int triangleHint) {
        vertexList.reserve(vertexHint);
        triIndexList.reserve(triangleHint);
    }

    Mesh::Mesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles) :
        vertexList(vertices), triIndexList(triangles) {}

    void Mesh::pushVertex(const CRTVector& vert) {
        vertexList.push_back(vert);
    }

    void Mesh::pushTriangle(const TriProxy& tri) {
        triIndexList.push_back(tri);
    }

    float Mesh::intersect(const CRTRay& ray, CRTVector& intersection, CRTVector& triNormal) const {
        //For each triangle index tuple, construct an actual triangle object and test for intersection
        float closestDist = FLT_MAX;
        CRTVector closestIntersect;

        for (TriProxy tri : triIndexList) {
            //ensure valid indices
            int vertexCount = vertexList.size();
            assert(tri.v0 < vertexCount && tri.v1 < vertexCount && tri.v2 < vertexCount);

            //construct actual triangle object and intersect
            CRTTriangle realTri(vertexList.at(tri.v0), vertexList.at(tri.v1), vertexList.at(tri.v2));
            float dist = realTri.intersect(ray, closestIntersect);

            //keep only closest intersection
            if (dist > -EPSILON && dist < closestDist) {
                closestDist = dist;
                intersection = closestIntersect;
                triNormal = realTri.normal();
            }
        }

        //Return -1.0f if no intersection occurred
        return closestDist < FLT_MAX ? closestDist : -1.0f;
    }

    int Mesh::getNumTriangles() const {
        return triIndexList.size();
    }
}