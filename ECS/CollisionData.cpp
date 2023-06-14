#pragma once
#include "stdafx.h"
#include "CollisionData.h"


CollisionData::CollisionData() 
{
	Clear();
}; 

void CollisionData::Clear() 
{
	Body1 = Body2 = Face1 = Face2 = 0;
	DenominatorNormal = DenominatorTangent = BiasNormal = DynamicFrictionCoeff = DynamicFrictionCoeff = TangentImpulseMagnitude = NormalImpulseMagnitude = Restitution = Penetration = 0.0f;
	R1 = R2 = BiasTangent= MinimumTranslationVector = CollisionPoint = CollisionTangent = CollisionNormal = NormalImpulse = TangentImpulse = ZeroVector;
	Overlap = Impulse = 0;
};



void ClampNormal(Vector2& Normal)
{
	if(fabs(Normal.x) < 0.01f)
	{
		Normal.x = 0.0f;
		if(Normal.y > 0.0f)
			Normal.y = 1.0f;
		else
			Normal.y = -1.0f;
	}
	else if(fabs(Normal.y) < 0.01f)
	{
		Normal.y = 0.0f;
		if(Normal.x > 0.0f)
			Normal.x = 1.0f;
		else
			Normal.x = -1.0f;
	}
};