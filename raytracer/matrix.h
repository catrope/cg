//
//  Framework for a raytracer
//  File: matrix.h
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

#ifndef MATRIX_H
#define MATRIX_H

#include "triple.h"

class Matrix {
public:
	Vector r1, r2, r3;
	
	Matrix(Vector r1, Vector r2, Vector r3) : r1(r1), r2(r2), r3(r3) {}
	Matrix() : r1(Vector(1, 0, 0)), r2(Vector(0, 1, 0)), r3(Vector(0, 0, 1)) { }
	
	Matrix transposed() const;
	Vector operator*(const Vector &v);
	Matrix operator*(const Matrix &m);
	
	/**
	 * Compute a rotation matrix
	 * @param v Axis to rotate around
	 * @param angle Angle to rotate around (in radians)
	 * @return Rotation matrix
	 */
	static const Matrix rotation(const Vector &v, double angle);
	
	/**
	 * Same as rotation(), but with the angle in degrees
	 */
	inline static const Matrix rotationDeg(const Vector &v, double angle) { return Matrix::rotation(v, angle*M_PI/180); }
	
	/**
	 * Compute a rotation matrix that will rotate v onto the x axis
	 * @param v Vector to rotate onto the x axis
	 * @param inverse If not NULL, the inverse rotation matrix will be written here
	 * @return Rotation matrix
	 */
	static const Matrix rotationOntoXAxis(const Vector &v, Matrix *inverse);
};

#endif /* end of include guard: MATRIX_H */
