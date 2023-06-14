#pragma once
#include <array>
#include <vector>
#include "Polygon2.h"
#include "Sensor.h"
#include "Rectangle2.h"
#include "TransformationMatrix.h"
#include "ComponentBase.h"


const unsigned MAX_POLYGON_NUMBER = 20;

class BodyComponent : public ComponentBase
{	
		unsigned									PolygonNumber;
		unsigned									SensorNumber;
		std::array<Polygon2, MAX_POLYGON_NUMBER>	PolygonData;
		std::array<Sensor, MAX_POLYGON_NUMBER>		SensorData;
		Rectangle2									BoundingBox;
		TransformationMatrix						BodyTransform;
		TransformationMatrix						NormalsTransform;

public:	
													BodyComponent();
													BodyComponent(const BodyComponent&);
													~BodyComponent();
		virtual void								Reset();
		
		BodyComponent&								operator=(const BodyComponent&);
	
		inline const bool							AddPolygon(const Polygon2&);
		inline const bool							RemovePolygon(const unsigned&);
		inline Polygon2* const						GetPolygon(const unsigned&);
		inline const unsigned&						GetPolygonNumber() const;

		inline const bool							AddSensor(const Polygon2&, const long long&);
		inline const bool							RemoveSensor(const unsigned&);
		inline Sensor* const						GetSensor(const unsigned&);
		inline const unsigned&						GetSensorNumber() const;
		
		inline const Rectangle2&					GetBoundingBox() const;
		const Rectangle2&							UpdateBoundingBox();
		void										SetTransform(const Vector2&, const float&);
};


const Rectangle2& BodyComponent::GetBoundingBox() const
{
	return BoundingBox;
};


const bool BodyComponent::AddPolygon(const Polygon2& Polygon)
{
	if(PolygonNumber < MAX_POLYGON_NUMBER)
	{
		PolygonData[PolygonNumber++] = Polygon;
		return true;
	}
	return false;
};


const bool BodyComponent::RemovePolygon(const unsigned& Index)
{
	if(Index < PolygonNumber)
	{	
		PolygonData[Index] = PolygonData[PolygonNumber-1];
		PolygonNumber--;
		return true;
	}
	return false;
};


Polygon2* const BodyComponent::GetPolygon(const unsigned& Index)
{
	if(Index < PolygonNumber)
	{
		return &PolygonData[Index];
	}
	return nullptr;
};


const unsigned& BodyComponent::GetPolygonNumber() const
{
	return PolygonNumber;
};



const bool BodyComponent::AddSensor(const Polygon2& Polygon, const long long& Id)
{
	if(SensorNumber < MAX_POLYGON_NUMBER)
	{
		SensorData[SensorNumber++] = std::move(Sensor(Polygon, Id));
		return true;
	}
	return false;
};


const bool BodyComponent::RemoveSensor(const unsigned& Index)
{
	if(Index < SensorNumber)
	{	
		SensorData[Index] = SensorData[SensorNumber-1];
		SensorNumber--;
		return true;
	}
	return false;
};


Sensor* const BodyComponent::GetSensor(const unsigned& Index)
{
	if(Index < SensorNumber)
	{
		return &SensorData[Index];
	}
	return nullptr;
};


const unsigned& BodyComponent::GetSensorNumber() const
{
	return SensorNumber;
};
