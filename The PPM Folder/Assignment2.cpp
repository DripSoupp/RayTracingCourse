#include <fstream>
#include <cstdlib>
#include <ctime>

struct Color {
    int red;
    int green;
    int blue;
};

static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

int main() {
    std::ofstream ppmFileStream("crt_output_image.ppm", std::ios::out | std::ios::binary);
    ppmFileStream << "P3\n";
    ppmFileStream << imageWidth << " " << imageHeight << "\n";
    ppmFileStream << maxColorComponent << "\n";

    const int numRects = 12;
    const int rectWidth = imageWidth/3 ;
    const int rectHeight = imageHeight/4;

   
    std::srand(std::time(nullptr));

    for (int rowIdx = 0; rowIdx < imageHeight; ++rowIdx) {
        for (int colIdx = 0; colIdx < imageWidth; ++colIdx) {
            Color color = { 255, 255, 255 }; 
            int rectIdx = (colIdx/rectWidth) + (rowIdx/rectHeight)*3;
            switch(rectIdx) {
                case 0:
                    color.red = std::rand() % 256;
                    color.green = 0;
                    color.blue = 0;
                    break;
                case 1:
                    color.red = 0;
                    color.green = std::rand() % 256;
                    color.blue = 0;
                    break;
                case 2:
                    color.red = 0;
                    color.green = 0;
                    color.blue = std::rand() % 256;
                    break;
                case 3:
                    color.red = std::rand() % 256;
                    color.green = std::rand() % 256;
                    color.blue = 0;
                    break;
                case 4:
                    color.red = std::rand() % 256;
                    color.green = 0;
                    color.blue = 0;
                    break;
                case 5:
                    color.red = std::rand() % 256;
                    color.green = std::rand() % 256;
                    color.blue = 0;
                    break;
                case 6:
                    color.red = std::rand() % 256;
                    color.green = 0;
                    color.blue = std::rand() % 256;
                    break;
                case 7:
                    color.red = 0;
                    color.green = 0;
                    color.blue = std::rand() % 256;
                    break;
                case 8:
                    color.red = 0;
                    color.green = std::rand() % 256;
                    color.blue = std::rand() % 256;
                    break;
                case 9:
                    color.red = 0;
                    color.green = std::rand() % 256;
                    color.blue = 0;
                    break;
                case 10:
                    color.red = 0;
                    color.green = std::rand() % 256;
                    color.blue = std::rand() % 256;
                    break;
                case 11:
                    color.red = std::rand() % 256;
                    color.green = std::rand() % 256;
                    color.blue = 0;
                    break;
            }
            ppmFileStream << color.red << " " << color.green << " " << color.blue << "\t";
        }
        ppmFileStream << "\n";
    }
    ppmFileStream.close();

    return 0;
}
