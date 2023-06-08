#pragma once
#include "CRTVector.h"
#include "Matrix3x3.h"

namespace ChaosCourse {

	class CRTCamera {
	
	public:

		//Default camera is centred at the origin and is aligned to to the world-coordinate axis
		CRTCamera() :pos(), orientation(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f) {}

		//It is assumed that the orientation matrix is orthogonal. NO EXPLICIT VERIFICATION!
		CRTCamera(const CRTVector& pos, const Matrix3x3& orientation) : pos(pos), orientation(orientation) {};

		void dolly(float dist);

		void truck(float dist);

		void pedestal(float dist);
		
		void pan(float angle);

		void tilt(float angle);

		void roll(float angle);

		/* GETTERS */

		CRTVector getPosition() const { return pos; }
		Matrix3x3 getOrientation() const { return orientation; }

		//Get normalised forward vector - points in the direction the camera is looking at
		CRTVector getForwardVec() const;
		//Get normalised right vector - points to the right of the camera
		CRTVector getRightVec() const;
		//Get normalised up vector - points up from the camera 
		CRTVector getUpVec() const;

		/* SETTERS */

		void setPosition(const CRTVector& newPos);
		void setOrientation(const Matrix3x3& newOrientation);

		/* OTHER */

		//Makes the orientation matrix orthogonal. Indended as a combat tool against accumulated numerical error.
		void orthogonalize();

	private:
		//Position in 3d space 
		CRTVector pos;
		Matrix3x3 orientation;
	};
}