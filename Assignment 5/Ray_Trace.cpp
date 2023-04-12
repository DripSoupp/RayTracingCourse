#include<fstream>
#include<iostream>
#include<cmath>
#include <vector>

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
		CRTVector cross(const CRTVector& other) const {
			return CRTVector (y * other.z - z * other.y, z* other.x - x * other.z, x * other.y - y * other.x);
		}
		
		float x, y, z;
};


class CRTRay {
public:
	CRTRay() {}
	
    CRTRay(CRTVector direction, CRTVector origin) : direction(direction), origin(origin) {}
       
	   

		CRTRay generateCameraRay(float colIdx, float rowIdx, float imageWidth, float imageHeight, 
                     float imagePlaneWidth, float imagePlaneHeight, float distanceToImagePlane, 
                     const CRTVector& cameraPosition) {

		CRTRay ray;

        float x = colIdx + 0.5;
        float y = rowIdx + 0.5;

        // Convert raster coordinates to NDC space
        x /= imageWidth;
        y /= imageHeight;

        // Convert NDC to screen space
        x = (2.0 * x) - 1.0;
        y = 1.0 - (2.0 * y);

        // Consider aspect ratio
        x *= (imagePlaneWidth / imagePlaneHeight);

        // Ray direction
        ray.direction.x = x;
		ray.direction.y = y;
		ray.direction.z = -distanceToImagePlane;
		ray.direction.normalize();

        ray.origin = cameraPosition;

		return ray;
    }
	CRTVector origin;
    CRTVector direction;
};



class CRTTriangle {
	public: 
	CRTTriangle(CRTVector v1, CRTVector v2, CRTVector v3) : v1(v1), v2(v2), v3(v3){}

	CRTVector v1, v2, v3;
	

	CRTVector normal() const {
		CRTVector edge1 = v2 - v1;
		CRTVector edge2 = v3 - v1;
		return edge1.cross(edge2).normalize();
	}

	float area() const {
		CRTVector edge1 = v2 - v1;
		CRTVector edge2 = v3 - v1;
		return edge1.cross(edge2).length();
	}

};

CRTVector rayTrace(const CRTRay& ray){
	CRTVector direction = ray.direction.normalize();

	float r = std::fabs(direction.x);
	float g = std::fabs(direction.y);
	float b = std::fabs(direction.z);

	return CRTVector(r, g, b) * maxColorComponent;
}



CRTVector cross(const CRTVector& v1, const CRTVector& v2) {
    return CRTVector(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}


bool triangleIntersection(const CRTRay& ray, const CRTTriangle& triangle, float& t)
{
    const CRTVector& V0 = triangle.v1;
    const CRTVector& V1 = triangle.v2;
    const CRTVector& V2 = triangle.v3;

    CRTVector E0 = V1 - V0;
    CRTVector E1 = V2 - V1;
    CRTVector P = ray.direction.cross(E1);
    float det = E0.dot(P);

    // ray and triangle are parallel if det is close to 0
	float epsilon = 0.000001f;
    if (fabs(det) < epsilon) {
        return false;
    }

    float invDet = 1 / det;

    CRTVector T = ray.origin - V0;
    float u = T.dot(P) * invDet;

    if (u < 0 || u > 1) {
        return false;
    }

    CRTVector Q = T.cross(E0);
    float v = ray.direction.dot(Q) * invDet;

    if (v < 0 || u + v > 1) {
        return false;
    }

    t = E1.dot(Q) * invDet;

    return true;
}



int main(){
	std::ofstream ppmFileStream("Vector_Trace.ppm");
	if(!ppmFileStream.is_open()){
		std::cerr << "Unable to open file for writing." << std::endl;
		return 1;
	}
	
	ppmFileStream << "P3\n" << imageWidth << " " << imageHeight << "\n" << maxColorComponent << "\n";

	std::vector<CRTTriangle> triangles;
	triangles.reserve(10);
	const CRTRay ray = CRTRay();
	const CRTTriangle triangle0{
		CRTVector(0, -0.5, -3),
		CRTVector(2, -0.5, -3),
		CRTVector(2, 1.75, -3)

	};
	const CRTTriangle triangle1{
		CRTVector(0, -0.5, -3),
		CRTVector(2, -0.5, -3),
		CRTVector(2, -2.5, -3)
	};
	
	const CRTTriangle triangle2 {
		CRTVector(0, -0.5, -3),
		CRTVector(-2, -0.5, -3),
		CRTVector(-2, -2.5, -3)
	};
	const CRTTriangle triangle3 { 
		CRTVector(0, -0.5, -3),
		CRTVector(-2, -0.5, -3),
		CRTVector(-2, 1.75, -3)
	};
	
	
	
	for (const CRTTriangle& triangle : triangles) {
		float t;
    triangleIntersection(ray, triangle, t); // pass each triangle object to the function
	}

	
	triangles.push_back(triangle0);
	triangles.push_back(triangle1);
	triangles.push_back(triangle2);
	triangles.push_back(triangle3);

	
	
	//camera position
	CRTVector cameraPosition(0, 0, 0);

	// Distance between camera and image plane
	float distanceToImagePlane = 1;

	// Image plane dimension
	float imagePlaneWidth = 2;
	float imagePlaneHeight = (float) imageHeight / (float) imageWidth * imagePlaneWidth;
	
	// Loop throug every pixel in the image
	for ( int rowIdx = 0; rowIdx < imageHeight; rowIdx++){
		for(int colIdx = 0; colIdx < imageWidth; colIdx++){
			CRTRay ray = ray.generateCameraRay(colIdx, rowIdx, imageWidth, imageHeight, imagePlaneWidth, imagePlaneHeight, distanceToImagePlane, cameraPosition);
            float t;
            bool hit = false;

            // Check for intersection with all triangles
            for (const CRTTriangle& triangle : triangles) {
                if (triangleIntersection(ray, triangle, t)) {
                    CRTVector color = rayTrace(ray);
                    int r = static_cast<int>(color.x);
                    int g = static_cast<int>(color.y);
                    int b = static_cast<int>(color.z);
                    ppmFileStream << r << " " << g << " " << b << " ";
                    hit = true;
                    break;  // Break out of the loop after first intersection
                }
            }

            // If no intersection, write black color
            if (!hit) {
                ppmFileStream << "0 0 0 ";
            }
        
		ppmFileStream << "\n";
	}
	}
	ppmFileStream.close();
	return 0;
}