#include "Shapes.h"

Color Shape::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	Color finalColor;

	float ambLight[3] = { 0.19,0.19,0.19 };		// Saved values from the slide. 
	float diffLight[3] = { 1.0,1.0,1.0 };		//So we don't have to divide with 255 later.

	float red;
	float green;
	float blue;

	Vec lightVector = light - (r.o + r.d*h.t);	//Vector from the hitpoint to the light point.
	lightVector.Normalize();					//The vector needs to be normalized. 

	float LN = lightVector.Dot(h.lastNormal);

	if (LN < 0)
	{
		LN = 0;
	}

			// Here we use the difflight[] and amblight[] not to divide with 255
			//This is the formula from the slide. We need the Mamb but in the same form as the Mdiff
	red   = diffLight[0] * h.color.r * LN + ambLight[0] * h.color.r;
	green = diffLight[1] * h.color.g * LN + ambLight[1] * h.color.g;
	blue  = diffLight[2] * h.color.b * LN + ambLight[2] * h.color.b;


	//We have to make sure that rgb isn't higher than 255, a color can't contain more.
	if (red > 255)
	{
		red = 255;
	}
	if (green > 255)
	{
		green = 255;
	}
	if (blue > 255)
	{
		blue = 255;
	}

	finalColor.r = red;
	finalColor.g = green;
	finalColor.b = blue;

	return finalColor;

}
