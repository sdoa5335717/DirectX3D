#ifndef _UGP_BOUNDINGGEOMETRY_H_
#define _UGP_BOUNDINGGEOMETRY_H_


#include"MathLibrary.h"

// �߽缸��ͼ�λ���
class CBoundingBase
{
public:
	CBoundingBase(){}
	virtual ~CBoundingBase() {}

	virtual void CreateFromPoints(CVector3 *pointList, int numPoints) = 0;

	// ����һ�����Ƿ��һ���߽缸��ͼ�η�����ײ
	virtual bool isPointInside(CVector3 &v) = 0;

	virtual bool Intersect(CRay ray, float *dist) = 0;
	virtual bool Intersect(CRay ray, float length, float *dist) = 0;

	// ����ÿ��������ƽ��(6��ƽ��)
	virtual void GetPlanes(CPlane *planes) = 0;

	// �鿴�����Ƿ�����ڱ߽缸��ͼ����
	virtual bool isRayInside(CRay &ray, float length) = 0;
};



// �߽����
class CBoundingBox : public CBoundingBase
{
public:
	CBoundingBox() {}
	~CBoundingBox() {}

	void CreateFromPoints(CVector3 *pointList, int numPoints);
	bool isPointInside(CVector3 &v);

	bool Intersect(CRay ray, float *dist);
	bool Intersect(CRay ray, float length, float *dist);

	void GetPlanes(CPlane *planes);
	bool isRayInside(CRay &ray, float length);

	CVector3 m_min, m_max;
};



// �߽���
class CBoundingSphere : public CBoundingBase
{
public:
	CBoundingSphere() : m_radius(0) {}
	~CBoundingSphere() {}

	void CreateFromPoints(CVector3 *pointList, int numPoints);
	bool isPointInside(CVector3 &v);

	bool Intersect(CRay ray, float *dist);
	bool Intersect(CRay ray, float length, float *dist);

	void GetPlanes(CPlane *planes) {}
	bool isRayInside(CRay &ray, float length);

	CVector3 m_center;
	float m_radius;
};


// �����������֮�����ײ
bool BoxToBoxIntersect(CBoundingBox &bb1, CBoundingBox &bb2);

// �����������֮�����ײ
bool SphereToSphereIntersect(CBoundingSphere &bs1, CBoundingSphere &bs2);

#endif
