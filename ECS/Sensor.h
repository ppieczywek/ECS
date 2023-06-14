#pragma once
#include "Polygon2.h"

class Sensor
{
		long long					Id;
		Polygon2					Body;

	public:

		friend class				BodyComponent;

									Sensor();	
									Sensor(const Polygon2&, const long long&);	

		inline void					SetId(const long long&);
		inline void					SetBody(const Polygon2&);

		inline const long long&		GetId() const;
		inline Polygon2* const		GetBody();
};


inline void	Sensor::SetId(const long long& Value)
{
	Id = Value;
};


inline void	Sensor::SetBody(const Polygon2& Value)
{
	Body = Value;
};


inline const long long&	Sensor::GetId() const
{
	return Id;
};


inline Polygon2* const Sensor::GetBody()
{
	return &Body;
};