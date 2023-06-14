#pragma once
#include "stdafx.h"
#include "Vector2.h"


class Rectangle2
{
public:
	
	Vector2					min,max;

							Rectangle2();
							Rectangle2(const float&, const float&, const float&, const float&);
							Rectangle2(const Vector2&, const Vector2&);

	inline void				SetSize(const Vector2&);
	inline void				SetPosition(const Vector2&);

	inline const Vector2	GetSize() const;
	inline const Vector2	GetPosition() const;

	inline const float		GetWidth() const;
	inline const float		GetHeight() const;

};


inline void Rectangle2::SetSize(const Vector2& Size)
{
	max.x = min.x + Size.x;
	max.y = min.y + Size.y;
};


inline void Rectangle2::SetPosition(const Vector2& Position)
{
	max.x = Position.x +  (max.x - min.x);
	max.y = Position.y +  (max.y - min.y);	
	min = Position;
};


inline const Vector2 Rectangle2::GetSize() const
{
	return Vector2(max.x-min.x, max.y-min.y);
};


inline const Vector2 Rectangle2::GetPosition() const
{
	return Vector2(min.x, min.y);
};


inline const float Rectangle2::GetWidth() const
{
	return (max.x-min.x);
};


inline const float Rectangle2::GetHeight() const
{
	return (max.y-min.y);
};