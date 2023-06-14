#pragma once
#include <set>
#include <vector>
#include "PositionComponent.h"
#include "BodyComponent.h"
#include "CollisionComponent.h"
#include "CollisionPair.h"
#include "ComponentContainer.h"

const int DefaultBucketSize = 100;
const int DefaultGridHorizontalCount = 6;
const int DefaultGridVerticalCount	 = 6;


class DynamicGrid
{

	float							GridWidth;
	float							GridHeight;
	int								GridVerticalCount;
	int								GridHorizontalCount;
	int								RowMin;
	int								RowMax;
	int								ColMin;
	int								ColMax;
	float							CellWidth;
	float							CellHeight;
	Rectangle2						GridBox;

	std::vector<std::vector<int>>	Buckets;
	std::vector<int>				BucketsSize;
	std::vector<int>				BucketsElements;

	void							UpdateGrid();
	void							InsertElement(const int& row, const int& col, const int& value);
	bool							BoxIntersection(const Rectangle2&, const Rectangle2&) const;
	
public:

									DynamicGrid();
	bool							Initialize();
	void							SetGridBox(const Vector2&, const Vector2&);
	void							SetGridBox(const Rectangle2&);
	void							SetGridVerticalCount(const int&);
	void							SetGridHorizontalCount(const int&);
	inline const int&				GetGridVerticalCount() const;
	inline const int&				GetGridHorizontalCount() const;
	const Rectangle2&				GetGridBox() const;

	void							Update(ComponentContainer<BodyComponent>&, ComponentContainer<CollisionComponent>&, const std::set<int>&, std::set<std::pair<int,int>>&);
	void							Update(ComponentContainer<BodyComponent>&, ComponentContainer<CollisionComponent>&, const std::vector<int>&, std::set<std::pair<int,int>>&);



	void							Update(ComponentContainer<BodyComponent>&, ComponentContainer<CollisionComponent>&, const std::set<int>&, std::vector<CollisionPair>&/*, int&*/);
};


inline const int& DynamicGrid::GetGridVerticalCount() const
{
	return GridVerticalCount;
};


inline const int& DynamicGrid::GetGridHorizontalCount() const
{
	return GridHorizontalCount;
};
