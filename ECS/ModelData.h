#pragma once
#include <string>
#include "Vertex.h"
#include "Resource.h"

class ModelData : public ResourceBase
{
		unsigned int				VAO;
		unsigned int				VertexData;
		unsigned int				VertexNumber;
		bool						Clear();
		const bool					Create(const Vertex*,const unsigned int&);

protected:

		void						Dispose();

public:

									ModelData(const std::string&, const Vertex*, const unsigned int&);
									~ModelData();
											
		inline const unsigned int&	GetVAO() const;
		inline const unsigned int&	GetVertexNumber() const;
};


inline const unsigned int&	ModelData::GetVAO() const
{
	return VAO;
};


inline const unsigned int&	ModelData::GetVertexNumber() const
{
	return VertexNumber;
};

