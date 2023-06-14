#pragma once
#include "stdafx.h"
#include "DynamicGrid.h"
#include <algorithm> 

DynamicGrid::DynamicGrid()
{

};


bool DynamicGrid::Initialize()
{
	GridVerticalCount	 = DefaultGridVerticalCount;
	GridHorizontalCount	 = DefaultGridHorizontalCount;
	UpdateGrid();
	return true;
};


void DynamicGrid::InsertElement(const int& row, const int& col, const int& value)
{
	int Index = GridHorizontalCount * row + col;
	if(BucketsElements[Index] >= BucketsSize[Index])
	{
		Buckets[Index].resize(2*BucketsSize[Index]);
		BucketsSize[Index]*=2;
	}
	Buckets[Index][ BucketsElements[Index]++ ] = value;  
};


bool DynamicGrid::BoxIntersection(const Rectangle2& A, const Rectangle2& B) const
{
    if (A.max.x < B.min.x) return false;
    if (A.min.x > B.max.x) return false;
    if (A.max.y < B.min.y) return false;
    if (A.min.y > B.max.y) return false;
    return true;
};


void  DynamicGrid::Update(ComponentContainer<BodyComponent>& BodyComponent, ComponentContainer<CollisionComponent>& CollisionComponent, const std::set<int>& Indicies, std::set<std::pair<int,int>>& Output)
{	
	GridWidth = GridBox.GetWidth();
	GridHeight = GridBox.GetHeight();

	if(Indicies.size() > 1 && GridHeight > 0.0f && GridWidth > 0.0f)
	{
		CellWidth  = 1.0f / (GridWidth / GridHorizontalCount);
		CellHeight = 1.0f / (GridHeight / GridVerticalCount);
		BucketsElements.assign(GridHorizontalCount * GridVerticalCount, 0);

		for each(auto Index in Indicies)
		{
			if(CollisionComponent[Index]->IsCollidable())
			{
				auto BodyBox = BodyComponent[Index]->GetBoundingBox();
				if( BoxIntersection(BodyBox, GridBox) )
				{
					ColMin = static_cast<int>(floor(BodyBox.min.x * CellWidth));
					ColMax = static_cast<int>(floor(BodyBox.max.x * CellWidth)) + 1;
					RowMin = static_cast<int>(floor(BodyBox.min.y * CellHeight));
					RowMax = static_cast<int>(floor(BodyBox.max.y * CellHeight)) + 1;
	
					if(ColMax >= 0 && RowMax >= 0)
					{
						RowMin = RowMin >= 0 ? RowMin : 0;
						ColMin = ColMin >= 0 ? ColMin : 0;

						RowMax = RowMax >= GridHorizontalCount ? GridHorizontalCount : RowMax;
						ColMax = ColMax >= GridVerticalCount ? GridVerticalCount : ColMax;

						for(int ii = RowMin; ii < RowMax; ++ii)
						{
							for(int jj = ColMin; jj < ColMax; ++jj)
							{
								InsertElement(ii,jj,Index);
							}
						}
					}
				}
			}
		}
	
		int	Index1;
		int	Index2;
		Output.clear();
		
		for(int ii=0; ii < static_cast<int>(Buckets.size()); ++ii)
		{
			for(int kk=0; kk < (BucketsElements[ii]-1); ++kk)
			{
				Index1 = Buckets[ii][kk];
				for(int jj=kk+1; jj < BucketsElements[ii]; ++jj)
				{
					Index2 = Buckets[ii][jj];
					if(  (CollisionComponent[Index1]->GetCategoryMask() & CollisionComponent[Index2]->GetCollisionMask()) != 0 &&
						 (CollisionComponent[Index2]->GetCategoryMask() & CollisionComponent[Index1]->GetCollisionMask()) != 0 )
					{
						if (BoxIntersection( BodyComponent[Index1]->GetBoundingBox(), BodyComponent[Index2]->GetBoundingBox() )) 
						{
							Output.insert(std::make_pair(Index1, Index2));
						}
					}
				}
			}
		}
	}
};


void DynamicGrid::SetGridBox(const Vector2& Position, const Vector2& Size)
{
	GridBox.min.x = Position.x;
	GridBox.min.y = Position.y;
	GridBox.max.x = Position.x + Size.x;
	GridBox.max.y = Position.y + Size.y;
};


void DynamicGrid::SetGridBox(const Rectangle2& Value)
{
	GridBox = Value;
};


const Rectangle2& DynamicGrid::GetGridBox() const
{
	return GridBox;
};


void DynamicGrid::Update(ComponentContainer<BodyComponent>& BodyComponent, ComponentContainer<CollisionComponent>& CollisionComponent, const std::vector<int>& Indicies, std::set<std::pair<int,int>>& Output)
{	
	GridWidth = GridBox.GetWidth();
	GridHeight = GridBox.GetHeight();

	if(Indicies.size() > 1 && GridHeight > 0.0f && GridWidth > 0.0f)
	{
		CellWidth  = 1.0f / (GridWidth / GridHorizontalCount);
		CellHeight = 1.0f / (GridHeight / GridVerticalCount);
		BucketsElements.assign(GridHorizontalCount * GridVerticalCount, 0);

		for each(auto Index in Indicies)
		{
			if(CollisionComponent[Index]->IsCollidable())
			{
				auto BodyBox = BodyComponent[Index]->GetBoundingBox();
				if( BoxIntersection(BodyBox, GridBox) )
				{
					ColMin = static_cast<int>(floor(BodyBox.min.x * CellWidth));
					ColMax = static_cast<int>(floor(BodyBox.max.x * CellWidth)) + 1;
					RowMin = static_cast<int>(floor(BodyBox.min.y * CellHeight));
					RowMax = static_cast<int>(floor(BodyBox.max.y * CellHeight)) + 1;
	
					if(ColMax >= 0 && RowMax >= 0)
					{
						RowMin = RowMin >= 0 ? RowMin : 0;
						ColMin = ColMin >= 0 ? ColMin : 0;

						RowMax = RowMax >= GridHorizontalCount ? GridHorizontalCount : RowMax;
						ColMax = ColMax >= GridVerticalCount ? GridVerticalCount : ColMax;

						for(int ii = RowMin; ii < RowMax; ++ii)
						{
							for(int jj = ColMin; jj < ColMax; ++jj)
							{
								InsertElement(ii,jj,Index);
							}
						}
					}
				}
			}
		}
	
		int	Index1;
		int	Index2;
		Output.clear();
		
		for(int ii=0; ii < static_cast<int>(Buckets.size()); ++ii)
		{
			for(int kk=0; kk < (BucketsElements[ii]-1); ++kk)
			{
				Index1 = Buckets[ii][kk];
				for(int jj=kk+1; jj < BucketsElements[ii]; ++jj)
				{
					Index2 = Buckets[ii][jj];
					if (BoxIntersection( BodyComponent[Index1]->GetBoundingBox(), BodyComponent[Index2]->GetBoundingBox() )) 
					{
						if(Index1 < Index2)
						{
							Output.insert(std::make_pair(Index1, Index2));
						}
						else
						{
							Output.insert(std::make_pair(Index2, Index1));
						}
					}
				}
			}
		}
	}
};


void DynamicGrid::SetGridVerticalCount(const int& Value)
{
	if(Value > 0)
	{
		GridVerticalCount	= Value;
		UpdateGrid();
	}
};


void DynamicGrid::SetGridHorizontalCount(const int& Value)
{
	if(Value > 0)
	{
		GridHorizontalCount = Value;
		UpdateGrid();
	}
};


void DynamicGrid::UpdateGrid()
{
	int GridSize = GridVerticalCount * GridHorizontalCount;

	Buckets.resize(GridSize);
	for(int ii=0; ii < static_cast<int>(Buckets.size()); ++ii)
	{
		Buckets[ii].resize(DefaultBucketSize);
	}
	
	BucketsSize.resize(GridSize, DefaultBucketSize);
	BucketsElements.resize(GridSize, 0);
};


void DynamicGrid::Update(ComponentContainer<BodyComponent>& BodyComponent, ComponentContainer<CollisionComponent>& CollisionComponent, const std::set<int>& Indicies, std::vector<CollisionPair>& Output)
{	
	GridWidth = GridBox.GetWidth();
	GridHeight = GridBox.GetHeight();
	
	Output.clear();

	if(Indicies.size() > 1 && GridHeight > 0.0f && GridWidth > 0.0f)
	{
		CellWidth  = 1.0f / (GridWidth / GridHorizontalCount);
		CellHeight = 1.0f / (GridHeight / GridVerticalCount);
		BucketsElements.assign(GridHorizontalCount * GridVerticalCount, 0);

		for each(auto Index in Indicies)
		{
			if(CollisionComponent[Index]->IsCollidable())
			{
				auto BodyBox = BodyComponent[Index]->GetBoundingBox();
				if( BoxIntersection(BodyBox, GridBox) )
				{
					ColMin = static_cast<int>(floor(abs(BodyBox.min.x) * CellWidth));
					ColMax = static_cast<int>(floor(abs(BodyBox.max.x) * CellWidth)) + 1;
					RowMin = static_cast<int>(floor(abs(BodyBox.min.y) * CellHeight));
					RowMax = static_cast<int>(floor(abs(BodyBox.max.y) * CellHeight)) + 1;
	
					if(ColMax >= 0 && RowMax >= 0)
					{
						RowMin = RowMin >= 0 ? RowMin : 0;
						ColMin = ColMin >= 0 ? ColMin : 0;

						RowMax = RowMax >= GridHorizontalCount ? GridHorizontalCount : RowMax;
						ColMax = ColMax >= GridVerticalCount ? GridVerticalCount : ColMax;

						for(int ii = RowMin; ii < RowMax; ++ii)
						{
							for(int jj = ColMin; jj < ColMax; ++jj)
							{
								InsertElement(ii,jj,Index);
							}
						}
					}
				}
			}
		}
	
		int	Index1;
		int	Index2;
				
		for(int ii=0; ii < static_cast<int>(Buckets.size()); ++ii)
		{
			for(int kk=0; kk < (BucketsElements[ii]-1); ++kk)
			{
				Index1 = Buckets[ii][kk];
				for(int jj=kk+1; jj < BucketsElements[ii]; ++jj)
				{
					Index2 = Buckets[ii][jj];
					if(  (CollisionComponent[Index1]->GetCategoryMask() & CollisionComponent[Index2]->GetCollisionMask()) != 0 &&
						 (CollisionComponent[Index2]->GetCategoryMask() & CollisionComponent[Index1]->GetCollisionMask()) != 0 )
					{
						if (BoxIntersection( BodyComponent[Index1]->GetBoundingBox(), BodyComponent[Index2]->GetBoundingBox() )) 
						{
							Output.push_back( std::move( CollisionPair(Index1, Index2)));
						}
					}
				}
			}
		}
		

		if(!Output.empty())
		{
			std::sort(Output.begin(), Output.end(), []( const CollisionPair& P1, const CollisionPair& P2 )->bool { return P1.GetId() < P2.GetId(); });
			unsigned CurrentIndex = 0;
			for(unsigned ii=1; ii < Output.size(); ++ii)
			{
				if(Output[ii] != Output[CurrentIndex])
				{
					CurrentIndex++;
					if(CurrentIndex != ii) Output[CurrentIndex] = Output[ii];
				}
			}
			Output.resize(CurrentIndex+1);
		}
	}
};