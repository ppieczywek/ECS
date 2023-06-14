#pragma once
#include "Observer.h"

class Subject
{
			Observer*			observer;
	public:

								Subject();
			inline void			AcceptObserver(Observer*);
			inline Observer*	GetObserver();
};


void Subject::AcceptObserver(Observer* Observer)
{
	observer = Observer;
};


Observer*   Subject::GetObserver()
{
	return observer;
};