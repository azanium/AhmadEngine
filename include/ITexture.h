/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _ITEXTURE_H_
#define _ITEXTURE_H_

#include "Types.h"
#include "IBuyut.h"
#include "IColor.h"
#include "IDimension.h"
#include "ISurface.h"

namespace Ahmad
{
namespace Video
{

enum E_TEXTURE_FORMAT
{
	ETF_ALPHA				= 0x00000001,
	ETF_R5G6B5				= 0x00000002,
	ETF_R8G8B8				= 0x00000004,
	ETF_A8R8G8B8			= 0x00000008,
	ETF_GENERATEMIPMAPS		= 0x00000010
};

//! This texture class has to be platform independent
class ITexture: public IBuyut
{
public:
	ITexture( void ) {}

	virtual ~ITexture( void ) {}

	virtual	void			*GetTexture( void ) = 0;

	virtual E_COLOR_FORMAT	GetColorFormat( void ) = 0;

	virtual u32				GetPitch( void ) = 0;

	virtual Core::IDimension<u32>& GetDimension( void ) = 0;

	virtual bool			IsAvailable( void ) = 0;

	virtual c8				*GetTextureName( void ) = 0;

};

}	// namespace Video
}	// namespace Ahmad

#endif	// _ITEXTURE_H_