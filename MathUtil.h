#pragma once

namespace ChaosCourse {

	static const float EPSILON = 1e-7f;

	static const float TO_RAD_CONST = 0.01745329251f; //pi/180
	static const float TO_DEG_CONST = 57.2957795131f; //180/pi


	float degToRad(float angle);

	float radToDeg(float angle);
}