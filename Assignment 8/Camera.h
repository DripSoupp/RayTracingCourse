#pragma once
#include "CRTVector.h"
#include "Matrix3x3.h"

namespace ChaosCourse {

	
	class Camera {
	public:
		//Default camera is centred at the origin and is aligned to to the world-coordinate axis
		Camera() :pos(), orientation(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f) {}

		//It is assumed that the orientation matrix is orthogonal. NO EXPLICIT VERIFICATION!
		Camera(const CRTVector& pos, const Matrix3x3& orientation) : pos(pos), orientation(orientation) {};


		//Move the camera forward-backward (along Z-axis).
		//Positive distance = move camera forward.
		void dolly(float dist);

		//Move the camera left-right (along X-axis)
		//Positive distance = move camera right
		void truck(float dist);

		//Move the camera up-down (along Y-axis)
		//Positive distatnce = move camera up
		void pedestal(float dist);


		//Rotate the camera left-right (about Y-axis), aka "yaw" angle
		//Positive angle = rotate camera left
		//Angle must be given in radians.
		void pan(float angle);

		//Rotate the camera up-down (about X axis), aka "pitch" angle
		//Positive angle = rotate camera up
		//Angle must be given in radians.
		void tilt(float angle);

		//Rotate the camera about Z-axis
		//Positive angle = "left side of camera plunges down, right side goes up"
		//Angle must be given in radians.
		void roll(float angle);



		CRTVector getPosition() const { return pos; }
		Matrix3x3 getOrientation() const { return orientation; }

		//Get normalised forward vector - points in the direction the camera is looking at
		CRTVector getForwardVec() const;
		//Get normalised right vector - points to the right of the camera
		CRTVector getRightVec() const;
		//Get normalised up vector - points up from the camera 
		CRTVector getUpVec() const;


		void setPosition(const CRTVector& newPos);
		void setOrientation(const Matrix3x3& newOrientation);


		//Makes the orientation matrix orthogonal. Indended as a combat tool against accumulated numerical error.
		void orthogonalize();

	private:
		//Position in 3d space 
		CRTVector pos;

		Matrix3x3 orientation;
	};
}