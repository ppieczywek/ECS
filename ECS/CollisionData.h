#pragma once
#include "Vector2.h"

enum CollisionType {Logical, Physical, Both, None};

class CollisionData 
{
public:
	int					Body1, Body2;
	int					Face1,Face2;
	int					Impulse;
	int					Overlap;
	CollisionType		cType;
	float				Restitution;
	float				Penetration;
	float				NormalImpulseMagnitude;
	float				TangentImpulseMagnitude;
	float				BiasNormal;
	float				StaticFrictionCoeff;
	float				DynamicFrictionCoeff;
	Vector2				MinimumTranslationVector;
	Vector2				CollisionNormal;
	Vector2				CollisionTangent;
	Vector2				CollisionPoint;
	Vector2				TangentImpulse;
	Vector2				NormalImpulse;
	Vector2				R1;
	Vector2				R2;
	Vector2				BiasTangent;
	float				DenominatorNormal;
	float				DenominatorTangent;

						CollisionData();
	void				Clear();		
};

void ClampNormal(Vector2& Normal);