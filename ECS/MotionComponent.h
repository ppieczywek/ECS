#pragma once
#include "Vector2.h"
#include "ComponentBase.h"


enum BodyType {Static, Kinematic, Dynamic};
enum PropertyIndicator {ON, OFF};

const float EnergyBias			= 0.82f; //0.82
const float OneMinusEnergyBias	= 1.0f - EnergyBias;
const float EnergyThreshold		= 0.09f; //0.045

class MotionComponent : public ComponentBase
{
	unsigned							Sleep; //zmienic na unsigned
	unsigned							AngularMotion;
	BodyType							TypeOfBody;
	float								Density;
	float								Mass;
	float								InverseMass;
	float								Inertia;
	float								InverseInertia;
	float								LinearDamping;
	float								AngularDamping;
	float								AngularVelocity;
	float								AngularAcceleration;
	float								Energy;
	Vector2								LinearVelocity;
	Vector2								LinearAcceleration;
	
public:

										MotionComponent();
										~MotionComponent();
	virtual void						Reset();
	inline const BodyType&				GetTypeOfBody()	const;
	inline const float&					GetDensity() const;
	inline const float&					GetMass() const;
	inline const float&					GetInverseMass() const;
	inline const float&					GetInertia() const;
	inline const float&					GetInverseInertia() const;
	inline const float&					GetLinearDamping() const;
	inline const float&					GetAngularDamping() const;
	inline const float&					GetAngularVelocity() const;
	inline const float&					GetAngularAcceleration() const;
	inline const Vector2&				GetLinearVelocity() const;
	inline const Vector2				GetLinearVelocityAtLocalPoint(const Vector2&) const;
	inline const Vector2&				GetLinearAcceleration() const;
	inline const bool					GetAngularMotion() const;
	inline const float&					GetEnergy() const;
	inline const bool					GetSleep() const;

	inline void							SetTypeOfBody(const BodyType&);
	inline void							SetDensity(const float&);
	inline void							SetMass(const float&);
	inline void							SetInertia(const float&);
	inline void							SetLinearDamping(const float&);
	inline void							SetAngularDamping(const float&);
	inline void							SetAngularVelocity(const float&);
	inline void							SetLinearVelocity(const Vector2&);
	inline void							SetAngularAcceleration(const float&);
	inline void							SetLinearAcceleration(const Vector2&);
	inline void							ApplyForce(const Vector2&, const Vector2&);
	inline void							ApplyTorque(const float&);
	inline void							ApplyImpulse(const Vector2&, const Vector2&);
	inline void							ApplyAngularImpulse(const float&);
	inline void							SetAngularMotion(const bool&);
	inline void							SetSleep(const bool&);	
};


inline const BodyType&	MotionComponent::GetTypeOfBody() const
{
	return TypeOfBody;
};


inline const float&	MotionComponent::GetDensity() const
{
	return Density;
};


inline const float&	MotionComponent::GetMass() const
{
	return Mass;
};


inline const float&	MotionComponent::GetInverseMass() const
{
	return InverseMass;
};


inline const float&	MotionComponent::GetInertia() const
{
	return Inertia;
};


inline const float&	MotionComponent::GetInverseInertia() const
{
	return InverseInertia;
};


inline const float&	MotionComponent::GetLinearDamping() const
{
	return LinearDamping;
};


inline const float&	MotionComponent::GetAngularDamping() const
{
	return AngularDamping;
};


inline const float&	MotionComponent::GetAngularVelocity() const
{
	return AngularVelocity;
};


inline const float&	MotionComponent::GetAngularAcceleration() const
{
	return AngularAcceleration;
};


inline const Vector2& MotionComponent::GetLinearVelocity() const
{
	return LinearVelocity;
};


inline const Vector2 MotionComponent::GetLinearVelocityAtLocalPoint(const Vector2& LocalPoint) const
{
	return Vector2( LinearVelocity.x - AngularVelocity * LocalPoint.y, LinearVelocity.y + AngularVelocity * LocalPoint.x); //nie wiadomo czy na 100% dobre
};


inline const Vector2& MotionComponent::GetLinearAcceleration() const
{
	return LinearAcceleration;
};


inline const bool	MotionComponent::GetAngularMotion() const
{
	return (AngularMotion == 1);
};


inline void	MotionComponent::SetTypeOfBody(const BodyType& Value)
{
	TypeOfBody = Value;
	if(TypeOfBody == Static)
	{
		AngularAcceleration = AngularVelocity = 0.0f;
		LinearAcceleration = LinearVelocity = ZeroVector;
		Energy = 0.0f;
		Sleep = 0; //by³o true
	}
	else if(TypeOfBody == Kinematic)
	{
		AngularAcceleration = 0.0f;
		LinearAcceleration = ZeroVector;
		Energy = 0.0f;
		Sleep = 0;
	}
};


inline void	MotionComponent::SetDensity(const float& Value)
{
	Density = Value;
};


inline void	MotionComponent::SetMass(const float& Value)
{
	Mass = Value;
	InverseMass = Mass > 0.0f ? 1.0f / Mass : 0.0f;
};


inline void	MotionComponent::SetInertia(const float& Value)
{
	Inertia = Value;
	InverseInertia = Inertia > 0.0f ? 1.0f / Inertia : 0.0f;
};


inline void	MotionComponent::SetLinearDamping(const float& Value)
{
	LinearDamping = Value;
};


inline void	MotionComponent::SetAngularDamping(const float& Value)
{
	AngularDamping = Value;
};


inline void	MotionComponent::SetAngularVelocity(const float& Value)
{
	if(TypeOfBody != Static)
	{
		AngularVelocity = Value;
		Energy = EnergyBias*Energy + OneMinusEnergyBias*(LinearVelocity*LinearVelocity + AngularVelocity*AngularVelocity);
	}
};


inline void	MotionComponent::SetLinearVelocity(const Vector2& Value)
{
	if(TypeOfBody != Static)
	{
		LinearVelocity = Value;
		Energy = EnergyBias*Energy + OneMinusEnergyBias*(LinearVelocity*LinearVelocity + AngularVelocity*AngularVelocity);
	}
};


inline void	MotionComponent::SetAngularAcceleration(const float& Value)
{
	if(TypeOfBody == Dynamic) AngularAcceleration = Value;
};


inline void	MotionComponent::SetLinearAcceleration(const Vector2& Value)
{
	if(TypeOfBody == Dynamic) LinearAcceleration = Value;
};


inline void	MotionComponent::ApplyForce(const Vector2& Force, const Vector2& LocalPoint)
{
	if(TypeOfBody == Dynamic)
	{
		LinearAcceleration += Force * InverseMass;
		if(AngularMotion == ON) AngularAcceleration += (LocalPoint.x * Force.y - LocalPoint.y*Force.x) *  InverseInertia;
	};
};


inline void	MotionComponent::ApplyTorque(const float& Torque)
{
	if(TypeOfBody == Dynamic) AngularAcceleration += Torque *  InverseInertia;
};


inline void	MotionComponent::ApplyImpulse(const Vector2& Impulse, const Vector2& LocalPoint)
{
	if(TypeOfBody == Dynamic)
	{
		LinearVelocity += Impulse * InverseMass;
		if(AngularMotion == 1) AngularVelocity += (LocalPoint.x * Impulse.y - LocalPoint.y * Impulse.x) *  InverseInertia;
		Energy = EnergyBias*Energy + OneMinusEnergyBias*(LinearVelocity*LinearVelocity + AngularVelocity*AngularVelocity);
	};
};


inline void	MotionComponent::ApplyAngularImpulse(const float& AngularImpulse) //R x Impulse
{
	if(AngularMotion == 1)
	{
		AngularVelocity += AngularImpulse *  InverseInertia;
		Energy = EnergyBias*Energy + OneMinusEnergyBias*(LinearVelocity*LinearVelocity + AngularVelocity*AngularVelocity);
	}
};
	

inline void	MotionComponent::SetAngularMotion(const bool& Value)
{
	if(Value)
	{
		AngularMotion = 1;
	}
	else
	{
		AngularMotion = 0;
	}
};

 
inline const float&	 MotionComponent::GetEnergy() const
{
	return Energy;
};


inline const bool MotionComponent::GetSleep() const
{
	return (Sleep==1);
};


inline void	MotionComponent::SetSleep(const bool& Value)
{
	if(Value)
	{
		Sleep = 1;
		LinearAcceleration = LinearVelocity = ZeroVector;
		AngularAcceleration = AngularVelocity = 0.0f;
	}
	else
	{
		Sleep = 0;
		Energy = 10.0f*EnergyThreshold;
	}
};