#pragma once

class CollisionPair
{
	
		unsigned Body1;
		unsigned Body2;
		unsigned Id;
public:
		CollisionPair();
		CollisionPair(const unsigned&, const unsigned&);
		inline bool						operator==(const CollisionPair &);
		inline bool						operator!=(const CollisionPair &);
		inline bool						operator<(const CollisionPair &);
		inline bool						operator>(const CollisionPair &);
		inline const unsigned GetBody1() const;
		inline const unsigned GetBody2() const;
		inline const unsigned GetId() const;
};


inline bool	CollisionPair::operator==(const CollisionPair& CollisionPair)
{
	return Id == CollisionPair.Id;
};


inline bool	CollisionPair::operator!=(const CollisionPair& CollisionPair)
{
	return Id != CollisionPair.Id;
};


inline bool	CollisionPair::operator<(const CollisionPair& CollisionPair)
{
	return Id < CollisionPair.Id;
};

inline bool	CollisionPair::operator>(const CollisionPair& CollisionPair)
{
	return Id > CollisionPair.Id;
};

inline const unsigned CollisionPair::GetBody1() const
{
	return Body1;
};


inline const unsigned CollisionPair::GetBody2() const
{
	return Body2;
};


inline const unsigned CollisionPair::GetId() const
{
	return Id;
};