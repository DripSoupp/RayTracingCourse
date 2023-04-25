#pragma once
#include "CRTVector.h"
#include "CRTMatrix.h"

namespace ChaosCourse {

	class CRTCamera {
	public:
		//Default camera is centred at the origin and is aligned to to the world-coordinate axis
		CRTCamera() :pos(), orientation(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f) {}

		//It is assumed that the orientation matrix is orthogonal. NO EXPLICIT VERIFICATION!
		CRTCamera(const CRTVector& pos, const CRTMatrix& orientation) : pos(pos), orientation(orientation) {};



		void dolly(float dist);

		void truck(float dist);

		void pedestal(float dist);



		void pan(float angle);

		void tilt(float angle);

		void roll(float angle);


		CRTVector getPosition() const { return pos; }
		CRTMatrix getOrientation() const { return orientation; }

		CRTVector getForwardVec() const;
		CRTVector getRightVec() const;
		CRTVector getUpVec() const;

		void setPosition(const CRTVector& newPos);
		void setOrientation(const CRTMatrix& newOrientation);



		void orthogonalize();

	private:
		CRTVector pos;

		CRTMatrix orientation;
	};
}