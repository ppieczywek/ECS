#pragma once
#include "stdafx.h"
#include <cassert>
#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include "Resource.h"

class ResourceManagerBase
{
		std::vector<std::pair<std::string, std::shared_ptr<ResourceBase>>> ResourceContainer;

public:

									ResourceManagerBase();
		bool						AddResource(std::shared_ptr<ResourceBase>);
		bool						DisposeResource(const std::string&);
		template<class T> T* const  GetResource(const std::string&);
		void						Close();
};

template<class T> T* const  ResourceManagerBase::GetResource(const std::string& Tag)
{
	for(unsigned ii = 0; ii < ResourceContainer.size(); ++ii)
	{
		if( Tag == ResourceContainer[ii].first )
		{
			if(ResourceContainer[ii].second != nullptr)
			{
				if(ResourceContainer[ii].second->GetValid())
				{
					T* pt = dynamic_cast<T*>(ResourceContainer[ii].second.get()); 
					//assert(pt);
					return pt;
				}
				else
				{
					std::cout << "Resource Manager:	invalid resource object." << std::endl;
					return nullptr;
				}
			}
			else
			{
				std::cout << "Resource Manager:	invalid resource object." << std::endl;
				return nullptr;
			}
		}
	}
	return nullptr;
};

