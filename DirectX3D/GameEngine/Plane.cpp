#include "StdAfx.h"
#include "Plane.h"

CPlane::CPlane(void)
{
	a = 0; b = 0; c = 0; d = 0;
}

CPlane::CPlane(float A, float B, float C, float D)
{
	a = A; b = B; c = C; d = D;
}


CPlane::CPlane(CVector3 &n, float D)
{
	a = n.x; b = n.y; c = n.z; d = D;
}


void CPlane::CreatePlaneFromTri(CVector3 &v1, CVector3 &v2, CVector3 &v3)
{
	// Get triangle normal.
	CVector3 normal, e1, e2;

	e1 = v3 - v1;
	e2 = v2 - v1;

	e1.Normal();
	e2.Normal();

	normal.CrossProduct(e1, e2);
	normal.Normal();

	// Save normal and calculate d.
	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = - (a * v1.x + b * v1.y + c * v1.z);
}

CPlane::~CPlane(void)
{
}

bool CPlane::Intersect(CPlane &p2, CPlane &p3, CVector3 *intersectPoint)
{
	CVector3 temp;
	CVector3 normal(a, b, c);
	CVector3 p2Normal(p2.a, p2.b, p2.c);
	CVector3 p3Normal(p3.a, p3.b, p3.c);

	// Get cross product of p2 and p3.
	temp.CrossProduct(p2Normal, p3Normal);

	// Get the dot prod of this plane with cross of p2 and p3.
	float dp = normal.DotProduct3(temp);
	if(dp == 0.0f) return false;

	// Else we must find intersect point.
	if(intersectPoint)
	{
		*intersectPoint = temp * d;

		temp.CrossProduct(p3Normal, normal);
		*intersectPoint = *intersectPoint + (temp * p2.d);

		temp.CrossProduct(normal, p2Normal);
		*intersectPoint = *intersectPoint + (temp * p3.d);

		*intersectPoint = *intersectPoint / -dp;
	}

	return true;
}


bool CPlane::Intersect(CPlane &pl, CVector3 *intersectPoint)
{
	CVector3 cross;
	CVector3 normal(a, b, c);
	CVector3 plNormal(pl.a, pl.b, pl.c);
	float length = 0;

	cross.CrossProduct(normal, plNormal);
	length = cross.DotProduct3(cross);

	if(length < 1e-08f) return false;

	if(intersectPoint)
	{
		float l0 = normal.DotProduct3(normal);
		float l1 = normal.DotProduct3(plNormal);
		float l2 = plNormal.DotProduct3(plNormal);
		float det = l0 * l2 - l1 * l1;
		float invDet = 0;

		if(fabs(det) < 1e-08f) return false;

		invDet = 1 / det;
		float d0 = (l2 * d - l1 * pl.d) * invDet;
		float d1 = (l0 * pl.d - l1 * d) * invDet;

		(*intersectPoint) = normal * d0 + plNormal * d1;
	}

	return true;
}


bool CPlane::Intersect(CPlane &pl, CRay *intersect)
{
	CVector3 cross;
	CVector3 normal(a, b, c);
	CVector3 plNormal(pl.a, pl.b, pl.c);
	float length = 0;

	cross.CrossProduct(normal, plNormal);
	length = cross.DotProduct3(cross);

	if(length < 1e-08f) return false;

	if(intersect)
	{
		float l0 = normal.DotProduct3(normal);
		float l1 = normal.DotProduct3(plNormal);
		float l2 = plNormal.DotProduct3(plNormal);
		float det = l0 * l2 - l1 * l1;
		float invDet = 0;

		if(fabs(det) < 1e-08f) return false;

		invDet = 1 / det;
		float d0 = (l2 * d - l1 * pl.d) * invDet;
		float d1 = (l0 * pl.d - l1 * d) * invDet;

		(*intersect).m_origin = normal * d0 + plNormal * d1;
		(*intersect).m_direction = cross;
	}

	return true;
}
