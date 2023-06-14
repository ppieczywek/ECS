#pragma once
#include "stdafx.h"
#include "CollisionComponent.h"


CollisionComponent::CollisionComponent()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	InGroupFlag					= 0;
	collisionType				= Both;
	RestitutionCoeff			= 1.15f;
	StaticFrictionCoeff			= 0.5f;
	DynamicFrictionCoeff		= 0.25f;
	TranslationsNumber			= 0.0f;
	CumulativeTranslation		= ZeroVector;
	CategoryMask				= 0x0001;
	CollisionMask				= 0xFFFF;
	BodyContact.reserve(25);
	SensorContact.reserve(25);
};


CollisionComponent::~CollisionComponent()
{
	
};


void CollisionComponent::Reset()
{
	TypeMask					= 0;
	Index						= -1;
	OwnerIndex					= -1;
	collisionType				= Both;
	RestitutionCoeff			= 1.0f;
	StaticFrictionCoeff			= 0.0f;
	DynamicFrictionCoeff		= 0.0f;
	TranslationsNumber			= 0.0f;
	CumulativeTranslation		= ZeroVector;
	CategoryMask				= 0x0001;
	CollisionMask				= 0xFFFF;
	InGroupFlag					= 0;
	BodyContact.clear();
	SensorContact.clear();
};

