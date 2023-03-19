#include <fstream>

static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

int main() {
    std::ofstream ppmFileStream("superman_logo.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    const int diamondWidth = 440;
    const int diamondHeight = 200;
    const int triangleWidth = 300;
    const int triangleHeight = 200;
    const int shieldWidth = 780;
    const int shieldHeight = 660;
    const int shieldCenterX = 960;
    const int shieldCenterY = 540;

    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            if (colIdx >= shieldCenterX - shieldWidth/2 && colIdx <= shieldCenterX + shieldWidth/2 &&
                rowIdx >= shieldCenterY - shieldHeight/2 && rowIdx <= shieldCenterY + shieldHeight/2) {
                ppmFileStream << "255 0 0\t";
            } else if ((colIdx >= shieldCenterX - diamondWidth/2 && colIdx <= shieldCenterX + diamondWidth/2 &&
                        rowIdx >= shieldCenterY - diamondHeight/2 && rowIdx <= shieldCenterY + diamondHeight/2) ||
                       (colIdx >= shieldCenterX - triangleWidth/2 && colIdx <= shieldCenterX + triangleWidth/2 &&
                        rowIdx >= shieldCenterY && rowIdx <= shieldCenterY + triangleHeight)) {
                ppmFileStream << "255 255 255\t";
            } else {
                ppmFileStream << "0 0 255\t";
            }
        }
        ppmFileStream << "\n";
    }

    ppmFileStream.close();

    return 0;
}
