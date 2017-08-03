#pragma once


#include<math.h>
#include"Vector.h"
#include"MathDefines.h"

class CQuaternion
{
public:
	CQuaternion(void);
	~CQuaternion(void);

	CQuaternion(float xAxis, float yAxis,
		float zAxis, float wAxis);

	void operator=(const CQuaternion &q);
	CQuaternion operator*(const CQuaternion &q);
	CQuaternion operator+(const CQuaternion &q);

	void CreateQuatFromAxis(CVector3 &a, float radians);

	float Length();
	void Normal();

	CQuaternion Conjugate();
	CQuaternion CrossProduct(const CQuaternion &q);

	void Rotatef(float amount, float xAxis,
		float yAxis, float zAxis);
	void RotationRadiansf(double X, double Y, double Z);

	void CreateMatrix(float *pMatrix);

	void Slerp(const CQuaternion &q1, const CQuaternion &q2, float t);

	float w, x, y, z;

};
