#pragma once
#include "stdafx.h"
#include "ComponentBase.h"


ComponentBase::ComponentBase()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	
};


ComponentBase::~ComponentBase()
{

};
