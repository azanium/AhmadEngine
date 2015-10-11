/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _SVERTEX_H_
#define _SVERTEX_H_

#include "Types.h"
#include "IVector.h"
#include "IColor.h"
#include "IDimension.h"

namespace Ahmad
{
namespace Video
{

enum E_VERTEX_TYPE
{
	EVT_STANDARD = 0,
	EVT_VERTEXPOS,
	EVT_VERTEXLIT,
	EVT_VERTEXNORMAL,
	EVT_VERTEX2TEX,
	EVT_VERTEX3TEX,
	EVT_VERTEXTANGENT,
	EVT_MAXVERTEXTYPE		// This is just for iteration
};

// ID:  Jenis vertex standar (bagus buat skybox)
// EN: A kind of standard vertex (good for skybox)
struct SVertex
{
	SVertex() {};
	SVertex( const Core::IVector3f &pos, 
			 const Core::IVector3f &normal,
		     const IColor &color,
		     const Core::IVector2f &texcoord ) 
	{
		Pos = pos;
		Normal = normal;
		Color = color;
		TexCoord = texcoord;
	};

	Core::IVector3f	Pos;
	Core::IVector3f	Normal;
	IColor			Color;
	Core::IVector2f	TexCoord;
};

// ID:  Gua make vertex type ini cuma buat ngetes
// EN: I use this vertex type for testing purposes only
struct SVertexPos
{
	SVertexPos() {};
	SVertexPos( const Core::IVector3f &pos,
				const Core::IVector2f &texcoord )
	{
		Pos = pos;
		TexCoord = texcoord;
	};

	Core::IVector3f	Pos;
	Core::IVector2f	TexCoord;
};

// ID: Wah, kalo vertex ini gua pake buat ngerender garis 3d, lihat di RenderMan::Render3DLine
// EN: I use this vertex type to render a 3d line, take a look at RenderMan::Render3DLine
struct SVertexLit
{
	SVertexLit() {};
	SVertexLit( const Core::IVector3f &pos,
				const IColor &color )
	{
		Pos = pos;
		Color = color;
	};

	Core::IVector3f	Pos;
	IColor			Color;
	Core::IVector2f	TexCoord;
};

// ID: Buat apa ya yang ini? gak tahu gua asal aja, mungkin nanti kepake
// EN: What for this vertex type? i don't know, it might come in handy later
struct SVertexNormal
{
	SVertexNormal() {};
	SVertexNormal( const Core::IVector3f &pos,
				   const Core::IVector3f &normal,
				   const Core::IVector2f &texcoord)
	{
		Pos = pos;
		Normal = normal;
		TexCoord = texcoord;
	};

	Core::IVector3f	Pos;
	Core::IVector3f	Normal;
	Core::IVector2f TexCoord;
};

// ID: Kalo ini khusus buat multitexture
// EN: This vertex type is for multitexture
struct SVertex2Tex
{
	SVertex2Tex() {};
	SVertex2Tex( const Core::IVector3f &pos,
				 const Core::IVector3f &normal,
				 const Core::IVector2f &texcoord1,
				 const Core::IVector2f &texcoord2 )
	{
		Pos = pos;
		Normal = normal;
		TexCoord1 = texcoord1;
		TexCoord2 = texcoord2;
	};

	Core::IVector3f	Pos;
	Core::IVector3f	Normal;
	Core::IVector2f	TexCoord1;
	Core::IVector2f	TexCoord2;
};

struct SVertex3Tex
{
	SVertex3Tex() {};
	SVertex3Tex( const Core::IVector3f &pos,
				 const Core::IVector3f &normal,
				 const Core::IVector2f &texcoord1,
				 const Core::IVector2f &texcoord2,
				 const Core::IVector2f &texcoord3 )
	{
		Pos = pos;
		Normal = normal;
		TexCoord1 = texcoord1;
		TexCoord2 = texcoord2;
		TexCoord3 = texcoord3;
	};

	Core::IVector3f	Pos;
	Core::IVector3f	Normal;
	Core::IVector2f	TexCoord1;
	Core::IVector2f	TexCoord2;
	Core::IVector2f	TexCoord3;
};

struct SVertexTangent
{
	SVertexTangent() {};
	SVertexTangent( const Core::IVector3f &pos,
				    const Core::IVector3f &normal,
				    const Core::IVector2f &texcoord1,
					const Core::IVector3f &tangent )
	{
		Pos = pos;
		Normal = normal;
		TexCoord1 = texcoord1;
		Tangent = tangent;
	};

	Core::IVector3f	Pos;
	Core::IVector3f	Normal;
	Core::IVector2f	TexCoord1;
	Core::IVector3f Tangent;
};

struct SVertexSkel
{
	SVertexSkel() {};
	SVertexSkel( const Core::IVector3f &pos,
				 const Core::IVector3f &normal,
				 const Core::IVector2f &texcoord1,
				 f32 fbone1, f32 fweight1,
				 f32 fbone2, f32 fweight2 )
	{
		Pos = pos;
		Normal = normal;
		TexCoord1 = texcoord1;
		fBone1 = fbone1;
		fWeight1 = fweight1;
		fBone2 = fbone2;
		fWeight2 = fweight2;
	};

	Core::IVector3f	Pos;
	Core::IVector3f	Normal;
	Core::IVector2f	TexCoord1;
	f32				fBone1, fWeight1;
	f32				fBone2, fWeight2;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _SVERTEX_H_