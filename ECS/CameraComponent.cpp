#pragma once
#include "stdafx.h"
#include "CameraComponent.h"


CameraComponent::CameraComponent() 
{
	TrackedEntityId = -1;
	ActivationFlag	= false;
	Position = Offset = DefaultOffset;
	ProjectionWidth = DefaultProjectionWidth;
	ProjectionDepth = DefaultProjectionDepth;
	Alpha			= DefaultAlpha;
	DeadZoneFlag    = true;
	DeadZoneRadius  = 0.0f;
};


CameraComponent::~CameraComponent()
{

};


void CameraComponent::Reset()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	Position.x = 0.0f;
	Position.y = 0.0f;
	TrackedEntityId = -1;
	Offset			= DefaultOffset;
	ProjectionWidth = DefaultProjectionWidth;
	ProjectionDepth = DefaultProjectionDepth;
	Alpha			= DefaultAlpha;
	ActivationFlag	= false;
};

