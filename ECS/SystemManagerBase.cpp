#pragma once
#include "stdafx.h"
#include "SystemManagerBase.h"


SystemManagerBase::SystemManagerBase()
{
	InitializationStatus = false;
};


bool SystemManagerBase::RegisterEntityManager(EntityManagerBase* EntityManager)
{
	if(EntityManager != nullptr)
	{
		for(int ii=0; ii<NumberOfSystems; ++ii)
		{
			SystemArray[ii]->RegisterEntityManager(EntityManager);
		}
		return true;
	}
	return false;
};


void SystemManagerBase::Register(const Entity* Entity)
{
	for(int ii=0; ii<NumberOfSystems; ++ii)
	{
		SystemArray[ii]->Register(Entity);
	}
};


void SystemManagerBase::Unregister(const Entity* Entity)
{
	for(int ii=0; ii<NumberOfSystems; ++ii)
	{
		SystemArray[ii]->Unregister(Entity);
	}
};


void SystemManagerBase::Update()
{
	for(int ii=0; ii < NumberOfSystems; ++ii)
	{
		if (SystemArray[ii]->GetSystemStatus())
		{
			SystemArray[ii]->Update();
		}
	}
};


void SystemManagerBase::Close()
{
	std::cout << "System Manager:	Closing Systems." << std::endl;
	for(int ii=0; ii<NumberOfSystems; ++ii)
	{
		SystemArray[ii]->Close();
	}
	InitializationStatus = false;
	std::cout << "System Manager:	Closed!" << std::endl;
};

void SystemManagerBase::OnCreateEntity(const Entity* Entity)
{
	Register(Entity);
};


void SystemManagerBase::OnDestroyEntity(const Entity* Entity)
{
	Unregister(Entity);
};