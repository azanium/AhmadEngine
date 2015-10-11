/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _ISURFACE_H_
#define _ISURFACE_H_

#include "Types.h"
#include "IBuyut.h"
#include "IDimension.h"
#include "IColor.h"

namespace Ahmad
{
namespace Video
{

class ISurface: public IBuyut
{
public:
	ISurface( void ) {}
	virtual ~ISurface( void ) {}

	virtual void			*GetData( void ) = 0;

	virtual Core::IDimension<u32>& GetDimension( void ) = 0;

	virtual u32				GetBitsPerPixels( void ) = 0;

	virtual u32				GetPitch( void ) = 0;

	virtual E_COLOR_FORMAT	GetColorFormat( void ) = 0;

	virtual IColor			GetPixel( u32 x, u32 y ) = 0;
	
	virtual u32				GetSize( void ) = 0;

	virtual void			SetAlphaKey( const IColor &color ) = 0;

	virtual void			SetTransparency( u32 alpha ) = 0;

	virtual c8				*GetSurfaceName( void ) = 0;
};

ISurface *createSurface( E_COLOR_FORMAT	epfType, const Core::IDimension<u32> &dim, void *Data );

}	// namespace Video
}	// namespace Ahmad


#endif	// _ISURFACE_H_