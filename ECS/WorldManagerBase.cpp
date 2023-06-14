#pragma once
#include "stdafx.h"
#include "WorldManagerBase.h"
#include "WorldStateBase.h"


WorldManagerBase::WorldManagerBase()
{
	InitializationStatus = false;
};


void WorldManagerBase::Update()
{
	if(InitializationStatus)
	{
		UpdateState();
		EntityManager.Update();
		SystemManager.Update();
	}
};


bool WorldManagerBase::Close()
{
	if(InitializationStatus != false)
	{
		std::cout << "World Manager:	Entity Manager closing." << std::endl;
		EntityManager.Close();
		std::cout << "World Manager:	System Manger closing." << std::endl;
		SystemManager.Close();
		std::cout << "World Manager:	Resource Manger closing." << std::endl;
		ResourceManager.Close();

		InitializationStatus = false;
		std::cout << "World Manager:	Closed!." << std::endl;
		return true;
	}
	else
	{
		std::cout << "World Manager:	Manager already closed!." << std::endl;
		return false;
	}
};


void WorldManagerBase::CleanupState()
{
	while ( !StatesStack.empty() )
	{
		StatesStack.back()->Cleanup(*this);
		StatesStack.pop_back();
	}
};


void WorldManagerBase::ChangeState(std::shared_ptr<WorldStateBase>& State) 
{
	if ( !StatesStack.empty() )
	{
		StatesStack.back()->Cleanup(*this);
		StatesStack.pop_back();
	}
	StatesStack.push_back(State);
	StatesStack.back()->Initialize(*this);
};


void WorldManagerBase::PushState(std::shared_ptr<WorldStateBase>& State) 
{
	if ( !StatesStack.empty() ) 
	{
		StatesStack.back()->Pause();
	}

 	StatesStack.push_back(State);
	StatesStack.back()->Initialize(*this);
};	


void WorldManagerBase::InitializeState() 
{
	if ( !StatesStack.empty() ) 
	{
		StatesStack.back()->Initialize(*this);
	}
};


void WorldManagerBase::PopState()
{
	if ( !StatesStack.empty() ) 
	{
		StatesStack.back()->Cleanup(*this);
		StatesStack.pop_back();
	}

	if ( !StatesStack.empty() ) 
	{
		StatesStack.back()->Resume();
	}
};

void WorldManagerBase::UpdateState() 
{
	if ( !StatesStack.empty() )
	{
		StatesStack.back()->Update(*this);
	}
};


inline const unsigned WorldManagerBase::GetCurrentStateId() const
{
	if ( !StatesStack.empty() )
	{
		return StatesStack.back()->GetStateId();
	}
	else
	{
		return 0;
	}
};