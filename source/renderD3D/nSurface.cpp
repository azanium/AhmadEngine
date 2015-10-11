/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nSurface.h"

namespace Ahmad
{
namespace Video
{

nSurface::nSurface( E_COLOR_FORMAT ecfType, const Core::IDimension<u32> &dim, void *Data ):
m_cName(0)
{
	SetupSurface( ecfType, dim, Data );
}

nSurface::nSurface( const c8 *name, E_COLOR_FORMAT ecfType, const Core::IDimension<u32> &dim, void *Data )
{
	if (name)
	{
		m_cName = new c8[strlen(name)+1];
		memcpy(m_cName, name, strlen(name)+1);
	}
	SetupSurface( ecfType, dim, Data );
};

void nSurface::SetupSurface( E_COLOR_FORMAT ecfType, const Core::IDimension<u32> &dim, void *Data )
{
	m_ColorFormat = ECF_A8R8G8B8;
	m_Dimension = dim;
	m_nPitch = GetBitsPerPixels() / 8;

	// bug fixed: 
	// the previous code: m_pData = 0xbe70; now: m_pData = NULL;
	m_pData = NULL;
	AllocData();
	switch (ecfType)
	{
		case ECF_A8R8G8B8:
			{
				memcpy(m_pData, Data, dim.Width * dim.Height * m_nPitch);
			}

		case ECF_R8G8B8:
			{
				IColor color;
				for (u32 y=0; y<dim.Height; y++)
					for (u32 x=0; x<dim.Width; x++)
					{
						u32 i = (y * dim.Width + x)*3;
						u8 *col = &((u8 *)Data)[i];
						color.Set(col[0], col[1], col[2], 255);
						((u32 *)m_pData)[y * m_Dimension.Width + x] = color.Color;
					}
			}
	}
}

nSurface::~nSurface( void )
{
	delete [] m_pData;

	if (m_cName)
	{
		delete [] m_cName;
		m_cName = 0;
	}
}

void *nSurface::GetData( void )
{
	return m_pData;
}

Core::IDimension<u32>& nSurface::GetDimension( void )
{
	return m_Dimension;
}

u32 nSurface::GetBitsPerPixels( void )
{
	switch (m_ColorFormat)
	{
	case ECF_R8G8B8:
		return 24;

	case ECF_A8R8G8B8:
		return 32;
		break;
	}

	return 0;
}

u32 nSurface::GetPitch( void )
{
	return m_nPitch;
}

void nSurface::AllocData( void )
{
	if (!m_pData)
		m_pData = new s8[m_Dimension.Width * m_Dimension.Height * m_nPitch];
}

E_COLOR_FORMAT	nSurface::GetColorFormat( void )
{
	return m_ColorFormat;
}

IColor nSurface::GetPixel( u32 x, u32 y )
{
	if ((x>=m_Dimension.Width) || (y>=m_Dimension.Height))
		return IColor(0);

	switch (m_ColorFormat)
	{
	case ECF_R8G8B8:	// 24 bit
		{
			u8 *bit = &((u8 *)m_pData)[(y*3) * m_Dimension.Width + (x*3)];
			return IColor(bit[0], bit[1], bit[2], 255);

		} break;

	case ECF_A8R8G8B8:	// 32 bit
		return IColor(((u32 *)m_pData)[y * m_Dimension.Width + x]);
	}

	return IColor(0);
}

u32 nSurface::GetSize( void )
{
	return m_nPitch * m_Dimension.Width * m_Dimension.Height;
}

ISurface *createSurface( E_COLOR_FORMAT	epfType, const Core::IDimension<u32> &dim, void *Data )
{
	return new nSurface( epfType, dim, Data );
}

void nSurface::SetAlphaKey( const IColor &color )
{
	IColor keyColor(color.GetRed(), color.GetGreen(), color.GetBlue(), (u32)255);
	IColor newColor;
	if (color.GetAlpha()>0) newColor.Set(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha());
	else newColor.Set(0, 0, 0, color.GetAlpha());

	switch (m_ColorFormat)
	{
		case ECF_A8R8G8B8:	// 32 bit
			{
				for (u32 y=0; y<m_Dimension.Height; y++)
					for (u32 x=0; x<m_Dimension.Width; x++)
					{
						if (((u32 *)m_pData)[y * m_Dimension.Width + x] == keyColor.Color)
							((u32 *)m_pData)[y * m_Dimension.Width + x] = newColor.Color;
					}
			}
	}
}

void nSurface::SetTransparency( u32 alpha )
{
	IColor oldColor;

	switch (m_ColorFormat)
	{
		case ECF_A8R8G8B8:	// 32 bit
			{
				for (u32 y=0; y<m_Dimension.Height; y++)
					for (u32 x=0; x<m_Dimension.Width; x++)
					{
						oldColor.Color = ((u32 *)m_pData)[y * m_Dimension.Width + x];

						//if (oldColor.GetAlpha() >= alpha)
							oldColor.SetAlpha(alpha);

						((u32 *)m_pData)[y * m_Dimension.Width + x] = oldColor.Color;
					}
			}
	}
}

c8 *nSurface::GetSurfaceName( void )
{
	return m_cName;
}

}	// namespace Video
}	// namespace Ahmad