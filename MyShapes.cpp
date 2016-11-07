#include "MyShapes.h"


Plane::Plane(Vec normal, float d, Color color)
{
	this->n = normal;
	this->d = d;
	this->c = color;
}

void Plane::test(Ray & ray, HitData & hit)
{

}

Vec Plane::normal(Vec & point)
{
	return Vec();
}
