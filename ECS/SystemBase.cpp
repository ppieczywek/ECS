#pragma once
#include "stdafx.h"
#include "SystemBase.h"


SystemBase::SystemBase()
{
	SystemStatus = false;
	InitializationStatus = false;
};


bool SystemBase::RegisterEntityManager(EntityManagerBase* EntityManagerBase)
{
	if(EntityManagerBase != nullptr)
	{
		EntityManager = EntityManagerBase;
		return true;
	}
	return false;
};