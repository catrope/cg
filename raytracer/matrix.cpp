//
//  Framework for a raytracer
//  File: matrix.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Roan Kattouw
//    Jan Paul Posma
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "matrix.h"

Matrix Matrix::transposed() const {
	return Matrix(Vector(r1.x, r2.x, r3.x), Vector(r1.y, r2.y, r3.y), Vector(r1.z, r2.z, r3.z));
}

Vector Matrix::operator*(const Vector &v)
{
	return Vector(r1.dot(v), r2.dot(v), r3.dot(v));
}

Matrix Matrix::operator*(const Matrix &m)
{
	Matrix t = m.transposed();
	return Matrix(t*r1, t*r2, t*r3);
}

const Matrix Matrix::rotation(const Vector &v, double angle)
{
	Vector u = v.normalized();
	double cosA = cos(angle);
	double sinA = sin(angle);
	
	/*
	 * [ cos a + ux^2(1-cos a)       ux uy(1-cos a)-uz sin a     ux uz(1-cos a)+uy sin a  ]
	 * [ uy ux(1-cos a)+uz sin a     cos a + uy^2(1-cos a)       uy uz(1-cos a)-ux sin a  ]
	 * [ uz ux(1-cos a)-uy sin a     uz uy(1-cos a)+ux sin a     cos a + uz^2(1-cos a)    ]
	 * From https://secure.wikimedia.org/wikipedia/en/wiki/Rotation_matrix#Rotation_matrix_from_axis_and_angle
	 */
	return Matrix(
		Vector(cosA + u.x*u.x*(1-cosA), u.x*u.y*(1-cosA) - u.z*sinA, u.x*u.z*(1-cosA) + u.y*sinA),
		Vector(u.y*u.x*(1-cosA) + u.z*sinA, cosA + u.y*u.y*(1-cosA), u.y*u.z*(1-cosA) - u.x*sinA),
		Vector(u.z*u.x*(1-cosA) - u.y*sinA, u.z*u.y*(1-cosA) + u.x*sinA, cosA + u.z*u.z*(1-cosA))
	);
}
