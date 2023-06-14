#pragma once
#include <cmath>
#include <assert.h>

static const float pi  = 3.1415f;
static const float tau = 2*pi;

#define SQRT_MAGIC_F 0x5f3759df 
float  sqrt2(const float x);

class Vector2
{
public:
	float							x,y;

									Vector2();
									Vector2(const float &xx, const float &yy );
									Vector2( const Vector2& V);
									~Vector2();

	inline float					length(void) const;
	const Vector2&					normalize(void);
	const Vector2					unit() const;
	
	friend inline const Vector2		operator*(const float &scalar , const Vector2 &vec);
	inline bool						operator!=(const Vector2 &vec);
	inline bool						operator==(const Vector2 &vec);
	inline Vector2&					operator=(const Vector2 &vec);
	inline Vector2&					operator/=(const float &scalar);
	inline Vector2&					operator*=(const float &scalar); 
	inline Vector2&					operator+=(const Vector2 &vec); 
	inline Vector2&					operator-=(const Vector2 &vec); 
	inline const Vector2			operator-() const;
	inline const Vector2			operator-(const Vector2 &vec) const;
	inline const Vector2			operator+(const Vector2 &vec) const;
	inline const Vector2			operator/(const float &scalar) const;
	inline const Vector2			operator*(const float &scalar) const;
	inline const float				operator*(const Vector2 &vec) const;
}; 


float Vector2::length(void) const	
{ 
	return sqrt2(x*x + y*y); 
};


bool Vector2::operator!=(const Vector2 &vec) 
{
	if( (x != vec.x) && (y != vec.y) )
	{
		return true;
	}
	else		
	{
		return false; 
	}
};


bool Vector2::operator==(const Vector2 &vec) 
{
	if( (x == vec.x) && (y == vec.y) ) 
	{
		return true;
	}
	else		
	{
		return false; 
	}
};


Vector2&  Vector2::operator=(const Vector2 &vec) 
{
	if(this == &vec) return *this;
	x = vec.x;
	y = vec.y;
	return *this; 
};


Vector2&  Vector2::operator/=(const float &scalar)
{							
	assert(scalar!=0.0f);//if( scalar == 0 ) {};
	x/=scalar;
	y/=scalar;
	return *this; 
};


Vector2&  Vector2::operator*=(const float &scalar) 
{				
	x*=scalar;
	y*=scalar;
	return *this; 
};


Vector2&  Vector2::operator+=(const Vector2 &vec) 
{							
	x+=vec.x;
	y+=vec.y;
	return *this;
};


Vector2&  Vector2::operator-=(const Vector2 &vec) 
{
	x-=vec.x;
	y-=vec.y;
	return *this;
};
	

const  Vector2 Vector2::operator-() const
{									
	return Vector2(-x , -y);
};


const Vector2 Vector2::operator-(const Vector2 &vec) const
{
	return Vector2(x - vec.x , y - vec.y);
};


const Vector2 Vector2::operator+(const Vector2 &vec) const 
{
	return Vector2(x + vec.x , y + vec.y);
};
	

const Vector2 Vector2::operator/(const float &scalar) const
{				
	assert(scalar!=0.0f);//if( scalar == 0 ) {};
	return Vector2(x / scalar , y / scalar); 
};


const Vector2 Vector2::operator*(const float &scalar) const
{
	return Vector2(x * scalar , y * scalar);
};


const float Vector2::operator*(const Vector2 &vec) const
{
	return ( (x * vec.x) + (y * vec.y) );
};


inline const Vector2 operator*(const float &scalar ,  const Vector2 &vec)
{
		return vec*scalar;
};


inline const float VectorProduct(const Vector2 &vec1, const Vector2 &vec2) 
{ 
	return (vec1.x * vec2.y - vec1.y*vec2.x); 
};	


//void AngleNormalize(float &value);

const Vector2 ZeroVector(0.0f,0.0f);

