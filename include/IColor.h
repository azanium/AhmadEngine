/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _ICOLOR_H_
#define _ICOLOR_H_

#include "Types.h"

namespace Ahmad
{
namespace Video
{

enum E_COLOR_FORMAT
{
	ECF_R5G6B5 = 0,		// no 16 bits texture color format supported yet..
	ECF_R8G8B8,
	ECF_A8R8G8B8
};

#define RGB16BIT(r, g, b)	(((r % 32) << 11) + ((g % 64) << 5) + (b % 32))
#define RGB16TO32BIT(color)	( (0xff < 24) | ((color >> 11 & 0x1F) << 16) | ((color >> 5 & 0x3F) << 8) | (color & 0x1F))

//! Enhanced version of color class, it used to be a struct: IColor, but now it is a class: IColor
class IColor
{
public:

	inline IColor( void ): Color(0) {}

	inline IColor( u32 r, u32 g, u32 b, u32 a ):
		   Color( ((a & 0xff)  << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff) ) { }
	
	inline IColor( u32 color ): Color(color) {}

	inline void Set( const IColor &color ) { Set(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()); }
	inline void Set( u32 r, u32 g, u32 b, u32 a ) { Color = ( ((a & 0xff)  << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff)); }
    
	inline u32	GetRed( void ) const { return (Color>>16) & 0xff; }
	inline u32	GetGreen( void ) const { return ((Color>>8) & 0xff); }
	inline u32	GetBlue( void ) const { return (Color & 0xff); }
	inline u32	GetAlpha( void ) const { return ((Color>>24) & 0xff); }

	inline f32  GetRedf( void ) const { return (f32)(GetRed() / 255); }
	inline f32  GetGreenf( void ) const { return (f32)(GetGreen() / 255); }
	inline f32  GetBluef( void ) const { return (f32)(GetBlue() / 255); }
	inline f32  GetAlphaf( void ) const { return (f32)(GetAlpha() / 255); }
		
	inline void SetRed( u32 r ) { Color = ((Color>>24 & 0xff) << 24) | ((r & 0xff) << 16) | ((Color>>8 & 0xff) << 8) | (Color & 0xff); }
	inline void SetGreen( u32 g ) { Color = ((Color>>24 & 0xff) << 24) | ((Color>>16 & 0xff) << 16) | ((g & 0xff) << 8) | (Color & 0xff); }
	inline void SetBlue( u32 b ) { Color = ((Color>>24 & 0xff) << 24) | ((Color>>16 & 0xff) << 16) | ((Color>>8 & 0xff) << 8) | (b & 0xff); }
	inline void SetAlpha( u32 a ) { Color = ((a & 0xff) << 24) | ((Color>>16 & 0xff) << 16) | ((Color>>8 & 0xff) << 8) | (Color & 0xff); }
	inline u32	GetColor( void ) { return Color; }

	inline void	operator =( const IColor &color ) { Color = color.Color; }
	inline bool operator ==( const IColor &color ) const { return Color == color.Color; }
	inline bool operator !=( const IColor &color ) const { return Color != color.Color; }

	u32 Color;	
};

//! The color class for floating point value of RGBA
class IColorf
{
public:

	inline IColorf( void ): fR(0.0f), fG(0.0f), fB(0.0f), fA(0.0f) { }
	
	inline IColorf( f32 r, f32 g, f32 b, f32 a=1.0f ): fR(r), fG(g), fB(b), fA(a) { }
	inline IColorf( u32 r, u32 g, u32 b, u32 a=255 ) { const f32 inv = 1.0f/255.0f; fR = r*inv; fG = g*inv; fB = b*inv; fA = a*inv; }

	inline IColorf( const IColorf &col ) { *this = col; }
	
	inline IColorf( const IColor &col ) { const f32 inv = 1.0f/255.0f; fR = col.GetRed()*inv; fG = col.GetGreen()*inv; fB = col.GetBlue()*inv; fA = col.GetAlpha()*inv; }

	inline void Set( f32 r, f32 g, f32 b, f32 a=1.0f ) { fR = r; fG = g; fB = b; fA = a; }
	inline void Set( u32 r, u32 g, u32 b, u32 a=255 ) { const f32 inv = 1.0f/255.0f; fR = r*inv; fG = g*inv; fB = b*inv; fA = a*inv; }

	inline void	SetRed( f32 r ) { fR = r; }
	inline void SetGreen( f32 g ) { fG = g; }
	inline void SetBlue( f32 b ) { fB = b; }
	inline void SetAlpha( f32 a ) { fA = a; }

	inline f32	GetRed( void ) const { return fR; }
	inline f32	GetGreen( void ) const { return fG; }
	inline f32	GetBlue( void ) const { return fB; }
	inline f32	GetAlpha( void ) const { return fA; }

	inline void operator =( const IColor &col ) 
	{ 
		const f32 inv = 1.0f/255.0f; 
		fR = col.GetRed()*inv; 
		fG = col.GetGreen()*inv; 
		fB = col.GetBlue()*inv; 
		fA = col.GetAlpha()*inv; 
	};

	inline void	operator =( const IColorf &color ) 
	{ 
		fR = color.GetRed(); 
		fG = color.GetGreen(); 
		fB = color.GetBlue();
		fA = color.GetAlpha();
	};

	inline bool operator ==( const IColorf &color ) const
	{ 
		if ((fR == color.GetRed()) &&
			(fG == color.GetGreen()) &&
			(fB == color.GetBlue()) &&
			(fA == color.GetAlpha()))
			return true;

		return false;
	};

	inline bool operator !=( const IColorf &color )  const
	{ 
		if ((fR != color.GetRed()) ||
			(fG != color.GetGreen()) ||
			(fB != color.GetBlue()) ||
			(fA != color.GetAlpha()))
			return true;

		return false;
	};

	f32	fR;
	f32 fG;
	f32 fB;
	f32 fA;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _IColor_H_