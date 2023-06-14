#pragma once
#include "stdafx.h"
#include "Rectangle2.h"


Rectangle2::Rectangle2()
{ 
	min.x = 0.0f; 
	min.y = 0.0f; 
	max.x = 0.0f; 
	max.y = 0.0f; 
};


Rectangle2::Rectangle2(const float &xmin, const float &ymax, const float &xmax, const float &ymin)
{
	min.x = xmin;
	min.y = ymin;
	max.x = xmin;
	min.y = ymin; 
};


Rectangle2::Rectangle2(const Vector2& Position, const Vector2& Size)
{
	min.x = Position.x;
	min.y = Position.y;
	max.x = Position.x + Size.x;
	max.y = Position.y + Size.y;
};