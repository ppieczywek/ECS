#pragma once
#include <vector>
#include "Vector2.h"
#include "ComponentBase.h"
#include "CollisionData.h"

const int DefaultCollisionGroupContainerSize = 10;

class CollisionComponent : public ComponentBase
{
	unsigned													InGroupFlag;
	CollisionType												collisionType;
	unsigned short												CategoryMask;
	unsigned short												CollisionMask;
	float														RestitutionCoeff;
	float														StaticFrictionCoeff;
	float														DynamicFrictionCoeff;
	float														TranslationsNumber;
	Vector2														CumulativeTranslation;
	
	std::vector<std::pair<int,const CollisionData*>>			BodyContact;
	std::vector<std::pair<unsigned,unsigned>>					SensorContact;
	
public:
																CollisionComponent();
																~CollisionComponent();
	virtual void												Reset();
	
	inline void													AccumulateTranslation(const Vector2&);

	inline void													SetCollisionType(const CollisionType&);
	inline void													SetRestitutionCoeff(const float&);
	inline void													SetStaticFrictionCoeff(const float&);
	inline void													SetDynamicFrictionCoeff(const float&);
	inline void													SetCategoryMask(const unsigned short&);
	inline void													SetCollisionMask(const unsigned short&);
	
	inline bool													IsCollidable() const;
	inline const CollisionType&									GetCollisionType() const;
	inline const Vector2										GetTotalTranslation();
	inline const float&											GetRestitutionCoeff() const;
	inline const float&											GetStaticFrictionCoeff() const;
	inline const float&											GetDynamicFrictionCoeff() const;
	inline const unsigned short&								GetCategoryMask() const;
	inline const unsigned short&								GetCollisionMask() const;
		
	inline void													AddBodyContact(const int&,const CollisionData*);
	inline const unsigned										GetBodyContactNumber() const;
	inline void													ClearBodyContact();
	inline std::vector<std::pair<int, const CollisionData*>>&	GetBodyContact();

	inline void													AddSensorContact(const unsigned&, const unsigned&);
	inline const unsigned										GetSensorContactNumber() const;
	inline void													ClearSensorContact();
	inline std::vector<std::pair<unsigned, unsigned>>&			GetSensorContact();

	inline void													SetInGroupFlag(const bool&);
	inline const bool											GetInGroupFlag() const;

	
};


inline void CollisionComponent::AccumulateTranslation(const Vector2& Value)
{
	TranslationsNumber++;
	CumulativeTranslation += Value;
};


void	CollisionComponent::SetCollisionType(const CollisionType& Value)
{
	collisionType = Value;
};


void	CollisionComponent::SetRestitutionCoeff(const float& Value)
{
	RestitutionCoeff = Value < 1.0f ? 1.0f : Value;
	if(RestitutionCoeff > 2.0f) RestitutionCoeff = 2.0f;
};


void	CollisionComponent::SetStaticFrictionCoeff(const float& Value)
{
	StaticFrictionCoeff = Value >= 0.0f ? Value : 0.0f;
};


void	CollisionComponent::SetDynamicFrictionCoeff(const float& Value)
{
	DynamicFrictionCoeff = Value >= 0.0f ? Value : 0.0f;
};



bool	CollisionComponent::IsCollidable() const
{
	return collisionType != None;
};


const CollisionType&	CollisionComponent::GetCollisionType() const
{
	return collisionType;
};


const Vector2	CollisionComponent::GetTotalTranslation()
{	
	auto Total = TranslationsNumber > 0.0f ? (CumulativeTranslation / TranslationsNumber) : ZeroVector;
	CumulativeTranslation = ZeroVector;
	TranslationsNumber = 0.0f;
	return Total;
};


const float&	CollisionComponent::GetRestitutionCoeff() const
{
	return RestitutionCoeff;
};


const float&	CollisionComponent::GetStaticFrictionCoeff() const
{
	return StaticFrictionCoeff;
};

const float&	CollisionComponent::GetDynamicFrictionCoeff() const
{
	return DynamicFrictionCoeff;
};


inline const unsigned short& CollisionComponent::GetCategoryMask() const
{
	return CategoryMask;
};


inline const unsigned short& CollisionComponent::GetCollisionMask() const
{
	return CollisionMask;
};


inline void	CollisionComponent::SetCategoryMask(const unsigned short& Value)
{
	CategoryMask = Value;
};


inline void	CollisionComponent::SetCollisionMask(const unsigned short& Value)
{
	CollisionMask = Value;
};


inline void	CollisionComponent::ClearBodyContact()
{
	BodyContact.clear();
};


inline void	CollisionComponent::AddBodyContact(const int& Value,const CollisionData* Data)
{
	BodyContact.push_back(std::make_pair(Value, Data));
};


inline const unsigned CollisionComponent::GetBodyContactNumber() const
{
	return BodyContact.size();
};


inline void	CollisionComponent::SetInGroupFlag(const bool& Value)
{
	if(Value)
		InGroupFlag = 1;
	else
		InGroupFlag = 0;
};


inline const bool CollisionComponent::GetInGroupFlag() const
{
	return (InGroupFlag==1);
};


inline std::vector<std::pair<int,const CollisionData*>>& CollisionComponent::GetBodyContact()
{
	return BodyContact;
};


inline void CollisionComponent::AddSensorContact(const unsigned& SensorIndex, const unsigned& EntityIndex)
{
	SensorContact.push_back( std::make_pair(SensorIndex, EntityIndex) );
};


inline const unsigned CollisionComponent::GetSensorContactNumber() const
{
	return SensorContact.size();
};


inline void	CollisionComponent::ClearSensorContact()
{
	SensorContact.clear();
};


inline std::vector<std::pair<unsigned, unsigned>>&	CollisionComponent::GetSensorContact()
{
	return SensorContact;
};