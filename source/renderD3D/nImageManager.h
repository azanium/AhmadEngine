/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NIMAGEMANAGER_H_
#define _NIMAGEMANAGER_H_

#include "IImageManager.h"
#include "nImageLoaderBMP.h"

namespace Ahmad
{
namespace Video
{

#define MAX_IMAGELOADER		1

class nImageManager: public IImageManager
{
public:
	nImageManager( IRenderDevice *pParent );
	~nImageManager( void );

	ISurface		*LoadImageFromFile( const c8 *filename );
	IRenderDevice	*GetParentRenderDevice( void ) { return m_pParentRenderDevice; }


private:
	u32				m_nLoaderSize;
	IImageLoader	**m_Loader;
	IRenderDevice	*m_pParentRenderDevice;
};

IImageManager *createImageManager( IRenderDevice *pParent );

}	// namespace Video
}	// namespace Ahmad

#endif	// _NIMAGEMANAGER_H_