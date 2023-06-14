#pragma once
#include "stdafx.h"
#include "BodyComponent.h"


BodyComponent::BodyComponent() 
{	
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	PolygonNumber	= 0;
	SensorNumber	= 0;
};


BodyComponent::BodyComponent( const BodyComponent& Copy) : ComponentBase(Copy)
{
	PolygonNumber	= Copy.PolygonNumber;
	SensorNumber	= Copy.SensorNumber;
	std::copy(Copy.PolygonData.begin(), Copy.PolygonData.begin() + PolygonNumber, PolygonData.begin()); 
	std::copy(Copy.SensorData.begin(), Copy.SensorData.begin() + SensorNumber, SensorData.begin()); 
};


BodyComponent::~BodyComponent()
{

};


BodyComponent& BodyComponent::operator=(const BodyComponent& Source)
{
	//ComponentBase::operator = (Source);
	PolygonNumber	= Source.PolygonNumber;
	SensorNumber	= Source.SensorNumber;
	std::copy(Source.PolygonData.begin(), Source.PolygonData.begin() + PolygonNumber, PolygonData.begin()); 
	std::copy(Source.SensorData.begin(), Source.SensorData.begin() + SensorNumber, SensorData.begin()); 
	return *this;
};


void BodyComponent::Reset()
{
	TypeMask = 0;
	Index = -1;
	OwnerIndex = -1;
	PolygonNumber	= 0;
	SensorNumber	= 0;
};


const Rectangle2& BodyComponent::UpdateBoundingBox()
{
	if(PolygonNumber > 0)
	{
		BoundingBox.min.x = BoundingBox.max.x = PolygonData[0].GlobalVertex[0].x;
		BoundingBox.min.y = BoundingBox.max.y = PolygonData[0].GlobalVertex[0].y;
		for(unsigned ii = 0; ii < PolygonNumber; ++ii)
		{
			for(unsigned jj = 0; jj < PolygonData[ii].VertexNumber; ++jj)
			{
				BoundingBox.max.x = BoundingBox.max.x < PolygonData[ii].GlobalVertex[jj].x ? PolygonData[ii].GlobalVertex[jj].x : BoundingBox.max.x;
				BoundingBox.min.x = BoundingBox.min.x > PolygonData[ii].GlobalVertex[jj].x ? PolygonData[ii].GlobalVertex[jj].x : BoundingBox.min.x;
				BoundingBox.max.y = BoundingBox.max.y < PolygonData[ii].GlobalVertex[jj].y ? PolygonData[ii].GlobalVertex[jj].y : BoundingBox.max.y;
				BoundingBox.min.y = BoundingBox.min.y > PolygonData[ii].GlobalVertex[jj].y ? PolygonData[ii].GlobalVertex[jj].y : BoundingBox.min.y;
			}
		}
	}
	
	for(unsigned ii = 0; ii < SensorNumber; ++ii)
	{
		for(unsigned jj = 0; jj < SensorData[ii].Body.VertexNumber; ++jj)
		{
			BoundingBox.max.x = BoundingBox.max.x < SensorData[ii].Body.GlobalVertex[jj].x ? SensorData[ii].Body.GlobalVertex[jj].x : BoundingBox.max.x;
			BoundingBox.min.x = BoundingBox.min.x > SensorData[ii].Body.GlobalVertex[jj].x ? SensorData[ii].Body.GlobalVertex[jj].x : BoundingBox.min.x;
			BoundingBox.max.y = BoundingBox.max.y < SensorData[ii].Body.GlobalVertex[jj].y ? SensorData[ii].Body.GlobalVertex[jj].y : BoundingBox.max.y;
			BoundingBox.min.y = BoundingBox.min.y > SensorData[ii].Body.GlobalVertex[jj].y ? SensorData[ii].Body.GlobalVertex[jj].y : BoundingBox.min.y;
		}
	}

	BoundingBox.max.x += 0.22f;
	BoundingBox.max.y += 0.22f;
	BoundingBox.min.x -= 0.22f;
	BoundingBox.min.y -= 0.22f;

	return BoundingBox;
};


void BodyComponent::SetTransform(const Vector2& Vector, const float& Angle)
{	
	BodyTransform.Set(Vector, Angle);
	NormalsTransform.Set(ZeroVector, Angle);
	
	for(unsigned ii = 0; ii < PolygonNumber; ++ii)
	{
		for(unsigned jj = 0; jj < PolygonData[ii].VertexNumber; ++jj)
		{
			BodyTransform.Transform(PolygonData[ii].LocalVertex[jj], PolygonData[ii].GlobalVertex[jj]);
			NormalsTransform.Transform(PolygonData[ii].LocalNormals[jj], PolygonData[ii].GlobalNormals[jj]);
		}
		BodyTransform.Transform(PolygonData[ii].LocalCentroid, PolygonData[ii].GlobalCentroid );
	}

	for(unsigned ii = 0; ii < SensorNumber; ++ii)
	{
		for(unsigned jj = 0; jj < SensorData[ii].Body.VertexNumber; ++jj)
		{
			BodyTransform.Transform(SensorData[ii].Body.LocalVertex[jj], SensorData[ii].Body.GlobalVertex[jj]);
			NormalsTransform.Transform(SensorData[ii].Body.LocalNormals[jj], SensorData[ii].Body.GlobalNormals[jj]);
		}
		BodyTransform.Transform(SensorData[ii].Body.LocalCentroid, SensorData[ii].Body.GlobalCentroid );
	}
};