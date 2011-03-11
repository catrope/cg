#ifndef CAMERA_H
#define CAMERA_H

#include "triple.h"

class Camera
{
public:
	Vector eye;
	Vector center;
	Vector up;
	int viewWidth;
	int viewHeight;
	double apertureRadius;
	unsigned int apertureSamples;
};

#endif // CAMERA_H
