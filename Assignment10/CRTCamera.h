#ifndef CRTCAMERA_H
#define CRTCAMERA_H

#include "Matrix3x3.h"


class CRTCamera {
public:
    CRTCamera() = default;

    CRTCamera(const Pointf& _loofFrom, const Pointf& _lookAt, const int _imageWidth = IMG_WIDTH,
        const int _imageHeight = IMG_HEIGHT)
        : lookFrom(_loofFrom), lookAt(_lookAt), imageWidth(_imageWidth), imageHeight(_imageHeight) {
        const CRTVectorf vUp{ 0.f, 1.f, 0.f }; 
        const CRTVectorf zVec = (lookFrom - lookAt).normalize();
        const CRTVectorf xVec = cross(vUp, zVec).normalize();
        const CRTVectorf yVec = cross(zVec, xVec);
        aspectRatio = imageWidth / (float)imageHeight;

        rotationM.m[0][0] = xVec.x;
        rotationM.m[0][1] = xVec.y;
        rotationM.m[0][2] = xVec.z;
        rotationM.m[1][0] = yVec.x;
        rotationM.m[1][1] = yVec.y;
        rotationM.m[1][2] = yVec.z;
        rotationM.m[2][0] = zVec.x;
        rotationM.m[2][1] = zVec.y;
        rotationM.m[2][2] = zVec.z;
    }

    void init(const Pointf& _lookFrom, const Matrix3x3& _rotationM, const int _imageWidth,
        const int _imageHeight, const Pointf& _lookAt = { 0.f, 0.f, -1.f }) {
        lookFrom = _lookFrom;
        lookAt = _lookAt;
        rotationM = _rotationM;
        imageWidth = _imageWidth;
        imageHeight = _imageHeight;
        aspectRatio = imageWidth / (float)imageHeight;
    }

    CRTRay getRay(const uint32_t x, const uint32_t y) const {
        const float ndcX = (y + 0.5f) / imageWidth;
        const float ndcY = (x + 0.5f) / imageHeight;
        const float screenX = (2.f * ndcX - 1.f) * aspectRatio;
        const float screenY = (1.f - 2.f * ndcY);
        const CRTVectorf rayDir(screenX, screenY, -1);
        return CRTRay(lookFrom, (rayDir * rotationM).normalize());
    }

    void truck(const float sidewayStep) { lookFrom += rotationM * CRTVectorf(sidewayStep, 0.f, 0.f); }

    void boom(const float upDownStep) { lookFrom += rotationM * CRTVectorf(0.f, upDownStep, 0.f); }

    void dolly(const float frontBackStep) {
        lookFrom += rotationM * CRTVectorf(0.f, 0.f, frontBackStep);
    }

    void move(const CRTVectorf& moveV) { lookFrom += rotationM * moveV; }

    void tilt(const float thetaDeg) {
        const Matrix3x3 xAxisRotationMatrix = rotateX(thetaDeg);
        rotationM = rotationM * xAxisRotationMatrix;
    }

    void pan(const float thetaDeg) {
        const Matrix3x3 yAxisRotationMatrix = rotateY(thetaDeg);
        rotationM = rotationM * yAxisRotationMatrix;
    }

    void roll(const float thetaDeg) {
        const Matrix3x3 zAxisRotationMatrix = rotateZ(thetaDeg);
        rotationM = rotationM * zAxisRotationMatrix;
    }

    CRTVectorf getLookFrom() const { return lookFrom; }

    CRTVectorf getLookAt() const { return lookAt * rotationM; }

    Matrix3x3 getRotationMatrix() const { return rotationM; }

private:
    Pointf lookFrom;     
    Pointf lookAt;       
    Matrix3x3 rotationM;  
    int imageWidth;      
    int imageHeight;      
    float aspectRatio;   
};

#endif  