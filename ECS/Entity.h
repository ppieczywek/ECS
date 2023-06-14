#pragma once

class Entity final
{
protected:

	int									Index;
	long long							ComponentMask;

	inline void							SetComponentMask(const long long&);
	inline void							SetIndex(const int&);
public:
	
	friend class EntityManagerBase;
										Entity();
										~Entity();
		
	inline const long long&				GetComponentMask() const;
	inline bool							HasComponent(const long long&) const;
	inline const int&					GetIndex() const;
};



void Entity::SetIndex(const int& Value)
{
	Index = Value;
};


const int&	Entity::GetIndex() const
{
	return Index;
};


void Entity::SetComponentMask(const long long& Value)
{
	ComponentMask = Value;
};



const long long& Entity::GetComponentMask() const
{
	return ComponentMask;
};


bool Entity::HasComponent(const long long& TypeMask) const
{
	return (ComponentMask & TypeMask) == TypeMask;
};