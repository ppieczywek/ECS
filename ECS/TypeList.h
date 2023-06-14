#pragma once


template<typename H,typename T> struct TypeList 
{
	typedef H head;
	typedef T tail;
};


class NullType {};


#define TYPELIST_1(T1)	TypeList< T1, NullType >
#define TYPELIST_2(T1, T2)	TypeList< T1, TYPELIST_1(T2) >
#define TYPELIST_3(T1, T2, T3)	TypeList< T1, TYPELIST_2(T2, T3) >
#define TYPELIST_4(T1, T2, T3, T4)	TypeList< T1, TYPELIST_3(T2, T3, T4) >
#define TYPELIST_5(T1, T2, T3, T4, T5)	TypeList< T1, TYPELIST_4(T2, T3, T4, T5) >
#define TYPELIST_6(T1, T2, T3, T4, T5, T6)	TypeList< T1, TYPELIST_5(T2, T3, T4, T5, T6) >


template<typename T> struct Length;


template<> struct Length<NullType> 
{
	enum {value = 0};
};
 

template<typename H,typename T> struct Length<TypeList<H,T> > 
{
	enum {value = 1 + Length<T>::value};
};


/*
template <int I, class a, class b>
class squared : public squared<I - 1, a, b>
{
    int x;
public:
    squared() : x((I - 1) * (I - 1)) { b[x] = 1; }
};

template <class a, class b>
class squared<1, a, b> 
{
    int x;
public:
    squared() : x(0) { b[x] = 1; }
};*/



template <class TList, class T> struct IndexOf;
template <class T>
struct IndexOf<NullType, T>
{
	enum { value = -1 };
};


template <class T, class Tail>
struct IndexOf<TypeList<T, Tail>, T>
{
	enum { value = 0 };
};


template <class Head, class Tail, class T>
struct IndexOf<TypeList<Head, Tail>, T>
{
private:
	enum { temp = IndexOf<Tail, T>::value };
public:
	enum { value = temp == -1 ? -1 : 1 + temp };
};


template<class TList, unsigned int index> struct TypeAt;
 
template<class Head, class Tail>
struct TypeAt<TypeList<Head, Tail>, 0>
{
	typedef Head Result;
};
 
template<class Head, class Tail, unsigned int i>
struct TypeAt<TypeList<Head, Tail>, i> 
{
	typedef typename TypeAt< Tail, i - 1>::Result Result;
};


template <class TList, template <class> class Unit>
class GenScatterHierarchy;


template <class Head, class Tail, template <class> class Unit>
class GenScatterHierarchy<TypeList<Head, Tail>, Unit> : public GenScatterHierarchy<Head, Unit>, public GenScatterHierarchy<Tail, Unit>
{
	typedef TypeList<Head, Tail> TList;
	typedef GenScatterHierarchy<Head, Unit> LeftBase;
	typedef GenScatterHierarchy<Tail, Unit> RightBase;
};


template <class AtomicType, template <class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType>
{
	typedef Unit<AtomicType> LeftBase;
};


template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit>
{

};

