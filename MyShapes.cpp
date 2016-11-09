#include "MyShapes.h"

//////////////////////// Plane //////////////////////////////////

Plane::Plane(Vec normal, float d, Color color)
{
	this->n = normal;
	this->d = d;
	this->c = color;
}

void Plane::test(Ray & ray, HitData & hit)
{
	if (n.Dot(ray.d) != 0)
	{
		float k = -n.Dot(n*d);
		float t = (-n.Dot(ray.o) - k) / (n.Dot(ray.d));		//Distance from ray's origin

		if (t < hit.t || hit.t < 0)
		{
			hit.t = t;
			hit.color = c;
			hit.lastNormal = n;
			hit.lastShape = this;
		}
	}
}

Vec Plane::normal(Vec & point)
{
	return this->n;
}


///////////////////////// Sphere //////////////////////////////////

Sphere::Sphere(Vec center, float radius, Color color)
{
	this->center = center;
	this->radius = radius;
	this->radius2 = radius*radius;
	this->c = color;
}

void Sphere::test(Ray& ray, HitData& hit)
{
	float i = ray.d.Dot(ray.o - center);
	float k = (ray.o - center).Dot(ray.o - center) - radius2;
	float b = i*i - k;


	if (b >= 0)
	{	
		
		float t1 = -i + sqrt(b);
		float t2 = -i - sqrt(b);

		if (t1 > t2)
		{
			std::swap(t1, t2);
		}
		
		if (t1 > 0)		//Om den är framför kameran
		{
			if (t1 < hit.t || hit.t < 0)
			{
				hit.t = t1;
				hit.color = c;
				hit.lastNormal = normal(ray.o);
				hit.lastShape = this;
			}
			
		}

		else if (t2 > 0) 
		{
			if (t2 < hit.t || hit.t < 0) 
			{
				hit.t = t2;
				hit.color = c;
				hit.lastNormal = normal(ray.o);
				hit.lastShape = this;
			}
		}
			
		
		
	}
}

Vec Sphere::normal(Vec &point)
{
	Vec n = point - center;
	n.Normalize();
	return n;
}

///////////////////// Triangle ////////////////////////////

Triangle::Triangle(Vec p1, Vec p2, Vec p3, Color color)
{
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	this->c = color;

	Vec A = p2 - p1;
	Vec B = p3 - p1;

	this->nor.x = (A.y*B.z) - (A.z*B.y);
	this->nor.y = (A.z*B.x) - (A.x*B.z);
	this->nor.z = (A.x*B.y) - (A.y*B.x);
}

float Triangle::det(Vec c1, Vec c2, Vec c3)
{
	float determinant = 0;


	///Creates a matrix with the parameters
	int matrix[3][3];

	matrix[0][0] = c1.x;	matrix[0][1] = c2.x;	matrix[0][2] = c3.x;
	matrix[1][0] = c1.y;	matrix[1][1] = c2.y;	matrix[1][2] = c3.y;
	matrix[2][0] = c1.z;	matrix[2][1] = c2.z;	matrix[2][2] = c3.z;


	//calculates the determinant
	determinant =(	matrix[0][0] * matrix[1][1] * matrix[2][2] +
					matrix[1][0] * matrix[2][1] * matrix[0][2] +
					matrix[0][1] * matrix[1][2] * matrix[2][0]) -
				 (	matrix[0][2] * matrix[1][1] * matrix[2][0] +
					matrix[0][1] * matrix[1][0] * matrix[2][2] +
					matrix[1][2] * matrix[2][1] * matrix[0][0]);

	return determinant;

}

void Triangle::test(Ray& ray, HitData& hit)
{
	Vec e1 = p2 - p1;
	Vec e2 = p3 - p1;
	Vec s = ray.o - p1;

	Vec TUV = { det(s, e1, e2), det(ray.d*-1, s, e2), det(ray.d*-1, e1, s)};
	TUV = TUV*(1 / det(ray.d*-1, e1, e2));

	//T is the distance
	
	float t = TUV.x;
	float u = TUV.y;		//simplify
	float v = TUV.z;

	if (t > 0)
	{
		if (u >= 0 && v >= 0 && (u + v) <= 1)		//if the point is within the triangle this is true
		{
			if (t < hit.t || hit.t < 0)
			{
				hit.t = t;
				hit.color = c;
				hit.lastNormal = this->nor;
				hit.lastShape = this;
			}	
		}
	}
}

Vec Triangle::normal(Vec &point)
{
	return nor;
}


OBB::OBB(Vec center, Vec u, Vec v, Vec w, float halfU, float halfV, float halfW, Color color)
{
	this->center = center;
	this->u = u;
	this->v = v;

	//Fortsätt med pu pv osv... 
	
	this->halfU = halfU;
	this->halfV = halfV;
	this->halfW = halfW;

	this->c = color;
}

OBB::OBB(Vec center, float halfu, float halfv, float halfw, Color color)
{
	this->center = center;
	this->halfU = halfu;
	this->halfV = halfv;
	this->halfW = halfw;

	this->c = color;
}