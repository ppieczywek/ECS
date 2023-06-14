#pragma once
#include "WorldManagerBase.h"

class WorldStateBase
{
			unsigned				StateId;
	public:
			inline	void			SetStateId(const unsigned&);
			inline  const unsigned 	GetStateId() const;
			virtual void			Initialize(WorldManagerBase &Engine)	= 0;
			virtual void			Cleanup(WorldManagerBase &Engine)		= 0;
			virtual void			Pause()									= 0;
			virtual void			Resume()								= 0;
			virtual void			Update(WorldManagerBase &Engine)		= 0;
};


inline	void WorldStateBase::SetStateId(const unsigned& Value)
{
	StateId = Value;
};


inline const unsigned	WorldStateBase::GetStateId() const
{
	return StateId;
};