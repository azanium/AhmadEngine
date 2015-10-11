/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nTexture.h"

namespace Ahmad
{
namespace Video
{

nTexture::nTexture( LPDIRECT3DDEVICE9 pDevice, ISurface *surface ):
m_pDevice(pDevice), m_pSurface(surface), m_TextureFormat(ETF_A8R8G8B8), 
m_pTexture(NULL), m_cName(0)
{
	if (m_pDevice) m_pDevice->AddRef();

	if (m_pSurface)
	{
		//m_pSurface->Catch();

		if (createTexture())
            copySurfaceToTexture();		
	}
}

nTexture::nTexture( LPDIRECT3DDEVICE9 pDevice, ISurface *surface, u32 nTexFormat ):
m_pDevice(pDevice), m_pSurface(surface), m_TextureFormat(nTexFormat), 
m_pTexture(NULL), m_cName(0)
{
	if (m_pDevice) m_pDevice->AddRef();

	if (m_pSurface)
	{
		//m_pSurface->Catch();

		if (createTexture())
            copySurfaceToTexture();
	}
}

nTexture::nTexture( const c8 *name, LPDIRECT3DDEVICE9 pDevice, ISurface *surface ):
m_pDevice(pDevice), m_pSurface(surface), m_TextureFormat(ETF_A8R8G8B8), 
m_pTexture(NULL), m_cName(0)
{
	if (m_pDevice) m_pDevice->AddRef();

	if (m_pSurface)
	{
		//m_pSurface->Catch();
        
		if (name)
		{
			m_cName = new c8[strlen(name)+1];
			memcpy(m_cName, name, strlen(name)+1);
		}

		if (createTexture())
            copySurfaceToTexture();		
	}
}

nTexture::nTexture( const c8 *name, LPDIRECT3DDEVICE9 pDevice, ISurface *surface, u32 nTexFormat ):
m_pDevice(pDevice), m_pSurface(surface), m_TextureFormat(nTexFormat), 
m_pTexture(NULL), m_cName(0)
{
	if (m_pDevice) m_pDevice->AddRef();

	if (m_pSurface)
	{
		//m_pSurface->Catch();

		if (name)
		{
			m_cName = new c8[strlen(name)+1];
			memcpy(m_cName, name, strlen(name)+1);
		}

		if (createTexture())
            copySurfaceToTexture();
	}
}

nTexture::~nTexture( void )
{
	//SAFE_THROW(m_pSurface);

	if (m_pTexture)
		m_pTexture->Release();

	if (m_pDevice)
		m_pDevice->Release();

	if (m_cName)
	{
		delete [] m_cName;
		m_cName = 0;
	}
}

void *nTexture::GetTexture( void )
{
	return m_pTexture;
}

E_COLOR_FORMAT nTexture::GetColorFormat( void )
{
	return m_ColorFormat;
}

E_TEXTURE_FORMAT nTexture::getTextureFormat( u32 nTexFormat )
{
	if (nTexFormat & ETF_ALPHA)
		return ETF_ALPHA;
	if (nTexFormat & ETF_R5G6B5)
		return ETF_R5G6B5;
	if (nTexFormat & ETF_R8G8B8)
		return ETF_R8G8B8;
	if (nTexFormat & ETF_A8R8G8B8)
		return ETF_A8R8G8B8;

	return ETF_R5G6B5;
}

bool nTexture::createTexture( void )
{
	HRESULT		hr;
	D3DFORMAT	fmt;

	switch (getTextureFormat( m_TextureFormat ))
	{
		case ETF_ALPHA:
		case ETF_A8R8G8B8:
		case ETF_R8G8B8:
			fmt = D3DFMT_A8R8G8B8;
			break;

		case ETF_R5G6B5:
			fmt = D3DFMT_R5G6B5;
			break;
	}

	m_SurfaceSize = m_pSurface->GetDimension();
	u32	nWidth = m_SurfaceSize.Width;
	u32	nHeight = m_SurfaceSize.Height;

	//! no mipmap at the moment
	bool mipmap = (m_TextureFormat & ETF_GENERATEMIPMAPS) != 0;

	hr = m_pDevice->CreateTexture(nWidth, nHeight, 
		1, 0, fmt, D3DPOOL_MANAGED, &m_pTexture, NULL);

	if (FAILED(hr))
	{
		fmt = D3DFMT_R5G6B5;
		hr = m_pDevice->CreateTexture(nWidth, nHeight, 
			1, 0, fmt, D3DPOOL_MANAGED, &m_pTexture, NULL);
		if (FAILED(hr))
			return false;
	}

	m_TexFormat = fmt;
	m_ColorFormat = (fmt == D3DFMT_R5G6B5) ? ECF_R5G6B5 : ECF_A8R8G8B8;

	return true;
}

bool nTexture::copySurfaceToTexture( void )
{
	HRESULT			hr;
	
	if (m_pTexture && m_pSurface)
	{
		D3DLOCKED_RECT	d3dRect;
		E_COLOR_FORMAT	fmt;
		
		fmt = m_pSurface->GetColorFormat();

		hr = m_pTexture->LockRect(0, &d3dRect, NULL, 0);		
		if (FAILED(hr)) return false;

		m_nPitch = d3dRect.Pitch;

		u32 pitch;
		if (m_TexFormat == D3DFMT_A8R8G8B8)
			pitch = m_nPitch >> 2;
		else
			pitch = m_nPitch >> 1;

		void *source = (void *)m_pSurface->GetData();
		void *dest = (u32 *)d3dRect.pBits;

		if (fmt == ECF_A8R8G8B8)
		{
			for (u32 y = 0; y < m_SurfaceSize.Height; y++)
				for (u32 x = 0; x < m_SurfaceSize.Width; x++)
				{
					if (m_TexFormat == D3DFMT_A8R8G8B8)
					{
						((u32 *)dest)[y*pitch+x] = ((u32 *)source)[y * m_SurfaceSize.Width + x];
					}
					else // D3DFMT_R5G6B5
					{
						IColor color(((u32 *)source)[y * m_SurfaceSize.Width + x]);
						((u16 *)dest)[y*pitch+x] = RGB16BIT(color.GetRed(), color.GetGreen(), color.GetBlue());
					}	
				}

		}
		else if (fmt == ECF_R8G8B8)
		{
			IColor col;
			for (u32 y = 0; y < m_SurfaceSize.Height; y++)
				for (u32 x = 0; x < m_SurfaceSize.Width; x++)
				{
					u32 i = (y * m_SurfaceSize.Width + x)*3;	// in u8, so we must multiply it with 3
					u8 *color = &((u8 *)source)[i];
					col.Set(color[0], color[1], color[2], 255);

					if (m_TexFormat == D3DFMT_A8R8G8B8)
						((u32 *)dest)[y*pitch+x] = col.Color;
					else
						((u16 *)dest)[y*pitch+x] = RGB16BIT(col.GetRed(), col.GetGreen(), col.GetBlue());
				}
		}

		hr = m_pTexture->UnlockRect(0);

		if (FAILED(hr))
			return false;

		return true;
	}

	return false;
}

u32 nTexture::GetPitch( void )
{
	return m_nPitch;
}

Core::IDimension<u32>& nTexture::GetDimension( void )
{
	return m_SurfaceSize;
}

ISurface *nTexture::GetSurface( void )
{
	HRESULT			hr;
	
	if (m_pTexture)
	{
		D3DLOCKED_RECT	d3dRect;
		E_COLOR_FORMAT	fmt;
		
		fmt = ECF_A8R8G8B8;
		LPDIRECT3DSURFACE9	ts;
		m_pTexture->GetSurfaceLevel(0, &ts);
		
		D3DSURFACE_DESC	desc;

		m_pTexture->GetLevelDesc(0, &desc);

		hr = ts->LockRect(&d3dRect, NULL, D3DLOCK_READONLY);
		//hr = m_pTexture->LockRect(0, &d3dRect, NULL, D3DLOCK_READONLY);
		if (FAILED(hr)) return false;

		m_nPitch = d3dRect.Pitch;

		u32 pitch;
		if (desc.Format == D3DFMT_A8R8G8B8)
			pitch = m_nPitch >> 2;
		else
			pitch = m_nPitch >> 1;

		// 32 bits destination
		void *dest = new u32[m_SurfaceSize.Width * m_SurfaceSize.Height * 4];
		u32 *source = (u32 *)d3dRect.pBits;
//		u32 *src;

		if (fmt == ECF_A8R8G8B8)
		{
			for (u32 y = 0; y < m_SurfaceSize.Height; y++)
			{
				//src = source;
				for (u32 x = 0; x < desc.Width; x++)
				{
					if (m_TexFormat == D3DFMT_A8R8G8B8)
					{
						//((u32 *)dest)[y * desc.Width + x] = D3DCOLOR_VALUE(0,0,0,0);//[y*m_SurfaceSize.Width+x];
						//src++;
					}
					else // D3DFMT_R5G6B5
					{						 
						((u32 *)source)[y * m_SurfaceSize.Width + x] = RGB16TO32BIT(((u16 *)dest)[y*pitch+x]);
					}	
				}
				//source += pitch;
			}

		}

		//hr = m_pTexture->UnlockRect(0);
		hr = ts->UnlockRect();
		ts->Release();

		if (FAILED(hr))
			return NULL;

		ISurface *surface = createSurface(fmt, m_SurfaceSize, dest);
		delete [] dest;

		return surface;
	}

	return NULL;
}

c8 *nTexture::GetTextureName( void )
{
	return m_cName;
}

}	// namespace Video
}	// namespace Ahmad