#pragma once
#include "stdafx.h"
#include <iostream>
#include "ModelData.h"


ModelData::ModelData(const std::string& Tag, const Vertex* VData,const unsigned int& VDataSize) : ResourceBase(Tag)
{
	VAO = 0;
	VertexData = 0;
	VertexNumber = 0;
	if( VData != nullptr && VDataSize > 0 )
	{
		Create(VData,VDataSize);
		SetValid(true);
	}
	else
	{
		SetValid(false);
	}
};


ModelData::~ModelData()
{
	//Clear();
};


const bool ModelData::Create(const Vertex* VData,const unsigned int& VDataSize)
{		
	VertexNumber = VDataSize;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); 

	glGenBuffers(1, &VertexData); 
	glBindBuffer(GL_ARRAY_BUFFER, VertexData); 
	
	glBufferData(GL_ARRAY_BUFFER, VDataSize * sizeof(Vertex), VData, GL_STATIC_DRAW); 
			
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position) ); 
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord) ); 
		
	glBindVertexArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
	
	return true;
};


bool ModelData::Clear()
{
	if ( VertexData != 0 && VAO != 0 )
	{
		std::cout << "Clearing model data - " << GetTag() << std::endl;
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glDeleteBuffers(1, &VertexData);

		glBindVertexArray (0);
		glDeleteVertexArrays(1, &VAO);
		return true;
	}
	else
	{
		std::cout << "Model data already cleared" << std::endl;
		return false;
	}
};


void ModelData::Dispose()
{
	Clear();
};