#include "CRTCamera.h"
#include "Matrix3x3.h"

namespace ChaosCourse {
	void CRTCamera::dolly(float dist) {
		//Move along forward vector
		pos = pos + getForwardVec() * dist;
	}

	void CRTCamera::truck(float dist) {
		//Move along right vector
		pos = pos + getRightVec() * dist;
	}

	void CRTCamera::pedestal(float dist) {
		//Move along up vector
		pos = pos + getUpVec() * dist;
	}

	void CRTCamera::pan(float angle) {
		//We apply a camera-centred transform (as opposed to world-centred transform) - we want
		//the rotation to be performed relative to the current camera coordinate frame.
		//We thus apply this rotation BEFORE any other transformations.
		//1. Invert current camera transforms (go back to the default position) - we assume orthogonal matrix;
		//2. Rotate about y axis
		//3. Re-apply previous camera transforms
		orientation = orientation * createRotationY(angle) * orientation.getTranspose();
	}

	void CRTCamera::tilt(float angle) {
		//We apply a camera-centred transform. Rotation is performed BEFORE any other transformations.
		//1. Invert current camera transforms
		//2. Rotate about x axis
		//3. Re-apply previous transforms
		orientation = orientation * createRotationX(angle) * orientation.getTranspose();
	}

	void CRTCamera::roll(float angle) {
		//We apply a camera-centred transform. Rotation is performed BEFORE any other transformations.
		//1. Invert current camera transforms
		//2. Rotate about z axis
		//3. Re-apply previous transforms
		orientation = orientation * createRotationZ(angle) * orientation.getTranspose();
	}

	CRTVector CRTCamera::getForwardVec() const {
		//Get normalized forward vector: -Z camera space axis
		CRTVector forward = -orientation.col(2);
		forward.normalize();
		return forward;
	}

	CRTVector CRTCamera::getRightVec() const {
		//Get normalized right vector: +X camera space axis
		CRTVector right = orientation.col(0);
		right.normalize();
		return right;
	}

	CRTVector CRTCamera::getUpVec() const {
		//Get normalized up vector: +Y camera space axis
		CRTVector up = orientation.col(1);
		up.normalize();
		return up;
	}

	void CRTCamera::setPosition(const CRTVector& newPos) {
		pos = newPos;
	}

	void CRTCamera::setOrientation(const Matrix3x3& newOrientation) {
		orientation = newOrientation;
	}

	void CRTCamera::orthogonalize() {
		orientation.orthogonalize();
	}
}