/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _RENDERSTATE_H_
#define _RENDERSTATE_H_

#include "Types.h"
#include "IColor.h"
#include "IVector.h"

namespace Ahmad
{
namespace Video
{

// Built-in font declaration 
#define FONT_VERDANA_NORMAL			0
#define FONT_VERDANA_BOLD			1
#define FONT_ARIAL_NORMAL			2
#define FONT_ARIAL_BOLD				3
#define FONT_TIMESNEWROMAN_NORMAL	4
#define FONT_TIMESNEWROMAN_BOLD		5

enum E_VIEW_MODE
{
	EVM_TWODIMENSION = 0,	// Two dimensional viewing mode (use the screen coordinate: in pixels)
	EVM_ORTHOGONAL,			// Use the orthogonal viewing mode (use the cartesian coordinate: in pixels)
	EVM_PERSPECTIVE			// Use the perspective viewing mode (use the cartesian coordinate: in units)
};

enum E_DEPTH_MODE
{
	EDM_NONE = 0,			// No Z-buffering supported
	EDM_READ,				// Read-Only Z-buffer, and no overwrite
	EDM_READWRITE			// Read and Write the Z-buffer
};

enum E_CULL_MODE
{
	ECM_NONE = 0,			// No culling
	ECM_CW,					// Clock-wise culling
	ECM_CCW					// Counter-Clock-Wise culling
};

enum E_TEXTURE_FILTER
{
	ETF_NEAREST = 0,		// Nearest Texture Filtering
	ETF_BILINEAR,			// Bilinear Texture Filtering
	ETF_TRILINEAR,			// Trilinear Texture Filtering
	ETF_ANISOTROPIC			// Anisotropic Texture Filtering
};

enum E_SHADE_MODE
{
	ESM_POINTS = 0,			// Per-vertex point
	ESM_POINTSPRITE,		// Texture sprite point
	ESM_WIREFRAME,			// Textured Wireframe
	ESM_LINES,				// Textured Lines
	ESM_SOLID				// Textured Solid
};

enum E_FOG_CALC
{
	EFC_LINEAR = 0,			// Linear fog calculation
	EFC_EXP,				// Exp fog calculation
	EFC_EXP2				// Exp2 fog calculation
};

enum E_TEXTURE_OP
{
	ETO_NONE = 0,			// No texture operation (disabled)
	ETO_GLOWMAP,			// Glow Map texture operation
	ETO_DARKMAP				// Dark Map texture operation
};

enum E_SHADER_VERSION
{
	ESV_VSHADER_1_1 = 0,	// Vertex shader v1.1
	ESV_PSHADER_1_1,		// Pixel shader v1.1
	ESV_VSHADER_1_4,		// Vertex shader v1.4
	ESV_PSHADER_1_4,		// Pixel shader v1.4
	ESV_VSHADER_2_0,		// Vertex shader v2.0
	ESV_PSHADER_2_0,		// Pixel shader v2.0
	ESV_VSHADER_3_0,		// Vertex shader v3.0
	ESV_PSHADER_3_0,		// Pixel shader v3.0
	ESV_MAXSHADERVERSION	// Maximum shader version for the use of the engine
};

struct SFog
{
	IColor		Color;
	f32			StartFog;
	f32			EndFog;
	f32			Density;
	E_FOG_CALC	efcType;	
	bool		PixelFog;

	SFog() 
	{
		Color.Set(0, 0, 0, 0);
		StartFog = 0;
		EndFog = 0;
		Density = 0;
		efcType = EFC_LINEAR;
	};

	SFog( const IColor &color, f32 fStartFog, f32 fEndFog, f32 fDensity, bool bPixelFog, E_FOG_CALC efc )
	{
		Color = color;
		StartFog = fStartFog;
		EndFog = fEndFog;
		Density = fDensity;
		PixelFog = bPixelFog;
		efcType = efc;
	};

	inline void Set( const IColor &color, f32 fStartFog, f32 fEndFog, f32 fDensity, bool bPixelFog, E_FOG_CALC efc )
	{
		Color = color;
		StartFog = fStartFog;
		EndFog = fEndFog;
		Density = fDensity;
		PixelFog = bPixelFog;
		efcType = efc;
	};
};
	
struct SViewport
{
	SViewport() {};

	SViewport( u32 x, u32 y, u32 width, u32 height)
	{
		X = x;
		Y = y;
		Width = width;
		Height = height;
	};

	inline void operator=( const SViewport &vp )
	{
		X = vp.X;
		Y = vp.Y;
		Width = vp.Width;
		Height = vp.Height;
	};

	inline bool operator==( const SViewport &vp ) const
	{
		if ((X == vp.X) && (Y == vp.Y) && (Width == vp.Width) && (Height == vp.Height))
			return true;
	
		return false;
	};

	inline bool operator!=( const SViewport &vp ) const
	{
		if ((X != vp.X) || (Y != vp.Y) || (Width != vp.Width) || (Height != vp.Height))
			return true;
	
		return false;
	};

	u32	X;
	u32 Y;
	u32 Width;
	u32 Height;
};

}	// namespace Video
}	// namespace Ahmad

#endif	_RENDERSTATE_H_