#pragma once
#include "stdafx.h"
#include "Vector2.h"

class TransformationMatrix 
{
	float					a,b,c,d,e,f,x,y;

public:
							TransformationMatrix();
							TransformationMatrix(const Vector2&, const float &);
		
	//inline const Vector2	operator*(const Vector2 &) const;
	inline void				Transform(const Vector2&, Vector2&);
	void					Set(const Vector2&, const float&);
};


void TransformationMatrix::Transform(const Vector2& In, Vector2& Out)
{	
	Out.x = a * In.x + b * In.y + c;
	Out.y = d * In.x + e * In.y + f;
};


static TransformationMatrix CCW(ZeroVector, pi/2.0f); 
/*
const Vector2 TransformationMatrix::operator*(const Vector2& Vector) const
{	
	return Vector2( a * Vector.x + b * Vector.y + c  , d * Vector.x + e * Vector.y + f ); 
};*/
