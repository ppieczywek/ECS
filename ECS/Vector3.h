#pragma once
#include <cmath>
#include <assert.h>

class Vector3{
public:
	float x,y,z;

	Vector3() : x(0.0f), y(0.0f), z(0.0f) {};				//konstruktor z parametrami ijednoczesnie domyslny
	Vector3(const float &xx, const float &yy, const float &zz ) : x(xx), y(yy), z(zz) {};				//konstruktor z parametrami ijednoczesnie domyslny
	Vector3( const Vector3& V) : x(V.x), y(V.y), z(V.z) {};																//konstruktor kopiuj¹cy 
	~Vector3() {};															//destruktor

	float			length(void) const	{ return sqrt( x*x + y*y + z*z); };		//d³ugoœæ wektora
	const Vector3&	normalize(void);
	const Vector3	unit() const;		//zwraca wektor jednostkowy bez zmiany bazowego
		
	bool operator!=(const Vector3 &vec) {
		if( (x != vec.x) && (y != vec.y) && (z != vec.z) ) return true;
			else		
				return false; 
	};

	bool operator==(const Vector3 &vec) {
		if( (x == vec.x) && (y == vec.y) && (z == vec.z) ) return true;
			else		
				return false; 
	};

	Vector3& operator=(const Vector3 &vec) {
		if(this == &vec) return *this;
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this; 
	};
	Vector3& operator/=(const float &scalar) {							// dzielenie przez skalar razem z przypisaniem
		assert(scalar!=0.0f);//if( scalar == 0 ) {};
		x/=scalar;
		y/=scalar;
		z/=scalar;
		return *this; 
	};
	Vector3& operator*=(const float &scalar) {							// mno¿enie przez skalar razem z przypisaniem
		//if( scalar == 0.0f ) {};
		x*=scalar;
		y*=scalar;
		z*=scalar;
		return *this; 
	};
	Vector3& operator+=(const Vector3 &vec) {							//dodawanie wektorów z przypisaniem
		x+=vec.x;
		y+=vec.y;
		z+=vec.z;
		return *this;
	};
	Vector3& operator-=(const Vector3 &vec) {							//odejmowanie wektorów z przypisaniem
		x-=vec.x;
		y-=vec.y;
		z-=vec.z;
		return *this;
	};
	
	const Vector3 operator-() const {									//odwrócenie wektora "-"
		return Vector3(-x , -y, -z);
	};
	const Vector3 operator-(const Vector3 &vec) const {				//odejmowanie wektorów
		return Vector3(x - vec.x , y - vec.y, z - vec.z);
	};
	const Vector3 operator+(const Vector3 &vec) const {				//dodawanie wektorów
		return Vector3(x + vec.x, y + vec.y, z + vec.z);
	};
	const Vector3 operator/(const float &scalar) const {				// dzielenie przez skalar
		assert(scalar!=0.0f);//if( scalar == 0 ) {};
		return Vector3(x/scalar, y/scalar, z/scalar); 
	};
	const Vector3 operator*(const float &scalar) const {				// mno¿enie przez skalar vec * sc
		return Vector3(x*scalar, y*scalar, z*scalar);
	};
	friend inline const Vector3 operator * (const float &scalar , const Vector3 &vec); //mno¿enie przez skalar  sc * vec

	const float operator*(const Vector3 &vec) const {					//iloczyn skalarny
		return ( (x * vec.x) + (y * vec.y) + (z * vec.z));
	};
}; 

inline const Vector3 operator*(const float &scalar, const Vector3 &vec) {  //mno¿enie przez skalar
		return vec*scalar;
};
	
const Vector3 VectorProduct(const Vector3 &vec1, const Vector3 &vec2);