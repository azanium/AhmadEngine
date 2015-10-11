/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NTEXTURE_H_
#define _NTEXTURE_H_

#include <d3dx9.h>
#include "ITexture.h"
#include "IColor.h"

namespace Ahmad
{
namespace Video
{

class nTexture: public ITexture
{
public:
	nTexture( LPDIRECT3DDEVICE9 pDevice, ISurface *surface );
	nTexture( LPDIRECT3DDEVICE9 pDevice, ISurface *surface, u32 nTexFormat );
	nTexture( const c8 *name, LPDIRECT3DDEVICE9 pDevice, ISurface *surface );
	nTexture( const c8 *name, LPDIRECT3DDEVICE9 pDevice, ISurface *surface, u32 nTexFormat );
	~nTexture( void );

	void				*GetTexture( void );

	E_COLOR_FORMAT		GetColorFormat( void );

	u32					GetPitch( void );

	Core::IDimension<u32>&	GetDimension( void );

	bool				IsAvailable( void ) { if (m_pSurface) return true; else return false; }

	ISurface			*GetSurface( void );

	c8					*GetTextureName( void );

private:
	E_TEXTURE_FORMAT	getTextureFormat( u32 nTexFormat );
	bool				createTexture( void );
	bool				copySurfaceToTexture( void );

	ISurface			*m_pSurface;
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DDEVICE9	m_pDevice;
	D3DFORMAT			m_TexFormat;
	E_COLOR_FORMAT		m_ColorFormat;
	u32					m_TextureFormat;
	u32					m_nPitch;
	Core::IDimension<u32>	m_SurfaceSize;
	c8					*m_cName;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _NTEXTURE_H_