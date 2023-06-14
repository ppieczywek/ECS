#pragma once
#include "EntityManagerBase.h"
#include "SystemManagerBase.h"
#include "ResourceManagerBase.h"

class WorldStateBase;

class WorldManagerBase final
{
			bool											InitializationStatus;
			EntityManagerBase								EntityManager;//po co shared prt jak moze byæ 
			SystemManagerBase								SystemManager;
			ResourceManagerBase								ResourceManager;
			std::vector<std::shared_ptr<WorldStateBase>>	StatesStack;
			
	public:

															WorldManagerBase();
			template<class TList, class SList> bool			Initialize();
			void											Update();
			bool											Close();

			inline EntityManagerBase* const					GetEntityManager();
			inline ResourceManagerBase* const				GetResourceManager();
			template<class T> inline T*	const				GetSystem();
			

			inline const unsigned							GetCurrentStateId() const;
			void											ChangeState(std::shared_ptr<WorldStateBase>&);
			void											PushState(std::shared_ptr<WorldStateBase>&);
			void											PopState();
			void											InitializeState();
			void											CleanupState();
			void											UpdateState();
};


template<class TList, class SList> bool WorldManagerBase::Initialize()
{
	if(!InitializationStatus)
	{
		std::cout << "World Manager:	Initializing Entity Manager with provided component list" << std::endl;
		if(!EntityManager.Initialize<TList>())
		{
			std::cout << "World Manager:	Entity Manager initialization failed!" << std::endl;
			return false;
		}

		std::cout << "World Manager:	Initializing System Manager with provided system list" << std::endl;
		if(!SystemManager.Initialize<SList>())
		{
			std::cout << "World Manager:	System Manager initialization failed!" << std::endl;
			return false;
		}

		EntityManager.AcceptObserver(&SystemManager);

		std::cout << "World Manager:	Registering Entity Manager" << std::endl;
		if(!SystemManager.RegisterEntityManager(&EntityManager) )
		{
			std::cout << "World Manager:	Entity Manager registration failed!" << std::endl;
			return false;
		}
		InitializationStatus = true;
		std::cout << "World Manager:	Initialization successfull!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "World Manager:	Already initialized!!" << std::endl;
		return false;
	}
};


inline EntityManagerBase* const WorldManagerBase::GetEntityManager()
{
	return &EntityManager;
};


inline ResourceManagerBase* const WorldManagerBase::GetResourceManager()
{
	return &ResourceManager;
};


template<class T> inline T* const WorldManagerBase::GetSystem()
{
	return SystemManager.GetSystem<T>();
};



