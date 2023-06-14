#pragma once
#include <typeindex> 
#include <iostream>
#include "Observer.h"
#include "SystemBase.h"
#include "SystemInitializer.h"

class SystemManagerBase final : public Observer
{
			bool											InitializationStatus;
			int												NumberOfSystems;
			std::vector<std::type_index>					SystemTypeInfo; 
			std::vector<std::shared_ptr<SystemBase>>		SystemArray;

			template<class SystemList> bool					Initialize();
			bool											RegisterEntityManager(EntityManagerBase*);
			void											Register(const Entity*);
			void											Unregister(const Entity*);
			void											Update();
			void											Close();
			void											OnCreateEntity(const Entity*);
			void											OnDestroyEntity(const Entity*);

	public:
			friend class WorldManagerBase;
															SystemManagerBase();
			template<class SystemType> SystemType* const	GetSystem();
};


template<class SystemList> bool	SystemManagerBase::Initialize()
{
	if(!InitializationStatus)
	{
		NumberOfSystems = Length<SystemList>::value;
		SystemArray.resize(Length<SystemList>::value);
		SystemTypeInfo.resize(Length<SystemList>::value, std::type_index( typeid( int) ) );

		SystemInitializer< Length<SystemList>::value, SystemList>::Run(&SystemArray[0]);
		TypeCodeInitializer< Length<SystemList>::value, SystemList>::Run(&SystemTypeInfo[0]);
		
		std::cout << "System Manager:	Initializing systems." << std::endl;
		for(int ii=0; ii<NumberOfSystems; ++ii)
		{
			bool Result = SystemArray[ii]->Initialize();
			if(!Result)
			{
				std::cout << "System Manager:	System initialization failed!" << std::endl;
				return false;
			}
		}
		InitializationStatus = true;
		std::cout << "System Manager:	Initialization successfull!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "System Manager:	Already initialized!!" << std::endl;
		return false;
	}
};


template<class SystemType> SystemType* const SystemManagerBase::GetSystem()
{
	//auto hc = typeid(SystemType).hash_code();
	auto hc = std::type_index(typeid(SystemType));//.hash_code();
	for(auto ii = 0; ii < NumberOfSystems; ++ii)
	{
		if(SystemTypeInfo[ii] == hc) return static_cast<SystemType*>(  SystemArray[ii].get() );	
	}
	return nullptr;
};


