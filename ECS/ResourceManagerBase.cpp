#pragma once
#include "stdafx.h"
#include "ResourceManagerBase.h"

ResourceManagerBase::ResourceManagerBase()
{

};

bool ResourceManagerBase::AddResource(std::shared_ptr<ResourceBase> resource)
{
	if(resource != nullptr)
	{
		std::string Tag = resource->GetTag();

		for(unsigned ii = 0; ii < ResourceContainer.size(); ++ii)
		{
			if( Tag == ResourceContainer[ii].first )
			{
				std::cout << "Resource Manager:	error - duplicated tag -  " << Tag << std::endl;
				return false;
			}
		}

		if(resource->GetValid())
		{
			ResourceContainer.push_back( std::make_pair(Tag, resource) );
			std::cout << "Resource Manager:	resource " << Tag << " added." << std::endl;
			return true;
		}
		else
		{
			std::cout << "Resource Manager:	invalid resource object." << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "Resource Manager:	invalid pointer to resource object." << std::endl;
		return false;
	}
};

bool ResourceManagerBase::DisposeResource(const std::string& Tag)
{
	for(unsigned ii = 0; ii < ResourceContainer.size(); ++ii)
	{
		if( Tag == ResourceContainer[ii].first )
		{
			if(ResourceContainer[ii].second != nullptr)
			{
				ResourceContainer[ii].second->Dispose();
				//delete ResourceContainer[ii].second;
				ResourceContainer.erase(ResourceContainer.begin() + ii);
				std::cout << "Resource Manager:	resource " << Tag << " removed." << std::endl;
				return true;
			}
			else
			{
				std::cout << "Resource Manager:	invalid pointer to resource object." << std::endl;
				return false;
			}
		}
	}
	return false;
};


void ResourceManagerBase::Close()
{
	std::string Tag;

	while(!ResourceContainer.empty())
	{		
		auto LastItem = ResourceContainer.back();
		Tag = LastItem.first;
		if(LastItem.second != nullptr)
		{
			LastItem.second->Dispose();
			//delete LastItem.second;
		}
		ResourceContainer.pop_back();
		std::cout << "Resource Manager:	resource " << Tag << " removed." << std::endl;
	}
	std::cout << "Resource Manager:	Closed!" << std::endl;
};
