#pragma once
#include "Shapes.h"



class Plane : public Shape
{
	Vec n;		//As in Normal
	float d;	//As in the amount of times to multiply with n


public:
	Plane(Vec normal, float _d, Color color);
	void test(Ray & ray, HitData & hit);
	Vec normal(Vec & point);

};

class Sphere : public Shape
{
	Vec center;			//Center point
	float radius;		//radious width
	float radius2;		//radius^2

public:
	Sphere(Vec center, float radius, Color color);
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);

};

class Triangle : public Shape
{
	Vec p1, p2, p3, nor;	//as in three point and the normal
	Vec edge1, edge2;

	///Help functions
	float det(Vec c1, Vec c2, Vec c3);

public:
	Triangle(Vec p1, Vec p2, Vec p3, Color color);
	void test(Ray& ray, HitData& hit);
	Vec normal(Vec &point);
	
};

class OBB : public Shape
{
	Vec center;		//The center of the cube.

	Vec u;			//vectors uvw (xyz axis)
	Vec v;
	Vec w;

	Vec Pu;
	Vec Puo;
	Vec Pv;
	Vec Pvo;
	Vec Pw;
	Vec Pwo;

	float halfU;
	float halfV;	//half-distance
	float halfW;

public:
	
	OBB(Vec center, Vec u, Vec v, Vec w, float halfU, float halfV, float halfW, Color color);
	
	OBB(Vec center, float halfU, float halfV, float halfW, Color color);

	void test(Ray& ray, HitData& hit);
	Vec normal(Vec& point);
};
