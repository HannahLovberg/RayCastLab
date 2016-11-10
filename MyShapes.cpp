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
	if (n.Dot(ray.d) != 0)		//make sure the plane isn't parallel with the ray (epsilon could be used?)
	{
		float k = -n.Dot(n*d);
		float t = (-n.Dot(ray.o) - k) / (n.Dot(ray.d));		//t = distance from ray's origin
															//We find the hit point
		if (t < hit.t || hit.t < 0)	//Make sure the ray hit the hitpoint and not anything else before it
		{
			hit.t = t;
			hit.color = c;
			hit.lastNormal = n;
			hit.lastShape = this;		//Set the shape
		}
	}
}

Vec Plane::normal(Vec & point)
{
	return this->n;		//We already have a normalized vector
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
		float t2 = -i - sqrt(b);	//Here we calculate t1 and t2
									
		if (t1 > t2)				//We want the smallest first. The one closest to the origin of
		{							//the ray and the one further away second.
			std::swap(t1, t2);
		}
		
		if (t1 > 0)							//We check if t1 is in front of the camera.
		{
			if (t1 < hit.t || hit.t < 0)	//If the the ray hits the shape and not anything else before it
			{
				hit.t = t1;
				hit.color = c;
				hit.lastNormal = normal(ray.o + ray.d * t1); //We need the normal between the center and the hitpoint
				hit.lastShape = this;
			}
			
		}

		else if (t2 > 0)	//If t1 isn't before the camera we check if t2 are. If not we don't have to draw the sphere.
		{					
			if (t2 < hit.t || hit.t < 0) 
			{
				hit.t = t2;
				hit.color = c;
				hit.lastNormal = normal(ray.o + ray.d * t2);
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
	this->nor.y = (A.z*B.x) - (A.x*B.z);		//Crossproduct to get the normal
	this->nor.z = (A.x*B.y) - (A.y*B.x);

	this->nor.Normalize();
}

float Triangle::det(Vec c1, Vec c2, Vec c3)		//The help-function
{
	float determinant = 0;


	///Creates a matrix with the vector parameters
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

	Vec TUV = { det(s, e1, e2), det(ray.d*-1, s, e2), det(ray.d*-1, e1, s)};	//get the three unknowns t,u,v
	TUV = TUV*(1 / det(ray.d*-1, e1, e2));

	//T is the distance
	
	float t = TUV.x;
	float u = TUV.y;		//Where u and v are base vectors in an alternative plane and are used to
	float v = TUV.z;		//express all the points in that plane

	if (t > 0)
	{
		if (u >= 0 && v >= 0 && (u + v) <= 1)	//Here we make sure that the point we found is within the triangle
		{										//If it isn't there's no hit
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

///////////////////////////////////OBB/////////////////////////////


OBB::OBB(Vec center, Vec u, Vec v, Vec w, float halfU, float halfV, float halfW, Color color)
{
	this->center = center;
	this->u = u;
	this->v = v;
	this->w = w;

	this->halfU = halfU;
	this->halfV = halfV;
	this->halfW = halfW;

	this->Pu = center + u*halfU;		//Centerpoints for each plane
	this->Puo = center - u*halfU;		// Pu and Puo is opposite of each other and so on...

	this->Pv = center + v*halfV;
	this->Pvo = center - v*halfV;

	this->Pw = center + w*halfW;
	this->Pwo = center - w*halfW;

	this->c = color;
}

OBB::OBB(Vec center, float halfU, float halfV, float halfW, Color color)
{
	this->center = center;

	this->u = { 1,0,0 };		//We don't get these in the parameter
	this->v = { 0,1,0 };		//Otherwise everything 's the same
	this->w = { 0,0,1 };

	this->halfU = halfU;
	this->halfV = halfV;
	this->halfW = halfW;

	this->Pu = center + u*halfU;
	this->Puo = center - u*halfU;

	this->Pv = center + v*halfV;
	this->Pvo = center - v*halfV;

	this->Pw = center + w*halfW;
	this->Pwo = center - w*halfW;

	this->c = color;
}

void OBB::test(Ray& ray, HitData& hit)
{
	float tMin = -INFINITY;		
	float tMax = INFINITY;

	float t1, t2;				//Our hitpoints

	float intersect;		

	Vec norm[3] = { u,v,w };
	float half[3] = { halfU, halfV, halfW };	
	float e;			
	float f;

	float epsilon = 0.0001;		//that funny e thing in the book that's not an e

	Vec p = center - ray.o;			//the vector between the centerpoint and the start of the ray

	for (int i = 0; i < 3; i++)
	{
		e = norm[i].Dot(p);			//plane between planes
		f = norm[i].Dot(ray.d);		//The cosinus angle between the normal and ray.d

		if (abs(f) > epsilon)	//makes sure the normal isn't parallel with the ray
		{
			t1 = (e + half[i]) / f;			//where the ray goes through plane1
			t2 = (e - half[i]) / f;			//where the ray goes through plane2
		
			if (t1 > t2) std::swap(t1, t2);		//Make sure the smallest value is first
			if (t1 > tMin) tMin = t1;			
			if (t2 < tMax) tMax = t2;

			if (tMin > tMax) return;
			if (tMax < 0) return;

		}

		else if (-e - half[i] > 0 || -e + half[i] < 0) return;	
		// If the ray is parallel with the slabs it checks if the ray is within another slab. 
		//For an example if the ray hits the box straight on, being parallel with two of the slabs
		//then this line checks if the ray hits the thrid slab, if not that means there's no hit

	}
	if (tMin > 0)	//Check if tMin is before the camera and makes it the intersect
	{
		intersect = tMin;		
	}
	else			
	{
		intersect = tMax;
	}

	if (intersect < hit.t || hit.t < 0) //If the the ray hits the shape and not anything else before it
	{
		hit.t = intersect;
		hit.color = c;
		hit.lastShape = this;
		hit.lastNormal = normal(ray.o + ray.d*intersect);	
		
	}
	
}

Vec OBB::normal(Vec& point)
{
	Vec n = { 0,0,0 };

	float epsilon = 0.0001;

	Vec norm[6] = { u, u*-1, v, v*-1, w, w*-1 };
	Vec mP[6] = { Pu, Puo, Pv, Pvo, Pw, Pwo };

	for (int i = 0; i < 6; i++)
	{
		if (abs(norm[i].Dot(point - mP[i])) < epsilon)		//The center of each plane dot the vector 
		{											//between the hitPoint and the center of the plane
			n = norm[i];							//If this is 0 then it's a hit
		}
		
	}
	return n;		//If not just return {0,0,0}
}