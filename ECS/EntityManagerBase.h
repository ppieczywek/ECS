#pragma once
#include <vector>
#include <assert.h>
#include <typeindex>
#include "Subject.h"
#include "ComponentArrayInitializer.h"
#include "TypeCodeInitializer.h"
#include "ComponentContainer.h"
#include <set>

class EntityManagerBase final : public Subject
{
		bool													SystemStatus;
		bool													InitializationStatus;
		int														NumberOfComponents;	
		
		std::vector<int>										Templates;
		std::vector<int>										ToCreate;
		std::vector<int>										ToDestroy;

		std::set<int>											Allocated;
		std::set<int>											Deallocated;
		std::vector<Entity>										Container;

		std::vector<std::type_index>							ComponentTypeInfo;
		std::vector<std::shared_ptr<ComponentContainerBase>>	componentArray;
		
		Entity*	const											CreateEntity(const long long&);
		bool													DestroyEntity(const int&);

		template<class TList> bool								Initialize();
		bool													Update();
		bool													Close();

public:
		friend class WorldManagerBase;

																EntityManagerBase();
		Entity* const											Create(const long long&);	
		bool													Destroy(const int&);
		bool													Destroy(Entity*);
	
		Entity* const											CreateTemplate(const long long&);
		bool													DestroyTemplate(const int&);
		bool													DestroyTemplate(Entity*);
	
		Entity*	const											CloneEntity(const int&);
		Entity*	const											CloneEntity(const Entity*);
		Entity*	const											GetEntity(const int&);
		
		inline	void											ActivateSystem();
		inline	void											DeactivateSystem();
		inline const bool										GetSystemStatus();
		inline const bool										GetInitializationStatus();

		template<class T> ComponentContainer<T>* const			GetContainer();
		template<class T> T* const							    GetComponent(const Entity*);

		template<class T> const unsigned					    GetComponentIndex();
		template<class T> const long long					    GetComponentMask();
};


template<class T> const unsigned EntityManagerBase::GetComponentIndex()
{
	for(auto ii = 0; ii < NumberOfComponents; ++ii)
	{		
		if(ComponentTypeInfo[ii] == std::type_index(typeid(T))) return static_cast<unsigned>(ii);	
	}
	return -1;
};


template<class T> const long long EntityManagerBase::GetComponentMask()
{
	for(auto ii = 0; ii < NumberOfComponents; ++ii)
	{		
		if(ComponentTypeInfo[ii] == std::type_index(typeid(T))) return (static_cast<long long>(1) << ii);	
	}
	return -1;
};



template<class T>  ComponentContainer<T>* const  EntityManagerBase::GetContainer()
{	
	for(auto ii = 0; ii < NumberOfComponents; ++ii)
	{		
		if(ComponentTypeInfo[ii] == std::type_index(typeid(T))) return dynamic_cast<ComponentContainer<T>*>( componentArray[ii].get() );	
	}
	return nullptr;
};


template<class T> T* const EntityManagerBase::GetComponent(const Entity* Entity)
{
	assert(Entity!=nullptr);
	for(int ii = 0; ii < NumberOfComponents; ++ii)
	{
		if(ComponentTypeInfo[ii] == std::type_index(typeid(T)))
		{
			return static_cast<T*>(componentArray[ii]->operator[](Entity->GetIndex()));		
		}
	}
	return nullptr;
};



template<class TList> bool EntityManagerBase::Initialize()
{
    if(!InitializationStatus)
	{		
		Container.resize(100);

		for(unsigned ii=0; ii < 100; ++ii)
		{
			Deallocated.insert(ii);
		}

		NumberOfComponents = Length<TList>::value;
			
		componentArray.resize(Length<TList>::value);
		ComponentTypeInfo.resize(Length<TList>::value, std::type_index( typeid( int) ) );
	
		ComponentArrayInitializer< Length<TList>::value, TList>::Run(&componentArray[0]);
		TypeCodeInitializer< Length<TList>::value, TList>::Run(&ComponentTypeInfo[0]);
		
		SystemStatus = InitializationStatus = true;
		std::cout << "Entity Manager:	Initialization successfull" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Entity Manager:	Manager already initialized!!" << std::endl;
		return false;
	}
};


void EntityManagerBase::ActivateSystem()
{
	if(InitializationStatus) SystemStatus = true;
};


void EntityManagerBase::DeactivateSystem()
{
	if(InitializationStatus) SystemStatus = false;
};


const bool	EntityManagerBase::GetSystemStatus()
{
	return SystemStatus;
};

const bool	EntityManagerBase::GetInitializationStatus()
{
	return InitializationStatus;
};