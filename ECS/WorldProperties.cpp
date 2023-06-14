#pragma once
#include "stdafx.h"
#include "WorldProperties.h"

WorldProperties::WorldProperties() : Gravity(true), Drag(true), Friction(true), GravityMagnitude(0.0f,-35.0f) 
{
	TimeStep = DefaultTimeStep;
	
};


