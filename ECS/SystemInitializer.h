#pragma once
#include "TypeList.h"
#include "SystemBase.h"


template< int i, class TList>
class SystemInitializer
{
  public:
    static inline void Run(std::shared_ptr<SystemBase>* Array)
	{
		Array[i-1] = std::make_shared< TypeAt<TList, i-1>::Result>();
		SystemInitializer<i-1, TList>::Run(Array);
    }
};


template <class TList>
class SystemInitializer< 0,  TList>
{
  public:
    static inline void Run(std::shared_ptr<SystemBase>* Array)
	{
		//Array[0] = std::make_shared< TypeAt<TList, 0>::Result>();
    }
};




