#ifndef CAMERA_H
#define CAMERA_H

#include "triple.h"

class Camera
{
public:
	Camera()
	{
		apertureRadius = 0.0;
		apertureSamples = 1;
		velocity = Vector(0, 0, 0);
		acceleration = Vector(0, 0, 0);
		exposureTime = 0.0;
		exposureSamples = 1;
		anaglyph = false;
		eyesOffset = Vector(0, 0, 0);
	}

	Vector eye;
	Vector center;
	Vector up;
	int viewWidth;
	int viewHeight;
	double apertureRadius;
	unsigned int apertureSamples;
	Vector velocity;
	Vector acceleration;
	double exposureTime;
	unsigned int exposureSamples;
	bool anaglyph;
	Vector eyesOffset;
};

#endif // CAMERA_H
