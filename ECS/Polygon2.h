#pragma once
#include "stdafx.h"
#include "Vector2.h"
#include "TransformationMatrix.h"

const int MaxPolygonVertex = 8;

class Polygon2
{
public:
					Polygon2();
					Polygon2(const Polygon2&);
	
	Polygon2&		operator=(const Polygon2 &Source);
						
	unsigned		VertexNumber;
	unsigned		ValidEdgeNumber;
	unsigned		ValidEdgeIndex[MaxPolygonVertex];

	Vector2			LocalCentroid;
	Vector2			LocalVertex[MaxPolygonVertex];
	Vector2			LocalNormals[MaxPolygonVertex];
	Vector2			LocalEdge[MaxPolygonVertex];
	

	Vector2			GlobalCentroid;
	Vector2			GlobalVertex[MaxPolygonVertex];
	Vector2			GlobalNormals[MaxPolygonVertex];
	Vector2			Intervals[MaxPolygonVertex];
		
	bool			EdgeFlag[MaxPolygonVertex];
};

const Polygon2		CreatePolygon(const Vector2*, const int&);
const bool			CalculatePolygonProperties(const Polygon2&, float&, float&);

const bool			ScalePolygon(Polygon2&, const float&, const float&);
const bool			TransformPolygon(Polygon2&, const Vector2&, const float&);
