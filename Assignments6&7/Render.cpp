#include "Render.h"
#include "Scene.h"
#include "ColorRGB.h"
#include "CRTRay.h"
#include "Mesh.h"
#include <assert.h>

	ColorRGB colors[6] = {
		{111, 200, 232},
		{255,12, 232},
		{11, 4, 232},
		{145, 235, 63},
		{45, 255, 232},
		{16, 87, 85}
	};

	void ChaosCourse::Render::render(const Scene& scene, const std::string& filename) {

		const Settings& settings = scene.getSettings();
        int imageWidth = settings.getWidth();
        int imageHeight = settings.getHeight();
        float aspectRatio = settings.getAspectRatio();
        const CRTCamera& cam = scene.getCamera();
        const std::vector<Mesh>& meshes = scene.getMesh();

        //Set up output file stream
        std::ofstream ppmFileStream(filename, std::ios::out | std::ios::binary);
        assert(ppmFileStream.is_open());
        ppmFileStream << "P3\n";
        ppmFileStream << imageWidth << " " << imageHeight << "\n";
        ppmFileStream << ColorRGB::maxColorComponent << "\n";

        //Loop through pixels and shoot camera rays
        for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
            for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
                CRTRay ray = computeCameraRay(colIdx, rowIdx, imageHeight, aspectRatio, cam);

                //intersect
                CRTVector intersection;
                int triIdx = 0;
                float dist = findIntersection(ray, meshes, intersection, triIdx);

                if (dist > 0.0f) {
                    //intersection occured - colour pixel based on index
                    ColorRGB triColor = colors[triIdx % 6];
                    ppmFileStream << (int)triColor.r << " " << (int)triColor.g << " " << (int)triColor.b << "\t";
                }
                else {
                    //no intersection - use background colour instead
                    ColorRGB bgColor = settings.getBgColor();
                    ppmFileStream << (int)bgColor.r << " " << (int)bgColor.g << " " << (int)bgColor.b << "\t";
                }

            }
            ppmFileStream << "\n";
        }

        ppmFileStream.close();
    }
    float ChaosCourse::Render::findIntersection(const CRTRay& ray, const std::vector<Mesh>& meshes, CRTVector intersection, int& triIdx){
        float closestDist = FLT_MAX;
        CRTVector closestIntersect;

        //Loop through all meshes and find closest intersection
        CRTVector intersect;
        for (const Mesh& mesh : meshes) {
            float dist = mesh.intersection(ray, intersect, triIdx);
            if (dist > 0.0f && dist < closestDist) {
                closestDist = dist;
                closestIntersect = intersect;
            }
        }

        if (closestDist < FLT_MAX) {
            //intersection occured
            intersection = closestIntersect;
            return closestDist;
        }
        else {
            //no intersection
            return -1.0f;
        }
    }

    