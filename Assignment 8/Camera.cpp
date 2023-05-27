#include "Camera.h"
#include "Matrix3x3.h"

namespace ChaosCourse {
	void Camera::dolly(float dist) {
		//Move along forward vector
		pos = pos + getForwardVec() * dist;
	}

	void Camera::truck(float dist) {
		//Move along right vector
		pos = pos + getRightVec() * dist;
	}

	void Camera::pedestal(float dist) {
		//Move along up vector
		pos = pos + getUpVec() * dist;
	}

	void Camera::pan(float angle) {
		//1. Invert current camera transforms (go back to the default position) - we assume orthogonal matrix;
		//2. Rotate about y axis
		//3. Re-apply previous camera transforms
		orientation = orientation * createRotationY(angle) * orientation.getTranspose();
	}

	void Camera::tilt(float angle) {
		//1. Invert current camera transforms
		//2. Rotate about x axis
		//3. Re-apply previous transforms
		orientation = orientation * createRotationX(angle) * orientation.getTranspose();
	}

	void Camera::roll(float angle) {
		//1. Invert current camera transforms
		//2. Rotate about z axis
		//3. Re-apply previous transforms
		orientation = orientation * createRotationZ(angle) * orientation.getTranspose();
	}

	CRTVector Camera::getForwardVec() const {
		//Get normalized forward vector: -Z camera space axis
		CRTVector forward = -orientation.col(2);
		forward.normalize();
		return forward;
	}

	CRTVector Camera::getRightVec() const {
		//Get normalized right vector: +X camera space axis
		CRTVector right = orientation.col(0);
		right.normalize();
		return right;
	}

	CRTVector Camera::getUpVec() const {
		//Get normalized up vector: +Y camera space axis
		CRTVector up = orientation.col(1);
		up.normalize();
		return up;
	}

	void Camera::setPosition(const CRTVector& newPos) {
		pos = newPos;
	}

	void Camera::setOrientation(const Matrix3x3& newOrientation) {
		orientation = newOrientation;
	}

	void Camera::orthogonalize() {
		orientation.orthogonalize();
	}
}