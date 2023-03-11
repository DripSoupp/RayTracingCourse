#include <fstream>
#include <cmath>

static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

int main() {
    std::ofstream ppmFileStream("crt_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    // size and position of the circles
    const int x1 = 700;
    const int y1_int = 500;
    const double y1 = 500.0;
    const int radius1 = 150;

    const int x2 = 1300;
    const int y2_int = 500;
    const double y2 = 500.0;
    const int radius2 = 150;

    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            int distToCenter1 = std::sqrt((colIdx - x1) * (colIdx - x1) + (rowIdx - y1_int) * (rowIdx - y1_int));
            if (distToCenter1 <= radius1) {
                ppmFileStream << "255 0 255\t";
            } else {
                int distToCenter2 = std::sqrt((colIdx - x2) * (colIdx - x2) + (rowIdx - y2_int) * (rowIdx - y2_int));
                if (distToCenter2 <= radius2) {
                    ppmFileStream << "255 0 255\t";
                } else {
                    // calculating the curve
                    double curveY = 0.01 * std::pow(colIdx - 960.0, 2) + 800.0;
                    if (rowIdx >= curveY) {
                        ppmFileStream << "255 0 0\t"; 
                    } else {
                        ppmFileStream << "255 255 255\t"; 
                    }
                }
            }
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    return 0;
}
