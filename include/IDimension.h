/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IDIMENSION_H_
#define _IDIMENSION_H_

namespace Ahmad
{
namespace Core
{

//! To keep anything hidden from the user, move the implementation into
//! the .CPP file, but i'm too lazy to do such things, so this class only
//! has one file Dimension.h. That's all it takes to use the Dimension. 

template <class T>
class IDimension
{
public:
	IDimension()
		: Width(0), Height(0) {};

	IDimension( T width, T height )
		: Width(width), Height(height) {};

	IDimension( const IDimension<T> &Dimension )
		: Width(Dimension.Width), Height(Dimension.Height) {};

	bool operator ==( const IDimension<T> &Dimension ) const
	{
		return (m_nWidth == Dimension.Width) && (m_nHeight == Dimension.Height);
	}
    
	bool operator !=( const IDimension<T> &Dimension ) const
	{
		return (m_nWidth != Dimension.Width) || (m_nHeight != Dimension.Height);
	}

	const IDimension<T>& operator =( const IDimension<T> &Dimension )
	{
		Width = Dimension.Width;
		Height = Dimension.Height;

		return *this;
	}

	T	Width, Height;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _Dimension_H_