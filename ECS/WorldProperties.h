#pragma once
#include "Vector2.h"
#include "Rectangle2.h"

const float DefaultTimeStep = 1.0f/60.0f;
const float MaxImpulse = 1000.0f;
const float MinImpulse = 0.00f;//0.005f;
const float VelocityThreshold = 0.0f; //1.1
const float PlasticCollisionThreshold = -0.4f;// -0.5f;



class WorldProperties
{
	bool						Gravity;
	bool						Drag;
	bool						Friction;
	float						TimeStep;
	Vector2						GravityMagnitude;
		
public:
	
								WorldProperties(); 

	inline void					SetGravityState(const bool&);
	inline void					SetFrictionState(const bool&);
	inline void					SetDragState(const bool&);
	inline void					SetGravityMagnitude(const Vector2&);
	inline void					SetTimeStep(const float&);
	
	inline const bool&			GetGravityState() const;
	inline const bool&			GetFrictionState() const;
	inline const bool&			GetDragState() const;
	inline const Vector2&		GetGravityMagnitude() const;
	inline const float&			GetTimeStep() const;
};


void WorldProperties::SetGravityState(const bool &GravityFlag)			
{
	Gravity = GravityFlag;
};


void WorldProperties::SetFrictionState(const bool &FrictionFlag)
{
	Friction = FrictionFlag;
};


void WorldProperties::SetDragState(const bool &DragFlag)
{
	Drag = DragFlag;
};


void WorldProperties::SetGravityMagnitude(const Vector2 &Gravity)
{
	GravityMagnitude = Gravity;
};


void WorldProperties::SetTimeStep(const float &Step)	
{
	TimeStep = Step > 0.0f ? Step : DefaultTimeStep;
};


const bool& WorldProperties::GetGravityState() const
{
	return Gravity;
};


const bool& WorldProperties::GetFrictionState() const							 
{
	return Friction;
};


const bool& WorldProperties::GetDragState()	const 							 
{
	return Drag;
};


const Vector2&	WorldProperties::GetGravityMagnitude() const							
{
	return GravityMagnitude;
};


const float& WorldProperties::GetTimeStep() const						
{
	return TimeStep;
};

