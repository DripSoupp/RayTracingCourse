#include <fstream>
#include <cmath>
#include <iostream>

static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

class CRTVector {
public:
	CRTVector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

	CRTVector operator+(const CRTVector& other) const {
		return CRTVector(x + other.x, y + other.y, z + other.z);
	}

	CRTVector operator-(const CRTVector& other) const {
		return CRTVector(x - other.x, y - other.y, z - other.z);
	}

	CRTVector operator*(float scalar) const {
		return CRTVector(x * scalar, y * scalar, z * scalar);
	}

	float dot(const CRTVector& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	float length() const {
		return std::sqrt(x * x + y * y + z * z);
	}

	CRTVector normalize() const {
		float len = length();
		return CRTVector(x / len, y / len, z / len);
	}

	float x, y, z;
};

class CRTRay {
public:
	CRTRay(const CRTVector& origin, const CRTVector& direction) : origin(origin), direction(direction) {}

	CRTVector origin;
	CRTVector direction;
};

CRTVector rayTrace(const CRTRay& ray) {
	CRTVector direction = ray.direction.normalize();

	float r = std::fabs(direction.x);
	float g = std::fabs(direction.y);
	float b = std::fabs(direction.z);

	return CRTVector(r, g, b) * maxColorComponent;
}

int main() {
	std::ofstream ppmFileStream("Vector_trace.ppm");
	if (!ppmFileStream.is_open()) {
		std::cerr << "Unable to open file for writing." << std::endl;
		return 1;
	}

	ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";

	// Camera position
	CRTVector cameraPosition(0, 0, -1);

	// Distance between camera and image plane
	float distanceToImagePlane = 1;

	// Image plane dimensions
	float imagePlaneWidth = 2;
	float imagePlaneHeight = (float)imageHeight / (float)imageWidth * imagePlaneWidth;

	// Image plane top left corner
	CRTVector imagePlaneTopLeftCorner(
		-cameraPosition.x - imagePlaneWidth / 2,
		-cameraPosition.y + imagePlaneHeight / 2,
		-cameraPosition.z - distanceToImagePlane
	);

	// Loop through every pixel in the image
	for (int j = 0; j < imageHeight; j++) {
		for (int i = 0; i < imageWidth; i++) {

			// Compute ray direction for current pixel
			float x = (float)i / (float)imageWidth;
			float y = (float)j / (float)imageHeight;

			CRTVector pixelPosition(
				imagePlaneTopLeftCorner.x + x * imagePlaneWidth,
				imagePlaneTopLeftCorner.y - y * imagePlaneHeight,
				imagePlaneTopLeftCorner.z
			);

			CRTRay ray(cameraPosition, pixelPosition - cameraPosition);

			// color of the pixel by tracing the ray
			CRTVector color = rayTrace(ray);

			ppmFileStream << (int)color.x << " " << (int)color.y << " " << (int)color.z << " ";
		}

		ppmFileStream << std::endl;
	}

	// Close the file
	ppmFileStream.close();

	return 0;
}