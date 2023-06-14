#pragma once

class ComponentBase
{
protected:
	
	int								Index;
	int								OwnerIndex;
	long long						TypeMask;
	
	inline void						SetTypeMask(const long long&);
	inline void						SetOwnerIndex(const int&);
	inline void						SetIndex(const int&);
public:
				
	friend class EntityManagerBase;
									ComponentBase();	
	virtual							~ComponentBase();
	virtual void					Reset()								= 0;
	//inline ComponentBase&			operator=(const ComponentBase&);
	
	inline const long long&			GetTypeMask() const;
	inline const int&				GetOwnerIndex() const;  
	inline const int&				GetIndex() const;
};

/*
inline ComponentBase&	ComponentBase::operator=(const ComponentBase& ComponentBase)
{
	return *this;
};*/


void ComponentBase::SetTypeMask(const long long& Value)
{
	TypeMask = Value;
};


void ComponentBase::SetOwnerIndex(const int& Value)
{
	OwnerIndex = Value;
};


const long long& ComponentBase::GetTypeMask() const
{
	return TypeMask;
};


const int& ComponentBase::GetOwnerIndex() const
{
	return OwnerIndex;
};


void ComponentBase::SetIndex(const int& Value)
{
	Index = Value;
};


const int&	ComponentBase::GetIndex() const
{
	return Index;
};
