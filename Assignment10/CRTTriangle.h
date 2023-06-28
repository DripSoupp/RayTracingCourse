#ifndef CRTTRIANGLE_H
#define CRTTRIANGLE_H

#include <array>
#include <cstdint>
#include <vector>
#include "AABBox.h"
#include "CRTVector.h"
#include "CRTRay.h"

using TriangleIndices = std::array<int, 3>;

class Material;

// Data for ray-triangle intersection
struct InfoIntersect {
    CRTVectorf pos;          
    Normalf faceNormal;   
    Normalf smoothNormal; 
    float t = MAX_FLOAT;  
    float u, v;             // Barycentric coordinates
    int32_t materialIdx;   
};

struct TriangleMesh;

struct Triangle {
    const int* indices;        ///< Indices of the triangle's vertices in the mesh
    const TriangleMesh* mesh;  ///< The triangle's owner mesh

    Triangle() = delete;

    Triangle(const TriangleIndices& _indices, const TriangleMesh* _mesh)
        : indices(_indices.data()), mesh(_mesh) {};

    /// @brief Verifies if ray intersect with the triangle
    bool intersect(const CRTRay& ray, InfoIntersect& info) const;

    /// @brief Verifies if ray intersect with the triangle using Moller-Trumbor method
    bool intersectMT(const CRTRay& ray, InfoIntersect& info) const;
};

/// @brief Triangle mesh class that stores information for each object in the scene
struct TriangleMesh {
    std::vector<Pointf> vertPositions;        // Positions of the vertices in world space
    std::vector<TriangleIndices> vertIndices;  //  Keeps indices for each triangle in the mesh
    std::vector<Normalf> vertNormals;         // Pre-computed normals for each vertex in the mesh
    int32_t materialIdx;  // Index from the materials list that characterise current object (mesh)
    BBox bounds;          // The bounding box of the mesh

    TriangleMesh() = delete;

    // Initializes triangle mesh from vertex positions, vertex indices, and material index
    TriangleMesh(const std::vector<Pointf>& _vertPositions,
        const std::vector<TriangleIndices>& _vertIndices, const int32_t _materialIdx);

    // Retrieves list of all triangles in the mesh upon request
    std::vector<Triangle> getTriangles() const;

    // Intersects ray with the mesh and records closest intersection point if any
    bool intersect(const CRTRay& ray, InfoIntersect& info) const;

    // Verifies if ray intersects with the mesh. Returns true on first intersection, false
    // if no ray-triangle intersection found
    bool intersectPrim(const CRTRay& ray, InfoIntersect& info) const;
};

#endif  