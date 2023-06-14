#pragma once
#include "Entity.h"
#include "ComponentBase.h"
#include "Vector2.h"


const Vector2 DefaultOffset			= ZeroVector;
const float DefaultProjectionWidth	= 100.0f;
const float DefaultProjectionDepth	= 100.0f;
const float DefaultAlpha			= 0.4f;

class CameraComponent : public ComponentBase
{
	bool						ActivationFlag;
	bool						DeadZoneFlag;
	int							TrackedEntityId;
	Vector2						Position;
	Vector2						Offset;
	float						ProjectionWidth;
	float						ProjectionDepth;
	float						Alpha;
	float						DeadZoneRadius;

public:
								CameraComponent();
								~CameraComponent();

	virtual void				Reset();
	inline void					SetActivationFlag(const bool&);
	inline void					SetTrackedEntityId(const int&);
	inline void					SetPosition(const Vector2&);
	inline void					SetOffset(const Vector2&);
	inline void					SetProjectionWidth(const float&);
	inline void					SetProjectionDepth(const float&);
	inline void					SetAlpha(const float&);
	inline void					SetDeadZoneFlag(const bool&);
	inline void					SetDeadZoneRadius(const float&);
	
	inline const bool&			GetActivationFlag() const;
	inline const int&			GetTrackedEntityId() const;
	inline const Vector2		GetPosition() const;
	inline const Vector2&		GetOffset() const;
	inline const float&			GetProjectionWidth() const;
	inline const float&			GetProjectionDepth() const;
	inline const float&			GetAlpha() const;
	inline const bool&			GetDeadZoneFlag() const;
	inline const float&			GetDeadZoneRadius() const;

	inline void					ReleaseTrackedEntity();
};


inline void CameraComponent::SetActivationFlag(const bool& Value)
{
	ActivationFlag = Value;
};


inline void	CameraComponent::SetTrackedEntityId(const int& Value)
{
	TrackedEntityId = Value;
};


inline void	CameraComponent::SetPosition(const Vector2& Value)
{
	Position = Alpha * Value + (1.0f - Alpha)*Position;
};


inline void	CameraComponent::SetOffset(const Vector2& Value)
{
	Offset = Value;
};


inline void	CameraComponent::SetProjectionWidth(const float& Value)
{
	if(Value > 0.0f)
	{
		ProjectionWidth = Alpha * Value + (1.0f - Alpha)*ProjectionWidth;
	}
};


inline void	CameraComponent::SetProjectionDepth(const float& Value)
{
	if(Value > 0.0f)
	{
		ProjectionDepth = Value;
	}
};


inline void	CameraComponent::SetAlpha(const float& Value)
{
	if(Value > 0.0f)
	{
		Alpha = Value;
	}
};
	

inline const bool& CameraComponent::GetActivationFlag() const 
{
	return ActivationFlag;
};


inline const int& CameraComponent::GetTrackedEntityId() const
{
	return TrackedEntityId;
};


inline const Vector2 CameraComponent::GetPosition() const
{
	return Position;
};


inline const Vector2& CameraComponent::GetOffset() const
{
	return Offset;
};


inline const float& CameraComponent::GetProjectionWidth() const
{
	return ProjectionWidth;
};


inline const float&	CameraComponent::GetProjectionDepth() const
{
	return ProjectionDepth;
};


inline const float& CameraComponent::GetAlpha() const
{
	return Alpha;
};


inline void	CameraComponent::ReleaseTrackedEntity()
{
	TrackedEntityId = -1;
};


inline void	CameraComponent::SetDeadZoneFlag(const bool& Value)
{
	DeadZoneFlag = Value;
};


inline void	CameraComponent::SetDeadZoneRadius(const float& Value)
{
	if(Value >= 0.0f) DeadZoneRadius = Value;
};


inline const bool& CameraComponent::GetDeadZoneFlag() const
{
	return DeadZoneFlag;
};


inline const float& CameraComponent::GetDeadZoneRadius() const
{
	return DeadZoneRadius;
};