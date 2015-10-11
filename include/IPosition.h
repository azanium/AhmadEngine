/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _POSITION_H_
#define _POSITION_H_

#include "Types.h"

namespace Ahmad
{
namespace Core
{

template <class T>
class IPosition
{
public:
	IPosition(): X(0), Y(0) {};

	IPosition( T x, T y ): X(x), Y(y) {};

	IPosition( const IPosition<T> &pos )
	{ X = pos.X; Y = pos.Y; }

	void operator +=( const IPosition<T> &pos )
	{
		X += pos.X;
		Y += pos.Y;
	}

	void operator -=( const IPosition<T> &pos )
	{
		X -= pos.X;
		Y -= pos.Y;
	}

	void operator *=( const IPosition<T> &pos )
	{
		X *= pos.X;
		Y *= pos.Y;
	}

	void operator /=( const IPosition<T> &pos )
	{
		if (pos.X != 0 && pos.Y != 0)
		{
			X /= pos.X;
			Y /= pos.Y;
		}
	}

	IPosition<T> operator +( const IPosition<T> &pos ) const
	{
		return IPosition( X+pos.X, Y+pos.Y );
	}
	
	IPosition<T> operator -( const IPosition<T> &pos ) const
	{
		return IPosition( X-pos.X, Y-pos.Y );
	}

	IPosition<T> operator *( const IPosition<T> &pos ) const
	{
		return IPosition( X*pos.X, Y*pos.Y );
	}

	IPosition<T> operator /( const IPosition<T> &pos ) const
	{
		return IPosition( X/pos.X, Y/pos.Y );
	}

	bool operator ==( const IPosition<T> &pos ) const
	{
		if ( (X != pos.X) || (Y != pos.Y) ) return false;

		return true;
	}

	bool operator !=( const IPosition<T> &pos ) const
	{
		if ( (X == pos.X) && (Y == pos.Y) ) return true;
		
		return false;
	}

	T X, Y;
};

}	// namespace Core
}	// namespace Ahmad

#endif	// _POSITION_H_