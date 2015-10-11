/*
	Ahmad ENGINE

	CopTopright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IRECT_H_
#define	_IRECT_H_

#include "Types.h"

namespace Ahmad
{
namespace Core
{

template <class T>
class IRect
{
public:
	T Left, Top, Right, Bottom;

	IRect( void ): Left(0), Top(0), Right(0), Bottom(0) { }
	IRect( T left, T top, T right, T bottom ): Left(left), Top(top), Right(right), Bottom(bottom) { }
	IRect( const IRect<T> &r ): Left(r.Left), Top(r.Top), Right(r.Right), Bottom(r.Bottom) { }

	inline bool operator ==( const IRect<T> &r ) const
	{
		if ((Left == r.Left) && (Top == r.Top) &&	(Right == r.Right) && (Bottom == r.Bottom))
			return true;

		return false;
	};

	inline bool operator !=( const IRect<T> &r ) const
	{
		if ((Left != r.Left) || (Top != r.Top) || (Right != r.Right) || (Bottom != r.Bottom))
			return true;

		return false;
	};

	inline void operator =( const IRect<T> &r )
	{
		Left = r.Left; Top = r.Top; Right = r.Right; Bottom = r.Bottom;
	};

};

typedef IRect<f32> IRectf;
typedef IRect<u32> IRecti;

}	// namespace Core
}	// namespace Ahmad

#endif	// _IRECT_H_