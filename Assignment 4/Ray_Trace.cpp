#include<fstream>
#include<iostream>
#include<cmath>

static const int imageWidth = 1920;
static const int imageHeight = 1080;

static const int maxColorComponent = 255;

class CRTVector {
	public:
		CRTVector(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z)  {}

		CRTVector operator+(const CRTVector& other) const{
			return CRTVector(x + other.x, y + other.y, z + other.z);
		}
		CRTVector operator-(const CRTVector& other) const{
			return CRTVector(x - other.x, y - other.y, z - other.z);
		}
		CRTVector operator*(float scalar) const {
			return CRTVector(x * scalar, y * scalar, z * scalar);
		}

		float dot(const CRTVector& other) const {
			return x * other.x + y * other. y + z * other.z;
		}

		float length() const{
			return std::sqrt(x*x + y*y + z*z);
		}
		CRTVector normalize() const {
			float len = length();
			return CRTVector(x / len, y / len, z / len);
		}
		float x, y, z;
};

class CRTRay {
	public:
		CRTRay(const CRTVector& origin, const CRTVector& direction):
		origin(origin), direction(direction){}

		CRTVector origin;
		CRTVector direction;
};

CRTVector rayTrace(const CRTRay& ray){
	CRTVector direction = ray.direction.normalize();

	float r = std::fabs(direction.x);
	float g = std::fabs(direction.y);
	float b = std::fabs(direction.z);

	return CRTVector(r, g, b) * maxColorComponent;
}

int main(){
	std::ofstream ppmFileStream("Vector_Trace.ppm");
	if(!ppmFileStream.is_open()){
		std::cerr << "Unable to open file for writing." << std::endl;
		return 1;
	}

	ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";


	//camera position
	CRTVector cameraPosition(0, 0, 0);

	// Distance between camera and image plane
	float distanceToImagePlane = 1;

	// Image plane dimension
	float imagePlaneWidth = 2;
	float imagePlaneHeight = (float) imageHeight / (float) imageWidth * imagePlaneWidth;

	// Loop throug every pixel in the image
	for ( int j = 0; j < imageHeight; j++){
		for(int i = 0; i < imageWidth; i++){

			// Compute ray direction for current pixel
			float x = i + 0.5;
			float y = j + 0.5;

			// Convert raster coordinates to NDC space
			x /= imageWidth;
			y /= imageHeight;

			// Convert NDC to screen space
			x = (2.0 * x) - 1.0;
			y = 1.0 - (2.0 * y);

			// Consider aspect ratio
			x *= imagePlaneWidth / imagePlaneHeight;

			//Ray direction
			CRTVector rayDirection(x, y, -distanceToImagePlane);
			rayDirection = rayDirection.normalize();

			CRTRay ray(cameraPosition, rayDirection);

			// Get color for current pixel 
			CRTVector color = rayTrace(ray);

			// Write pixel color to file
			int ir = (int)color.x;
			int ig = (int)color.y;
			int ib = (int)color.z;
			ppmFileStream << ir << " " << ig << " " << ib << " ";
		}
		ppmFileStream << std::endl;
	}
	ppmFileStream.close();
	return 0;
}