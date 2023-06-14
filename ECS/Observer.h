#pragma once
#include "Entity.h"


class Observer
{
public:
		virtual void OnCreateEntity(const Entity*)	= 0;
		virtual void OnDestroyEntity(const Entity*)	= 0;
};