#pragma once
#include <set>
#include <stack>
#include "EntityManagerBase.h"


class SystemBase
{
	protected:

			bool				SystemStatus;
			bool				InitializationStatus;
			EntityManagerBase*	EntityManager;

			virtual bool		Initialize() = 0;
			virtual bool		Update() = 0;
			bool				RegisterEntityManager(EntityManagerBase*);
			virtual bool		Register(const Entity*) = 0;
			virtual bool		Unregister(const Entity*) = 0;
			virtual bool		Close() = 0;
			
	public:
			
			friend class SystemManagerBase;
			friend class WorldManagerBase;

								SystemBase();
			inline	void		ActivateSystem();
			inline	void		DeactivateSystem();
			inline const bool&	GetSystemStatus();
			inline const bool&	GetInitializationStatus();


};


void SystemBase::ActivateSystem()
{
	if(InitializationStatus) SystemStatus = true;
};


void SystemBase::DeactivateSystem()
{
	if(InitializationStatus) SystemStatus = false;
};


const bool&	SystemBase::GetSystemStatus()
{
	return SystemStatus;
};

const bool&	SystemBase::GetInitializationStatus()
{
	return InitializationStatus;
};