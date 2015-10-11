/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include <string.h>
#include "nImageLoaderBMP.h"
#include "IFileIO.h"
#include "nSurface.h"

namespace Ahmad
{
namespace Video
{

bool nImageLoaderBmp::IsExtensionValid( const c8 *cFileName )
{
	if ((strstr(cFileName, ".bmp")) || (strstr(cFileName, ".BMP"))) return true;
	return false;
}

bool nImageLoaderBmp::IsContentValid( const c8 *cFileName )
{
	IO::IFileIO *fp = IO::createFileIO( cFileName, IO::EFI_IO_Read );
	if (!fp) return false;

	c8 header[2];
	fp->Read(header, sizeof(c8)*2);

	if (strstr(header, "BM")) return true;
	
	fp->Close();
	
	SAFE_THROW(fp);

	return false;
}

ISurface *nImageLoaderBmp::LoadImage( IO::IFileIO *file )
{
	SBMPHeader	header;
	c8 *pData;
	c8 *tmpData;

	if (!file) return NULL;
    
	file->Seek(0);
	file->Read(&header, sizeof(SBMPHeader));

	if (header.Id != 0x4d42)
	{
		if (header.Id != 0x424d)
			return NULL;
	}

	// We only support for 24 and 32 bit image
	switch (header.Compression)
	{
		case 1:
		case 4:
		case 8:
		case 16:
			return NULL;
			break;
	}
	
	header.BitmapDataSize += (4-(header.BitmapDataSize%4))%4;
	
	if (!header.BitmapDataSize)
	{
		header.BitmapDataSize = file->Size() - header.BitmapDataOffset;
	}

	file->Seek(header.BitmapDataOffset);
	f32 t = header.Width * (header.BPP / 8.0f);
	s32 width = (s32)t;
	t -= width;
	if (t != 0.0f)
		++width;

	s32 pitch = 0;
	s32 lineData = width + ((4 - (width % 4))) % 4;
	pitch = lineData - width;

	pData = new c8[header.BitmapDataSize];
	tmpData = new c8[header.BitmapDataSize];
	file->Read(pData, header.BitmapDataSize);

	ISurface *surface;
	switch (header.BPP)
	{
		case 24:
			flipMirrorColor24Bit(pData, tmpData, header.Width, header.Height, pitch);
			
			delete [] pData;

			surface = new nSurface(file->GetFileName(), ECF_R8G8B8, Core::IDimension<u32>(header.Width, header.Height), tmpData);

			break;

		case 32:
			flipMirrorColor32Bit((s32 *)pData, (s32 *)tmpData, header.Width, header.Height, pitch);
			
			delete [] pData;
			
			surface = new nSurface(file->GetFileName(), ECF_A8R8G8B8, Core::IDimension<u32>(header.Width, header.Height), tmpData);
			
			break;
	}

	delete [] tmpData;

	return surface;
}

void nImageLoaderBmp::flipMirrorColor24Bit(const c8 *pIn, c8 *pOut, s32 nWidth, s32 nHeight, s32 nPitch )
{
	const c8 *p = pIn;
	const s32 lineWidth = 3 * nWidth + nPitch;
	pOut += nWidth * nHeight * 3 + nPitch;

	for (s32 y=0; y<nHeight; ++y)
	{
		for (s32 x=0; x<nWidth; ++x)
		{
			p = pIn + (nWidth-x-1)*3;
			pOut -= 3;
			pOut[0] = p[2];
			pOut[1] = p[1];
			pOut[2] = p[0];
		}

		pIn += lineWidth;
	}
}

void nImageLoaderBmp::flipMirrorColor32Bit( const s32 *pIn, s32 *pOut, s32 nWidth, s32 nHeight, s32 nPitch )
{
	const s32 *p = pIn;
	const s32 lineWidth = nWidth;
	pOut += nHeight * nWidth;

	for (s32 y=0; y<nHeight; ++y)
	{
		for (s32 x=0; x<nWidth; ++x)
		{
			p = pIn + (nWidth-x-1);
			--pOut;
			*pOut = *p;
		}
		pIn += lineWidth;
	}
}


/*************************************
  ImageLoaderBmp Creation
 *************************************/

IImageLoader *createImageLoaderBmp( void )
{
	return new nImageLoaderBmp();
}

}	// namespace Video
}	// namespace Ahmad