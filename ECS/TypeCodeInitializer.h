#pragma once
#include <typeindex>
#include "TypeList.h"


template< int i, class TList>
class TypeCodeInitializer
{
  public:
    static inline void Run(std::type_index* Array)
	{
		//Array[i-1]= (  typeid( TypeAt<TList, i-1>::Result ).hash_code() );
		Array[i-1]= std::type_index( typeid( TypeAt<TList, i-1>::Result ) );
		TypeCodeInitializer< i-1, TList>::Run(Array);
    }
};


template <class TList>
class TypeCodeInitializer< 0,  TList>
{
  public:
    static inline void Run(std::type_index* Array)
	{
	
    }
};
