#pragma once
#include "stdafx.h"
#include <string>

class ResourceManagerBase;

class ResourceBase
{
			std::string					Tag;
			bool						resourceValidity;

	protected:

			virtual void				Dispose() = 0;
			inline void					SetValid(const bool&);

	public:

			friend class				ResourceManagerBase;		
										ResourceBase(const std::string &resourceTag) : Tag(resourceTag) {};
			virtual						~ResourceBase() {};
			inline const std::string	GetTag() const; 
			inline bool					GetValid();
};


inline const std::string ResourceBase::GetTag() const
{
	return Tag;
};

inline void ResourceBase::SetValid(const bool& value)
{
	resourceValidity = value;
};

inline bool ResourceBase::GetValid()
{
	return resourceValidity;
};