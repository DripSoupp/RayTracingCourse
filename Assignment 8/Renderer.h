#pragma once
#include<string>
#include<fstream>
#include<vector>

namespace ChaosCourse {

    //Forward declarations
    class Scene;
    class CRTRay;
    class Mesh;
    class CRTVector;
    class ColorRGB;

    /*
    * A Ray-Tracing Renderer. Takes a scene description and renders an image file.
    * De-coupled from any scene data - a universal renderer that can be applied to many different scenes.
    */
    class Renderer {
    public:
        Renderer() {} //empty constructor

        //Render a scene to a .ppm file with the given filename (newly created).
        void render(const Scene& scene, const std::string& filename);

    private:
        //Find the closest intersection point (if any) of a ray with a collection of meshes.
        // - Returns distance to closest intersection. Intersection point stored in 'intersection'.
        // - If no intersection found, returns -1.0.
        float findIntersection(const CRTRay& ray, const std::vector<Mesh>& meshes, CRTVector& intersection, CRTVector& triNormal);

        //Perform shading on a given point. 
        //Diffuse lighting for now.
        ColorRGB shade(const CRTVector& point, const CRTVector& triNormal, const Scene& scene);
    };
}