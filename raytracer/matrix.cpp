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

const Matrix Matrix::rotationOntoXAxis(const Vector &v, Matrix *inverse)
{
	Vector xAxis(1, 0, 0), yAxis(0, 1, 0);
	Vector u = v.normalized();
	
	// Project u onto the yz plane
	Vector uProj(0, u.y, u.z);
	uProj.normalize();
	// Get the sin and cos of the angle between uProj and the y axis (A)
	double cosA = uProj.dot(yAxis);
	double sinA = uProj.cross(yAxis).length();
	// Build a matrix that rotates around the x axis with angle A, thereby
	// rotating u into the xy plane. We need clockwise rotation if u.z is
	// positive, counterclockwise rotation otherwise.
	if (u.z > 0) {
		// Clockwise rotation: counterclockwise rotation with angle -A
		// cos(-A) = cos(A) but sin(-A) = -sin(A)
		sinA = -sinA;
	}
	Matrix rotA(Vector(1, 0, 0), Vector(0, cosA, -sinA), Vector(0, sinA, cosA));
	// Rotate u
	u = (rotA*u).normalized();
	
	// Get the sin and cos of the angle between u and the x axis (B)
	double cosB = u.dot(xAxis);
	double sinB = u.cross(xAxis).length();
	// Build a matrix that rotates around the z axis with angle B, thereby
	// rotating u onto the x axis. We need clockwise rotation if u.y is
	// positive, counterclockwise rotation otherwise.
	if (u.y > 0) {
		// Angle negation trick, see above
		sinB = -sinB;
	}
	Matrix rotB(Vector(cosB, -sinB, 0), Vector(sinB, cosB, 0), Vector(0, 0, 1));
	
	// Compute inverse if requested
	if (inverse) {
		Matrix invRotA(Vector(1, 0, 0), Vector(0, cosA, sinA), Vector(0, -sinA, cosA));
		Matrix invRotB(Vector(cosB, sinB, 0), Vector(-sinB, cosB, 0), Vector(0, 0, 1));
		*inverse = invRotA*invRotB;
	}
	
	return rotB*rotA;
}
