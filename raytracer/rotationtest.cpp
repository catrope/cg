#include <stdio.h>
#include "matrix.h"
using namespace std;


void test(Vector v);

int main()
{
	/*Vector v(0, -3, 4);
	Matrix inv;
	Matrix m = Matrix::rotationOntoXAxis(v, &inv);
	
	printf("v: (%f, %f, %f)\n", v.x, v.y, v.z);
	printf("m: ((%f, %f, %f), (%f, %f, %f), (%f, %f, %f))\n",
		m.r1.x, m.r1.y, m.r1.z, m.r2.x, m.r2.y, m.r2.z, m.r3.x, m.r3.y, m.r3.z);
	Vector mv = m*v;
	printf("m*v: (%f, %f, %f)\n", mv.x, mv.y, mv.z);
	Vector invv = inv*v;
	printf("inv*v: (%f, %f, %f)\n", invv.x, invv.y, invv.z);
	Vector invmv = inv*m*v;
	printf("inv*m*v: (%f, %f, %f)\n", invmv.x, invmv.y, invmv.z);
	
	Matrix i1 = inv*m;
	Matrix i2 = m*inv;
	printf("inv*m: ((%f, %f, %f), (%f, %f, %f), (%f, %f, %f))\n",
		i1.r1.x, i1.r1.y, i1.r1.z, i1.r2.x, i1.r2.y, i1.r2.z, i1.r3.x, i1.r3.y, i1.r3.z);
	printf("m*inv: ((%f, %f, %f), (%f, %f, %f), (%f, %f, %f))\n",
		i2.r1.x, i2.r1.y, i2.r1.z, i2.r2.x, i2.r2.y, i2.r2.z, i2.r3.x, i2.r3.y, i2.r3.z);
	
	//cout << "v: (" << v.x << ", " << v.y << ", " << v.z << ")" << endl;
	
	*/
	
	test(Vector(3, 4, 0));
	test(Vector(-3, 4, 0));
	test(Vector(3, -4, 0));
	test(Vector(-3, -4, 0));
	test(Vector(3, 0, 4));
	test(Vector(-3, 0, 4));
	test(Vector(3, 0, -4));
	test(Vector(-3, 0, -4));
	test(Vector(0, 3, 4));
	test(Vector(0, -3, 4));
	test(Vector(0, 3, -4));
	test(Vector(0, -3, -4));
	return 0;
}

void test(Vector v)
{
	Matrix inv;
	Matrix m = Matrix::rotationOntoXAxis(v, &inv);
	Vector mv = m*v;
	printf("(%f, %f, %f) --> (%f, %f, %f)\n", v.x, v.y, v.z, mv.x, mv.y, mv.z);
}