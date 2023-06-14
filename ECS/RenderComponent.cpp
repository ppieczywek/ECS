#pragma once
#include "stdafx.h"
#include "RenderComponent.h"

RenderComponent::RenderComponent() 
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;

	VisibleFlag = 0;
	TextureFlag = 0;
	Layer =  0.0f;
	VertexColor = glm::vec4(1.0f);
	Scale = glm::vec3(1.0f);

	ColorMixingCoeff = 1.0f;
	TexCoordScale = glm::vec2(1.0f);
	TexCoordOffset = glm::vec2(0.0f);
	ModelCoordinatesMode = World;
	VAO = 0;
	VertexNumber = 0;
	TextureId = 0;

	TextSize = 1.0f;
	TextColor = glm::vec4(1.0f);
	TextOffset = ZeroVector;
	TextAngle = 0.0f;
};


RenderComponent::~RenderComponent()
{

};


void RenderComponent::Reset()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	
	VisibleFlag = 0;
	TextureFlag = 0;
	Layer =  0.0f;
	VertexColor = glm::vec4(1.0f);
	Scale = glm::vec3(1.0f);

	ColorMixingCoeff = 1.0f;
	TexCoordScale = glm::vec2(1.0f);
	TexCoordOffset = glm::vec2(0.0f);
	ModelCoordinatesMode = World;
	VAO = 0;
	VertexNumber = 0;
	TextureId = 0;

	TextSize = 1.0f;
	TextColor = glm::vec4(1.0f);
	TextOffset = ZeroVector;
	TextAngle = 0.0f;
};


void RenderComponent::SetModelData(const ModelData* Data)
{
	if(Data != nullptr)
	{
		VAO = Data->GetVAO();
		VertexNumber = Data->GetVertexNumber();
	}
};