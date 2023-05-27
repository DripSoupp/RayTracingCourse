#pragma once
#include<vector>
#include "CRTVector.h"

namespace ChaosCourse {

    //Forward-delcare
    class CRTRay;

    //A 3-tuple of vertex indices.
    //Used instead of a Triangle in the mesh class to avoid duplicated vertices. 
    struct TriProxy {
        int v0;
        int v1;
        int v2;
        TriProxy(int vert0, int vert1, int vert2) :v0(vert0), v1(vert1), v2(vert2) {}
    };

    /*
    * A geometry mesh, composed of triangles.
    */
    class Mesh {
    public:
        
        Mesh(int vertexHint = 100, int triangleHint = 100);

       
        Mesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles);

        //Add a vertex to the mesh.
        void pushVertex(const CRTVector& vert);

        //Add a triangle to the mesh. For memory optimisation purposes, the triangle is given as a 3-tuple of vertex indices.
        void pushTriangle(const TriProxy& tri);

       
        float intersect(const CRTRay& ray, CRTVector& intersection, CRTVector& triNormal) const;

        int getNumTriangles() const;

    private:
        std::vector<CRTVector> vertexList;
        std::vector<TriProxy> triIndexList;

    };

}