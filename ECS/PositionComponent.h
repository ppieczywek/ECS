#pragma once
#include "ComponentBase.h"
#include "Vector2.h"
#include <math.h>



void NormalizeAngle(float& startAngle);

class PositionComponent : public ComponentBase
{
		Vector2						Position;
		float						Angle;	
public:
									PositionComponent();
									~PositionComponent();

		virtual void				Reset();
		//inline	PositionComponent&	operator=(const PositionComponent&);
		inline void					SetPosition(const Vector2&);
		inline void					SetAngle(const float&);
		inline const Vector2&		GetPosition() const;
		inline const float&			GetAngle() const;
		inline void					AddPosition(const Vector2&);
		inline void					AddAngle(const float&);
};
/*
inline PositionComponent&	PositionComponent::operator=(const PositionComponent& PositionComponent)
{
	Position = PositionComponent.Position;
	Angle	 = PositionComponent.Angle;
	return *this;
};*/

inline void	PositionComponent::SetPosition(const Vector2& Value)
{
	Position = Value;
};


inline void	PositionComponent::SetAngle(const float& Value)
{
	NormalizeAngle(Angle = Value);
};


inline const Vector2& PositionComponent::GetPosition() const
{
	return Position;
};


inline const float& PositionComponent::GetAngle() const 
{
	return Angle;
};


inline void PositionComponent::AddPosition(const Vector2& Value)
{
	Position+=Value;
};


inline void	PositionComponent::AddAngle(const float& Value)
{
	NormalizeAngle(Angle+=Value);
};