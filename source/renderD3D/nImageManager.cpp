/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nImageManager.h"
#include "IFileSystem.h"

namespace Ahmad
{
namespace Video
{

nImageManager::nImageManager( IRenderDevice *pParent ): m_pParentRenderDevice(pParent)
{
	m_nLoaderSize = MAX_IMAGELOADER;
	
	m_Loader = (IImageLoader **)malloc(sizeof(IImageLoader *)*m_nLoaderSize);
	
	m_Loader[0] = createImageLoaderBmp();
}

nImageManager::~nImageManager( void )
{
	if (m_Loader)
	{
		for (u32 i=0; i<m_nLoaderSize; i++)
		{
			SAFE_THROW(m_Loader[i]);
		}
		SAFE_DELETE(m_Loader);
	}
}

ISurface *nImageManager::LoadImageFromFile( const c8 *filename )
{
	ISurface		*surface = NULL;
	IO::IFileIO		*fs = NULL;

	for (u32 i=0; i<m_nLoaderSize; i++)
	{
		if (m_Loader[i]->IsExtensionValid( filename ))
		{
			fs = IO::createFileIO( filename, IO::EFI_IO_Read );
			if (fs)
			{
				surface = m_Loader[i]->LoadImage( fs );
			}
			SAFE_DELETE(fs);
			if (surface) break;
		}

		if (!surface)
		{

			if (m_Loader[i]->IsContentValid( filename ))
			{
				fs = IO::createFileIO( filename, IO::EFI_IO_Read );
				if (fs)
				{
					surface = m_Loader[i]->LoadImage( fs );
				}
				SAFE_THROW(fs);

				if (surface) break;
			}
		}
	}

	return surface;
}


/*********************************
  ImageManager Creation
 *********************************/

IImageManager *createImageManager( IRenderDevice *pParent )
{
	return new nImageManager( pParent );
}

}	// namespace Video
}	// namespace Ahmad