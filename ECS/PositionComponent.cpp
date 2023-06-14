#pragma once
#include "stdafx.h"
#include "PositionComponent.h"


PositionComponent::PositionComponent() : Position(0.0f,0.0f), Angle(0.0f)
{
	
};


PositionComponent::~PositionComponent()
{

};


void PositionComponent::Reset()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	Position.x = 0.0f;
	Position.y = 0.0f;
	Angle = 0.0f;
};


void NormalizeAngle(float& startAngle)
{
	startAngle = fmod(startAngle, tau);
	if(startAngle < 0)
		startAngle += tau;
};