#pragma once
#include <type_traits>
#include <algorithm>
#include <functional>
#include "ComponentContainerBase.h"

const unsigned DefaultContainerSize = 200;
const unsigned DefaultContainerPool = 500;
//sort freq
template<class T> class ComponentContainer final: public ComponentContainerBase
{	

			std::vector<int>					Dictionary;
			std::vector<T>						Container;
			std::vector<int>					Deallocated;
			int									DictionarySize;
			
			void								ExpandContainer();
			T* const							Connect(const Entity*)  override;
			bool								Disconnect(const Entity*)  override;
			inline bool							Copy(const Entity*, const Entity*)  override;
			void								Clear()  override;

	public:
						
												ComponentContainer();
			inline T* const						Get(const Entity*)  override;			
			inline T* const						operator [](int i) override;
};
			

template<class T> ComponentContainer<T>::ComponentContainer()
{
	static_assert(std::is_base_of<ComponentBase, T>::value == true, "Template class does not derive from ComponentBase.  Cannot proceed!");		
	
	DictionarySize = DefaultContainerSize;
	Dictionary.resize(DefaultContainerSize, -1);
	Container.resize(DefaultContainerSize);

	for (int index = DefaultContainerSize-1; index >= 0; --index)
	{
		Deallocated.push_back(index);
	}

};

template<class T> void ComponentContainer<T>::ExpandContainer()
{
	if(Deallocated.empty())
	{
		int CurrentSize = static_cast<int>(Container.size());
		int NewSize;
		if (CurrentSize == 0)
		{
			NewSize = DefaultContainerSize;
		}
		else
		{
			NewSize = 2 * CurrentSize;
		}
		
		Container.resize(NewSize);
		for(int index = NewSize - 1; index >= CurrentSize; --index)
		{
			Deallocated.push_back(index);
		}

	}
};


template<class T> T* const ComponentContainer<T>::Connect(const Entity* Entity)
{
	if(Entity != nullptr)
	{
		int EntityIndex = Entity->GetIndex();

		if(DictionarySize <= EntityIndex )
		{
			Dictionary.resize( 2 * EntityIndex, -1 );
			DictionarySize = static_cast<int>(Dictionary.size());
		}

		if( Dictionary[EntityIndex] == -1 )
		{			
			ExpandContainer();
			
			auto ComponentIndex = Deallocated.back();
			Deallocated.pop_back();
			Dictionary[EntityIndex] = ComponentIndex;
			
			return &Container[ComponentIndex];
		}
		else
		{
			return nullptr;
		}
	
	}
	return nullptr;
};

template<class T> bool  ComponentContainer<T>::Disconnect(const Entity* Entity)
{
	if(Entity != nullptr)
	{
		int EntityIndex = Entity->GetIndex();
		
		if(DictionarySize <= EntityIndex ) return false;
		
		if( Dictionary[EntityIndex] != -1)
		{			
			int ComponentIndex = Dictionary[EntityIndex];
			Dictionary[EntityIndex] = -1;
			Deallocated.push_back(ComponentIndex);
			std::sort(Deallocated.begin(), Deallocated.end(), std::greater<int>());
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
};


template<class T> inline T* const ComponentContainer<T>::Get(const Entity* Entity)
{
	if(Entity != nullptr)
	{
		int EntityIndex = Entity->GetIndex();
		if(DictionarySize <= EntityIndex ) return nullptr;
		
		if( Dictionary[EntityIndex] != -1 )
		{
			return &Container[Dictionary[EntityIndex]];
		}
		else
		{
			return nullptr;
		}
	}
	return nullptr;
};

template<class T> inline bool ComponentContainer<T>::Copy(const Entity* src, const Entity* dst)
{
	if( src != nullptr && dst != nullptr)
	{
		if(DictionarySize <= src->GetIndex() ) return false;
		if(DictionarySize <= dst->GetIndex() ) return false;

		if( Dictionary[src->GetIndex()] != -1 &&  Dictionary[dst->GetIndex()] != -1 )
		{
			Container[ Dictionary[dst->GetIndex()] ] = Container[ Dictionary[src->GetIndex()] ]; 
		}
	}
	return false;
};

template<class T> inline T* const ComponentContainer<T>::operator [](int i)
{
	//return &Container[Dictionary[i]];
	if(DictionarySize <= i ) return nullptr;
	if( Dictionary[i] != -1)
	{
		return &Container[Dictionary[i]];
	}
	else
	{
		return nullptr;
	}
};


template<class T> void ComponentContainer<T>::Clear()
{

};