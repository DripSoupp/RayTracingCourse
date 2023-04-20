#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <utility>
#include <string>

#include "CRTVector.h"
#include "CRTMatrix.h"
#include "MathUtil.h"
#include "CRTTriangle.h"
#include "CRTRay.h"
#include "CRTCamera.h"

using namespace ChaosCourse;

static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

struct ColorRGB {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    ColorRGB() : r(0), g(0), b(0) {}
    ColorRGB(unsigned char red, unsigned char green, unsigned char blue) : r(red), g(green), b(blue) {}
};

void setUpScene(std::vector<std::pair<CRTTriangle, ColorRGB>>& triangleList);

int main() {

    //Set up Scene
    std::vector<std::pair<CRTTriangle, ColorRGB>> triangles;
    triangles.reserve(36);
    setUpScene(triangles);

    CRTCamera cam;
    //Task 1 & 2 
    //cam.truck(2.0f);
    //cam.pan(degToRad(30.0f));

    //Task 3 
    //cam.roll(degToRad(30.0f));

    //Task 4 
    //cam.dolly(-5.0f);
    //cam.pedestal(7.0f);
    //cam.tilt(-45.0f);
    //cam.dolly(-1.0f);

    float aspectRatio = (float)imageWidth / imageHeight;

    //Task 5 - Produce 72 frames
    for (int i = 0; i < 1; i++) {
        //Set up output stream. Ensure file naming will produce proper file ordering.
        std::string zeroFill = "0";
        if (i > 9) zeroFill = "";
        std::ofstream ppmFileStream("Frame_#" + zeroFill + std::to_string(i) + ".ppm",
            std::ios::out | std::ios::binary);
        ppmFileStream << "P3\n";
        ppmFileStream << imageWidth << " " << imageHeight << "\n";
        ppmFileStream << maxColorComponent << "\n";

        //Task 5 - Pan camera by 5 deg every frame
        cam.pan(degToRad(i * 5.0f));

        for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
            for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
                CRTRay ray = computeCameraRay(colIdx, rowIdx, imageHeight, aspectRatio, cam);

                float closestDist = FLT_MAX;
                ColorRGB finalColor(51, 0, 51); //background color

                for (const auto& tri : triangles) {
                    CRTVector intersection;
                    float dist = tri.first.intersect(ray, intersection);
                    if (dist > 0.0 && dist < closestDist) {
                        closestDist = dist;
                        finalColor = tri.second;
                    }
                }

                ppmFileStream << (int)finalColor.r << " " << (int)finalColor.g << " " << (int)finalColor.b << "\t";
            }
            ppmFileStream << "\n";
        }

        ppmFileStream.close();

        if (i % 10 == 0) {
            //Re-orthogonalize camera orientation matrix to ensure that it stays orthogonal
            cam.orthogonalize();
        }
    }

    return 0;
}

void setUpScene(std::vector<std::pair<CRTTriangle, ColorRGB>>& triangleList) {
    //Tasks 1 & 2
    /*triangleList.push_back(std::make_pair(
        CRTTriangle(
            CRTVector(-1.75, -1.75, -3),
            CRTVector(1.75, -1.75, -3),
            CRTVector(0, 1.75, -3)),
        ColorRGB(255, 255, 100)));*/
//}

    //Task 3 - 5
    float sqrt2 = sqrtf(2);
    CRTVector a = CRTVector(-sqrt2, -2.0f, -10.0f);
    CRTVector b = CRTVector(sqrt2, -2.0f, -10.0f);
    CRTVector c = CRTVector(0.0f, 3.0f, -10.0f);
    CRTVector m = CRTVector(0.0f, 0.0f, -10.0f);

    CRTVector aFlip = CRTVector(-sqrt2, 3.0f, -10.0f);
    CRTVector bFlip = CRTVector(sqrt2, 3.0f, -10.0f);
    CRTVector cFlip = CRTVector(0.0f, -2.0f, -10.0f);
    CRTVector mFlip = CRTVector(0.0f, 2.0f, -10.0f);

    int numSections = 24;
    float angle = 360.0f / numSections;
    for (int i = 0; i < numSections; i++) {
        CRTMatrix rot = createRotationY(degToRad(angle * i));

        int coeff = (abs(i - numSections / 2) - numSections / 2) * 10;

        if (i % 2 == 0) {
            CRTTriangle tri1(rot * a, rot * m, rot * c);
            CRTTriangle tri2(rot * a, rot * b, rot * m);
            CRTTriangle tri3(rot * b, rot * c, rot * m);
            triangleList.push_back(std::make_pair(tri1, ColorRGB(255, 0, 100)));
            triangleList.push_back(std::make_pair(tri2, ColorRGB(255, 0 + coeff, 200 + coeff)));
            triangleList.push_back(std::make_pair(tri3, ColorRGB(255, 50 + coeff, 0 + coeff)));
        }
        else {
            CRTTriangle tri1(rot * aFlip, rot * cFlip, rot * mFlip);
            CRTTriangle tri2(rot * aFlip, rot * mFlip, rot * bFlip);
            CRTTriangle tri3(rot * bFlip, rot * mFlip, rot * cFlip);
            triangleList.push_back(std::make_pair(tri1, ColorRGB(255, 0, 100)));
            triangleList.push_back(std::make_pair(tri2, ColorRGB(255,  + coeff, 200 + coeff)));
            triangleList.push_back(std::make_pair(tri3, ColorRGB(255, 50 + coeff, 0 + coeff)));
        }
    }

}