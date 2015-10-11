/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IIMAGEMANAGER_H_
#define _IIMAGEMANAGER_H_

#include "Types.h"
#include "IBuyut.h"
#include "ISurface.h"

namespace Ahmad
{
namespace Video
{

// Parent clas for IImageManager
class IRenderDevice;

class IImageManager: public IBuyut
{
public:
	IImageManager( void ) {}
	virtual ~IImageManager( void ) {}

	virtual ISurface		*LoadImageFromFile( const c8 *filename ) = 0;
	virtual IRenderDevice	*GetParentRenderDevice( void ) = 0;
};

}	// namespace Video
}	// namesapce Ahmad

#endif	// _IIMAGEMANAGER_H_