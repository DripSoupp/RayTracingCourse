#include "CRTCamera.h"
#include "CRTMatrix.h"

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
		
		orientation = orientation * createRotationY(angle) * orientation.getTranspose();
	}

	void CRTCamera::tilt(float angle) {
		
		orientation = orientation * createRotationX(angle) * orientation.getTranspose();
	}

	void CRTCamera::roll(float angle) {
		
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

	void CRTCamera::setOrientation(const CRTMatrix& newOrientation) {
		orientation = newOrientation;
	}

	void CRTCamera::orthogonalize() {
		orientation.orthogonalize();
	}
}