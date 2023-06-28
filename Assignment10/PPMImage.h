#ifndef PPMIMAGE_H
#define PPMIMAGE_H

#include <cstdint>
#include <iostream>
#include <vector>
#include "CRTVector.h"

// Template structure for representing a PPM image
template <typename T>
struct PPMImage {
    // Union to hold the pixel data
    union alignas(4 * alignof(T)) Pixel {
        CRTVector<T> color;  // Color vector representing RGB values
        struct {
            T r, g, b;  // Individual color components
        };

        Pixel() : color() {}
    };

    PPMImage() = delete;

    // Constructor that takes the image width and height
    PPMImage(const int imageWidth, const int imageHeight) : data(imageWidth* imageHeight) {}

    std::vector<Pixel> data;  // Vector to store the pixel data of the image
};

typedef PPMImage<float> PPMImageF;  // PPMImage specialization for floating-point values
typedef PPMImage<int> PPMImageI;    // PPMImage specialization for integer values
typedef PPMImageF::Pixel PPMPixelF; // Pixel specialization for floating-point image
typedef PPMImageI::Pixel PPMPixelI; // Pixel specialization for integer image

// Function to serialize a PPMImage and write it to an output stream
inline static void serializePPMImage(std::ostream& outputStream, const PPMImageI& ppmImage) {
    outputStream << "P3\n";                          // PPM image format identifier
    outputStream << IMG_WIDTH << " " << IMG_HEIGHT << "\n"; // Image width and height
    outputStream << MAX_COLOR_COMP << "\n";          // Maximum color component value

    // Loop through each pixel and write its RGB values to the output stream
    for (int c = 1, row = 1; const PPMPixelI & pixel : ppmImage.data) {
        outputStream << pixel.r << " " << pixel.g << " " << pixel.b << " ";

        // Add a new line character after writing each row of pixels
        if (c == IMG_WIDTH * row) {
            outputStream << "\n";
            row++;
        }
        c++;
    }
}

#endif
