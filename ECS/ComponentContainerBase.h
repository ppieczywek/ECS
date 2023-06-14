#pragma once
#include "ComponentBase.h"
#include "Entity.h"

class ComponentContainerBase
{
	protected:
			virtual ComponentBase* const				Connect(const Entity*)				= 0;
			virtual bool								Disconnect(const Entity*)			= 0;
			virtual inline bool							Copy(const Entity*, const Entity*)	= 0;
			virtual void								Clear()								= 0;
	public:
					
			friend class								EntityManagerBase;

			virtual inline ComponentBase* const			Get(const Entity*) 			= 0;			
			virtual inline ComponentBase* const			operator [](int i)			= 0;
};