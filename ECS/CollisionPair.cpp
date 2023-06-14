#pragma once
#include "stdafx.h"
#include "CollisionPair.h"



CollisionPair::CollisionPair()
{	
	Body1 = 0;
	Body2 = 0;
	Id = 0;
};


CollisionPair::CollisionPair(const unsigned& B1, const unsigned& B2)
{
	if(B1 < B2)
	{
		Body1 = B1;
		Body2 = B2;
	}
	else
	{
		Body1 = B2;
		Body2 = B1;
	}

	Id = 0.5*(Body1 + Body2) * (Body1 + Body2 + 1) + Body2;
};