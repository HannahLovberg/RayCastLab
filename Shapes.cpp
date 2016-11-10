#include "Shapes.h"

Color Shape::shade(Vec& light, const Vec& cam, Ray& r, HitData& h)
{
	Color finalColor;

	float ambLight[3] = { 0.19,0.19,0.19 };		// Saved values from the slide. Think 19% of amb. light
	float diffLight[3] = { 1.0,1.0,1.0 };		//So we don't have to divide with 255 later.
												//1.0 is the same as 255  (100%)
	float red;
	float green;
	float blue;

	Vec lightVector = light - (r.o + r.d*h.t);	//Vector from the hitpoint to the light point.
	lightVector.Normalize();					//The vector needs to be normalized. 

	float LN = lightVector.Dot(h.lastNormal);

	if (LN < 0)		//We make sure the light can hit the point. If it's under 0 then the 
	{				//angle is in a point where the point cannot be hit. The same goes for negative 
		LN = 0;		//values. That's why we set the LN to 0. We don't want negative colors.
	}

			// Here we use the difflight[] and amblight[] not to divide with 255
			//This is the formula from the slide. We need the Mamb but in the same form as the Mdiff
	red   = diffLight[0] * h.color.r * LN + ambLight[0] * h.color.r;
	green = diffLight[1] * h.color.g * LN + ambLight[1] * h.color.g;
	blue  = diffLight[2] * h.color.b * LN + ambLight[2] * h.color.b;
			//Think of the values as %.
			//We get 100% of the lightray's light and 19% of the amb light.
			//So...
			//100%raylight*materialColor*angle between lightVector and normal + 19%amblight*materialColor


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
