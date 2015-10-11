/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IIMAGELOADER_H_
#define _IIMAGELOADER_H_

#include "Types.h"
#include "IBuyut.h"
#include "ISurface.h"
#include "IFileIO.h"

namespace Ahmad
{
namespace Video
{

class IImageLoader: public IBuyut
{
public:
	IImageLoader( void ) { }
	virtual ~IImageLoader( void ) { }

	virtual bool		IsExtensionValid( const c8 *cFileName ) = 0;

	virtual bool		IsContentValid( const c8 *cFileName ) = 0;

	virtual ISurface	*LoadImage( IO::IFileIO *file ) = 0;

};

}	// namespace Video
}	// namespace Ahmad

#endif	// _IIMAGELOADER_H_