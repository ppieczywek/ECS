#include "stdafx.h"
#include "Vector3.h"

const Vector3&	Vector3::normalize(void) {
	auto l = length(); 
	if(l>0){ 
		return (*this)/=l;
	} else {
		x = 0.0f; y=0.0f;
		return *this; };
};

const Vector3	Vector3::unit() const {
	auto l = length(); 
	if(l>0){ 
		return (*this)/l;
	} else {
		return Vector3();
	};		
};


inline const Vector3 VectorProduct(const Vector3 &vec1, const Vector3 &vec2) {
	{ return Vector3(vec1.y*vec2.z - vec1.z*vec2.y, vec1.z*vec2.x - vec1.x*vec2.z, vec1.x*vec2.y - vec1.y*vec2.x); };
};