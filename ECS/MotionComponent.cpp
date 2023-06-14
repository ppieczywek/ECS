#pragma once
#include "stdafx.h"
#include "MotionComponent.h"

MotionComponent::MotionComponent() 
{
	Sleep = 0;
	TypeOfBody = Dynamic;
	AngularMotion = 1;
	Energy = Density = Mass = InverseMass = Inertia = InverseInertia = LinearDamping = AngularDamping = AngularVelocity = AngularAcceleration = 0.0f;
	LinearVelocity = LinearAcceleration = Vector2(0.0f,0.0f);
};


MotionComponent::~MotionComponent()
{

};


void MotionComponent::Reset()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	Sleep = 0;
	TypeOfBody = Dynamic;
	AngularMotion = 1;
	Energy = Density = Mass = InverseMass = Inertia = InverseInertia = LinearDamping = AngularDamping = AngularVelocity = AngularAcceleration = 0.0f;
	LinearVelocity = LinearAcceleration = Vector2(0.0f,0.0f);
};
