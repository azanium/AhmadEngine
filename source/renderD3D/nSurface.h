/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NSURFACE_H_
#define _NSURFACE_H_

#include "ISurface.h"

namespace Ahmad
{
namespace Video
{

class nSurface: public ISurface
{
public:
	nSurface( E_COLOR_FORMAT ecfType, const Core::IDimension<u32> &dim, void *Data );
	nSurface( const c8 *name, E_COLOR_FORMAT ecfType, const Core::IDimension<u32> &dim, void *Data );
	~nSurface( void );

	void			*GetData( void );

	Core::IDimension<u32>& GetDimension( void );

	u32				GetBitsPerPixels( void );

	u32				GetPitch( void );

	E_COLOR_FORMAT	GetColorFormat( void );

	IColor			GetPixel( u32 x, u32 y );

	u32				GetSize( void );

	void			SetAlphaKey( const IColor &color );

	void			SetTransparency( u32 alpha );
	
	c8				*GetSurfaceName( void );

private:

	void			SetupSurface( E_COLOR_FORMAT ecfType, const Core::IDimension<u32> &dim, void *Data );
	void			AllocData( void );
	void			*m_pData;

	Core::IDimension<u32>	m_Dimension;
	E_COLOR_FORMAT			m_ColorFormat;
	u32						m_nPitch;
	u32						m_nBitsPerPixels;
	c8					   *m_cName;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _NSURFACE_H_