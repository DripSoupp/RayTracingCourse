#pragma once
#include<vector>
#include "CRTVector.h"

namespace ChaosCourse {

    //Forward-delcare
    class CRTRay;
    class InfoIntersect;

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
        //Construct an empty mesh.
        //Use 'vertexHint' and 'triangleHint' to reserve memory at construction time.
        // - 'vertexHint' = number of vertices to reserve.
        // - 'triangleHint' = number of triangle index tuples to reserve.
        Mesh(int vertexHint = 100, int triangleHint = 100);

        //Construct a mesh.
        // - 'vertices' must specify the list of vertices in the mesh.
        // - 'triangles' must specifiy the list of triangles (each triangle is given as a tuple of indices in the vertex list)
        // in the mesh.
        Mesh(const std::vector<CRTVector>& vertices, const std::vector<TriProxy>& triangles, int matIndex);

        //Add a vertex to the mesh.
        void pushVertex(const CRTVector& vert);

        //Add a triangle to the mesh. For memory optimisation purposes, the triangle is given as a 3-tuple of vertex indices.
        void pushTriangle(const TriProxy& tri);

        //Set material index
        void setMatIndex(int newMatIndex);

        //Uses the current information for mesh vertices and triangles to calculate a normal vector for each vertex.
        void recalculateNormals();

        //Intersect a ray with the given mesh. 
        // - Returns the distance from the ray origin to the closest intersection point if such exists. Modifies 'intersection'
        // to store the intersection point.
        // - Returns -1.0 (negative value) otherwise.
        float intersect(const CRTRay& ray, InfoIntersect& intersectInfo, int& triIndex) const;

        int getNumTriangles() const;
        int getMatIndex() const;

        const std::vector<CRTVector>& getVertices() const;
        const std::vector<TriProxy>& getTriangles() const;
        const std::vector<CRTVector>& getVertNormals() const;

    private:
        std::vector<CRTVector> vertexList;
        std::vector<CRTVector> vertexNormalList;
        std::vector<TriProxy> triIndexList;
        int matIndex;
    };

}