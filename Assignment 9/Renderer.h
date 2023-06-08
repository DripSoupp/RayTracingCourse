#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace ChaosCourse {

    //Forward declarations
    class Scene;
    class CRTRay;
    class Mesh;
    class Material;
    class CRTCamera;
    class CRTVector;
    class ColorRGB;
    class InfoIntersect;

    //Shading mode
    enum class ShadingMode { Light, Barycentric };

    
    class Renderer {
    public:
        Renderer() {} //empty constructor

        //Render a scene to a .ppm file with the given filename (newly created).
        void render(const Scene& scene, const std::string& filename, const ShadingMode& shadingMode);

    private:
        
        CRTVector rayTrace(const CRTRay& ray, int depth, ShadingMode shadingMode, const Scene& scene);
        
        float findIntersection(const CRTRay& ray, const std::vector<Mesh>& meshes, InfoIntersect& intersectInfo,
            int& meshIndex, int& triIndex);

        CRTVector extractHitNormal(const std::vector<Mesh>& meshes, const InfoIntersect& intersectInfo, int meshIndex, int triIndex);

        //Perform Lambertian shading on a given point. 
        //Diffuse lighting for now.
        CRTVector shadeLambertian(const CRTVector& point, const CRTVector& normal, const CRTVector& albedo, const Scene& scene);

        //Color point based on its barycentric coordinates. No lights required.
        CRTVector shadeBarycentric(float coords[3]);

    };
}
