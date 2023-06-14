#pragma once
#include "stdafx.h"
#include "EntityManagerBase.h"


EntityManagerBase::EntityManagerBase()
{
	SystemStatus = InitializationStatus = false;
};


Entity*	const EntityManagerBase::CreateEntity(const long long& EntityMask)
{		
	if(EntityMask != 0)
	{
		if(Deallocated.empty())
		{
			auto CurrentSize = Container.size();
			Container.resize(2*CurrentSize);
			int ContainerSize = static_cast<int>(Container.size());
	
			for(unsigned ii=CurrentSize; ii < 2*CurrentSize; ++ii)
			{
				Deallocated.insert(ii);
			}
		}
	
		auto it = Deallocated.begin();
		unsigned EntityIndex = *it;
	
		Deallocated.erase(it);
		Allocated.insert(EntityIndex);
		
		Container[EntityIndex].SetIndex(EntityIndex);
		Container[EntityIndex].SetComponentMask(EntityMask);

		for(int ii=0; ii < NumberOfComponents; ++ii)
		{
			long long ComponentMask = static_cast<long long>(1) << ii;
			if( (EntityMask & ComponentMask) > 0 )
			{
				auto Component = componentArray[ii]->Connect(&Container[EntityIndex]);
				if( Component != nullptr )
				{
					Component->Reset();
					Component->SetOwnerIndex(EntityIndex);
					Component->SetTypeMask(ComponentMask);
				}
			}
		}
		return &Container[EntityIndex];
	}
	return nullptr;
};


Entity* const EntityManagerBase::Create(const long long& EntityMask)
{
	auto Entity = CreateEntity(EntityMask);
	if (Entity!=nullptr) ToCreate.push_back(Entity->GetIndex());
	return Entity;
};


bool EntityManagerBase::DestroyEntity(const int& EntityIndex)
{
	if(EntityIndex >= 0 && EntityIndex < static_cast<int>(Container.size()) )
	{		
		for(int ii=0; ii < NumberOfComponents; ++ii)
		{
			long long ComponentMask = static_cast<long long>(1) << ii;
			if(Container[EntityIndex].HasComponent(ComponentMask))
			{				
				componentArray[ii]->Disconnect(&Container[EntityIndex]);
			}
		}
		
		if( Allocated.erase(EntityIndex) > 0 )
		{
			Deallocated.insert(EntityIndex);
		}	
		Container[EntityIndex].SetIndex(-1);
		Container[EntityIndex].SetComponentMask(0);
		return true;
	}
	return false;
};


bool EntityManagerBase::Destroy(const int& EntityIndex)
{
	if (EntityIndex >= 0 && EntityIndex < static_cast<int>(Container.size()))
	{
		for(unsigned ii=0; ii < Templates.size(); ++ii)
		{
			if(Templates[ii] == EntityIndex)
			{
				return false;
			}
		}
		ToDestroy.push_back(EntityIndex);
		return true;
	}
	return false;
};


bool EntityManagerBase::Destroy(Entity* Entity)
{
	//assert(Entity!=nullptr);
	if(Entity!=nullptr)
	{	
		return Destroy(Entity->GetIndex());
	}
	return false;
};


Entity*	const EntityManagerBase::CloneEntity(const int& EntityIndex)
{
	if (EntityIndex >= 0 && EntityIndex < static_cast<int>(Container.size()))
	{
		auto CloneMask = Container[EntityIndex].GetComponentMask();
		auto Entity = CreateEntity(CloneMask);
		
		if( Entity != nullptr )
		{
			for(int ii=0; ii < NumberOfComponents; ++ii)
			{
				long long ComponentMask = static_cast<long long>(1) << ii;
				if( Entity->HasComponent(ComponentMask) )
				{			
					componentArray[ii]->Copy( &Container[EntityIndex], Entity);
					auto CloneComponent = componentArray[ii]->operator[](Entity->GetIndex());
					if (CloneComponent != nullptr)
					{
						CloneComponent->SetOwnerIndex(Entity->GetIndex());
					}
				}
			}
			ToCreate.push_back(Entity->GetIndex());
			return Entity;
		}
	}
	return nullptr;
};


Entity*	const EntityManagerBase::CloneEntity(const Entity* Entity)
{
	if(Entity != nullptr)
	{	
		return CloneEntity(Entity->GetIndex());
	}
	return false;
};


Entity* const EntityManagerBase::CreateTemplate(const long long& TemplateMask)
{
	auto Entity = CreateEntity(TemplateMask);
	if(Entity != nullptr) Templates.push_back(Entity->GetIndex());
	return Entity;
};


bool EntityManagerBase::DestroyTemplate(const int& TemplateIndex)
{
	for(unsigned ii=0; ii < Templates.size(); ++ii)
	{
		if(Templates[ii] == TemplateIndex)
		{
			ToDestroy.push_back(TemplateIndex);
			Templates.erase(Templates.begin() + ii);
			return true;
		}
	}
	return false;
};


bool EntityManagerBase::DestroyTemplate(Entity* Template)
{
	//assert(Template!=nullptr);
	if(Template!=nullptr)
	{	
		return Destroy(Template->GetIndex());
	}
	return false;
};


Entity*	const EntityManagerBase::GetEntity(const int& Index)
{
	if (Index >= 0 && Index < static_cast<int>(Container.size()))
	{
		return &Container[Index];
	}
	return nullptr;
};


bool EntityManagerBase::Update()
{
	if(SystemStatus && InitializationStatus)
	{
		auto Observer = GetObserver();

		if(Observer != nullptr)
		{
			for(auto it = ToCreate.begin(); it != ToCreate.end(); ++it)
			{
				Observer->OnCreateEntity(GetEntity(*it));
			}
	
			for(auto it = ToDestroy.begin(); it != ToDestroy.end(); ++it)
			{
				Observer->OnDestroyEntity(GetEntity(*it));
				//DestroyEntity(*it);
			}
		}

		for(auto it = ToDestroy.begin(); it != ToDestroy.end(); ++it)
		{
			DestroyEntity(*it);
		}

		ToCreate.clear();
		ToDestroy.clear();

		return true;
	}
	return false;
};


bool EntityManagerBase::Close()
{
	if(SystemStatus && InitializationStatus)
	{
		for(int ii=0; ii < NumberOfComponents; ++ii)
		{
			componentArray[ii]->Clear();
		}
			
		Templates.clear();
		ToCreate.clear();
		ToDestroy.clear();

		Allocated.clear();
		Deallocated.clear();
		Container.clear();
	
		ComponentTypeInfo.clear();
		componentArray.clear();
		return true;
	}
	return false;
};

