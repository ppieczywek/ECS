#pragma once
#include <memory>
#include "TypeList.h"
#include "ComponentContainer.h"


template< int i, class TList>
class ComponentArrayInitializer
{
  public:
    static inline void Run(std::shared_ptr<ComponentContainerBase>* Array)
	{
		Array[i-1] = std::make_shared<ComponentContainer< TypeAt<TList, i-1 >::Result > >();
		ComponentArrayInitializer< i-1, TList>::Run(Array);
    }
};


template <class TList>
class ComponentArrayInitializer< 0,  TList>
{
  public:
    static inline void Run(std::shared_ptr<ComponentContainerBase>* Array)
	{
		
    }
};






