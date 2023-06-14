#pragma once
#include "stdafx.h"
#include <cstring>
#include "Polygon2.h"	


Polygon2::Polygon2()
{
	ValidEdgeNumber = 0;
	VertexNumber	= 0;
};


Polygon2::Polygon2(const Polygon2& Copy)
{
	*this = Copy;
};
	

Polygon2& Polygon2::operator=(const Polygon2 &Source)
{	
	ValidEdgeNumber = Source.ValidEdgeNumber;
	LocalCentroid	= Source.LocalCentroid;
	VertexNumber	= Source.VertexNumber;
	
	memcpy(ValidEdgeIndex, Source.ValidEdgeIndex, ValidEdgeNumber*sizeof(unsigned));
	memcpy(LocalNormals, Source.LocalNormals, VertexNumber*sizeof(Vector2));
	memcpy(LocalVertex, Source.LocalVertex, VertexNumber*sizeof(Vector2));
	memcpy(LocalEdge, Source.LocalEdge, VertexNumber*sizeof(Vector2));
	memcpy(EdgeFlag, Source.EdgeFlag, VertexNumber*sizeof(bool));
	return *this;
};



const Polygon2	CreatePolygon( const Vector2* VertexList, const int& ListSize)
{
	Polygon2 Polygon;
	
	if(ListSize != 0 && ListSize <= MaxPolygonVertex)
	{
		Polygon.VertexNumber = ListSize;
		
		for(unsigned jj = 0; jj < Polygon.VertexNumber; ++jj)
		{
			Polygon.GlobalVertex[jj]	=  Polygon.LocalVertex[jj] = VertexList[jj];
			Polygon.LocalCentroid		+= VertexList[jj];
			Polygon.EdgeFlag[jj]		=  true;	
		}
	 	
		Polygon.LocalCentroid /=  static_cast<float>(Polygon.VertexNumber);
		Polygon.GlobalCentroid =  Polygon.LocalCentroid;


		for(unsigned jj = 0; jj < Polygon.VertexNumber-1; ++jj)
		{
			Polygon.LocalEdge[jj]		=  VertexList[jj+1] - VertexList[jj];
			CCW.Transform(Polygon.LocalEdge[jj].unit(), Polygon.LocalNormals[jj]);
			Polygon.GlobalNormals[jj] = Polygon.LocalNormals[jj];
		}
		auto Last = Polygon.VertexNumber-1;
		Polygon.LocalEdge[Last] = VertexList[0] - VertexList[Last];
		CCW.Transform(Polygon.LocalEdge[Last].unit(), Polygon.LocalNormals[Last]);
		Polygon.GlobalNormals[Last] = Polygon.LocalNormals[Last];
				

		 Polygon.ValidEdgeNumber = 0;
		for(unsigned ii=0; ii < Polygon.VertexNumber-1; ++ii)
		{
			for(unsigned jj=ii+1; jj < Polygon.VertexNumber; ++jj)
			{
				if ((1 - abs(Polygon.LocalNormals[ii] * Polygon.LocalNormals[jj])) < 0.01f)
				{
					Polygon.EdgeFlag[jj] = false; 
				}
			}
		}

		for(unsigned ii=0; ii < Polygon.VertexNumber;++ii)
		{
			if(Polygon.EdgeFlag[ii])
			{
				Polygon.ValidEdgeIndex[Polygon.ValidEdgeNumber] = ii;
				Polygon.ValidEdgeNumber++;
			}
		}
	}
	
	return Polygon;
};


const bool	CalculatePolygonProperties(const Polygon2& Polygon, float& TotalArea, float& Inertia)
{
	if(Polygon.VertexNumber > 0)
	{
		TotalArea = 0.0f;
		Vector2 v1,v2;
		float vp;
		float sum1 = 0.0f;
		float sum2 = 0.0f;
		for (unsigned ii = 1; ii< Polygon.VertexNumber; ++ii)
		{					
			v1 = Polygon.LocalVertex[ii-1] - Polygon.LocalCentroid;
			v2 = Polygon.LocalVertex[ii] - Polygon.LocalCentroid;
			vp = abs( (v1.x*v2.y - v1.y*v2.x) );

			TotalArea +=  0.5f * vp;

			sum1 += vp * ( v2*v2 + v2*v1  + v1*v1);
			sum2 += vp;
		}
		v1 = Polygon.LocalVertex[Polygon.VertexNumber-1] - Polygon.LocalCentroid;
		v1 = Polygon.LocalVertex[0] - Polygon.LocalCentroid;
		vp = abs( (v1.x*v2.y - v1.y*v2.x) );
		TotalArea +=  0.5f * vp;

		sum1 += vp * ( v2*v2 + v2*v1  + v1*v1);
		sum2 += vp;

		Inertia = sum1 / (6*sum2);
		
		return true;
	}
	return false;	
};


const bool	ScalePolygon(Polygon2& Out, const float& x, const float& y)
{
	if(Out.VertexNumber > 0)
	{
		for (unsigned ii = 1; ii< Out.VertexNumber; ++ii)
		{	
			Out.LocalVertex[ii].x = (Out.LocalVertex[ii].x - Out.LocalCentroid.x) * x + Out.LocalCentroid.x;
			Out.LocalVertex[ii].y = (Out.LocalVertex[ii].y - Out.LocalCentroid.y) * y + Out.LocalCentroid.y;
			Out.LocalEdge[ii].x *= x;
			Out.LocalEdge[ii].y *= y;
		}
		return true;
	}
	return false;
};


const bool	TransformPolygon(Polygon2& Out, const Vector2& vector, const float& angle)
{
	if(Out.VertexNumber > 0)
	{
		Vector2 Copy;
		TransformationMatrix TM(vector, angle);
		for (unsigned ii = 1; ii< Out.VertexNumber; ++ii)
		{	
			Copy = Out.LocalVertex[ii];
			TM.Transform( Copy, Out.LocalVertex[ii]);
			
			Copy = Out.LocalEdge[ii];
			TM.Transform( Copy, Out.LocalEdge[ii]);
		}
		Copy = Out.LocalCentroid;
		TM.Transform( Copy, Out.LocalCentroid);
		return true;
	}
	return false;
};