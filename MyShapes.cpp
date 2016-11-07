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
		
		if (t1 > 0)		//Om den �r framf�r kameran
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
}

void Triangle::test(Ray& ra, HitData& hit)
{
	
}

Vec Triangle::normal(Vec &point)
{
	Vec A = p2 - p1;
	Vec B = p3 - p1;

	Vec n;
	n.x = (A.y*B.z) - (A.z*B.y);
	n.y = (A.z*B.x) - (A.x*B.z);
	n.z = (A.x*B.y) - (A.y*B.x);

	return n;
}