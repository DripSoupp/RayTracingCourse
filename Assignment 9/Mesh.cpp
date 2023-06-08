#include"Mesh.h"
#include"CRTTriangle.h"
#include"MathUtil.h"
#include<assert.h>

namespace ChaosCourse {

    Mesh::Mesh(int vertexHint, int triangleHint) {
        vertexList.reserve(vertexHint);
        vertexNormalList.reserve(vertexHint);
        triIndexList.reserve(triangleHint);
    }

    Mesh::Mesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles, int matIndex) :
        vertexList(vertices), triIndexList(triangles), matIndex(matIndex) {
        recalculateNormals();
    }

    void Mesh::pushVertex(const CRTVector& vert) {
        vertexList.push_back(vert);
    }

    void Mesh::pushTriangle(const TriProxy& tri) {
        triIndexList.push_back(tri);
    }

    void Mesh::setMatIndex(int newMatIndex) {
        matIndex = newMatIndex;
    }

    void Mesh::recalculateNormals() {
        //Normal list has the same size as vertex list and initialise with zero vectors.
        vertexNormalList.reserve(vertexList.size());
        vertexNormalList.assign(vertexList.size(), CRTVector());

        //Loop through all triangles and add normal contribution to the list of normal vectors (for corresponding vertex)
        for (TriProxy& tri : triIndexList) {
            CRTVector triNormal = CRTTriangle(vertexList[tri.v0], vertexList[tri.v1], vertexList[tri.v2]).normal();
            vertexNormalList[tri.v0] = vertexNormalList[tri.v0] + triNormal;
            vertexNormalList[tri.v1] = vertexNormalList[tri.v1] + triNormal;
            vertexNormalList[tri.v2] = vertexNormalList[tri.v2] + triNormal;
        }

        //Normalise all vertex normals
        for (CRTVector& vertNormal : vertexNormalList) {
            vertNormal.normalize();
        }
    }

    float Mesh::intersect(const CRTRay& ray, InfoIntersect& intersectInfo, int& triIndex) const {
        //For each triangle index tuple, construct an actual triangle object and test for intersection
        float closestDist = FLT_MAX;
        InfoIntersect closestIntersect;
        intersectInfo.hasIntersection = false;

        int index = 0;
        for (TriProxy tri : triIndexList) {
            //ensure valid indices
            int vertexCount = vertexList.size();
            assert(tri.v0 < vertexCount && tri.v1 < vertexCount && tri.v2 < vertexCount);

            //construct actual triangle object and intersect
            CRTTriangle realTri(vertexList.at(tri.v0), vertexList.at(tri.v1), vertexList.at(tri.v2));
            float dist = realTri.intersect(ray, closestIntersect);

            //keep only closest intersection
            if (closestIntersect.hasIntersection && dist < closestDist) {
                closestDist = dist;
                intersectInfo = closestIntersect;
                triIndex = index;
            }
            index++;
        }

        //Return -1.0f if no intersection occurred
        return intersectInfo.hasIntersection ? closestDist : -1.0f;
    }

    int Mesh::getNumTriangles() const {
        return triIndexList.size();
    }
    int Mesh::getMatIndex() const {
        return matIndex;
    }
    const std::vector<CRTVector>& Mesh::getVertices() const {
        return vertexList;
    }
    const std::vector<TriProxy>& Mesh::getTriangles() const {
        return triIndexList;
    }
    const std::vector<CRTVector>& Mesh::getVertNormals() const {
        return vertexNormalList;
    }
}