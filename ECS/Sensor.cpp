#pragma once
#include "stdafx.h"
#include "Sensor.h"

Sensor::Sensor()
{
	Id = 0;

};


Sensor::Sensor(const Polygon2& B, const long long& Idx)
{
	Id = Idx;
	Body = B;
};