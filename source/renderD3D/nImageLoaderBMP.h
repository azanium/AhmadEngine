/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NIMAGELOADERBMP_H_
#define _NIMAGELOADERBMP_H_

#include "IImageLoader.h"

#include "IImageManager.h"	// The parent

namespace Ahmad
{
namespace Video
{

#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACKED
#endif

	struct SBMPHeader
	{
		u16	Id;					
		u32	FileSize;
		u32	Reserved;
		u32	BitmapDataOffset;
		u32	BitmapHeaderSize;	 
		u32 Width;
		u32 Height;
		u16 Planes;
		u16	BPP;					
		u32 Compression;			
		u32 BitmapDataSize;		
		u32 PixelPerMeterX;
		u32 PixelPerMeterY;
		u32 Colors;
		u32 ImportantColors;

#ifdef PACKED
	} PACKED;
#else
	};
#endif

#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#ifdef PACKED
#undef PACKED
#endif

class nImageLoaderBmp: public IImageLoader
{
public:
	//nImageLoaderBmp( IImageManager *pParent ): m_pParent(pParent) { }
	nImageLoaderBmp( void ) { }
	~nImageLoaderBmp( void ) { }

	bool			IsExtensionValid( const c8 *cFileName );
    
	bool			IsContentValid( const c8 *cFileName );

	ISurface		*LoadImage( IO::IFileIO *file );

//	IImageManager	*GetParentImageManager( void ) { return m_pParent; }

private:
	void			flipMirrorColor24Bit( const c8 *pIn, c8 *pOut, s32 nWidth, s32 nHeight, s32 nPitch );
	void			flipMirrorColor32Bit( const s32 *pIn, s32 *pOut, s32 nWidth, s32 nHeight, s32 nPitch );
	
	//IImageManager	*m_pParent;

};

//IImageLoader	*createImageLoaderBmp( IImageManager *pParent );
IImageLoader	*createImageLoaderBmp( void );

}	// namespace Video
}	// namespace Ahmad

#endif	// _NIMAGELOADERBMP_H_
