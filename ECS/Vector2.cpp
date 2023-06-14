#include "stdafx.h"
#include "Vector2.h"


float  sqrt2(const float x)
{
  const float xhalf = 0.5f*x;
 
  union // get bits for floating value
  {
    float x;
    int i;
  } u;
  u.x = x;
  u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
  return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy 
};


Vector2::Vector2() : x(0.0f), y(0.0f)
{

};


Vector2::Vector2(const float &xx, const float &yy ) : x(xx), y(yy)
{

};


Vector2::Vector2( const Vector2& V) : x(V.x), y(V.y)
{

};

Vector2::~Vector2()
{


};


const Vector2&	Vector2::normalize(void)
{
	auto ln = length(); 
	ln = ln > 0.0f ? 1.0f/ln : 0.0f;
	return (*this)*=ln;
};


const Vector2	Vector2::unit() const 
{
	auto ln = length(); 
	ln = ln > 0.0f ? 1.0f/ln : 0.0f;
	return (*this)*ln;
};
