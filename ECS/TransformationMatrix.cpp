#include "stdafx.h"
#include "TransformationMatrix.h"




TransformationMatrix::TransformationMatrix() : a(1.0f), b(0.f), c(0.0f), d(0.0f), e(1.0f), f(0.0f)
{

};

TransformationMatrix::TransformationMatrix(const Vector2& Vector, const float& Angle) : c(Vector.x) , f(Vector.y) 
{
	e = a =  cos(Angle);
	d =  sin(Angle);
	b = -d;
};


void TransformationMatrix::Set(const Vector2& Vector, const float& Angle) 
{
	e = a =  cos(Angle);
	d =  sin(Angle);
	b = -d;
	c = Vector.x;
	f = Vector.y;
};


/*TransformationMatrix& TransformationMatrix::operator() (const float &angle, const Vector2 &T) {
	e = a =  cos(angle);
	d =  sin(angle);
	b = -d;
	c = T.x;
	f = T.y;
	return *this;
};*/