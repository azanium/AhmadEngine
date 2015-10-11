/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IRENDERMAN_H_
#define _IRENDERMAN_H_

#include "Types.h"
#include "IBuyut.h"
#include "SVertex.h"
#include "IVector.h"
#include "IColor.h"

namespace Ahmad
{
namespace Video
{

class IRenderDevice;

class IRenderMan: public IBuyut
{
public:
	IRenderMan( void ) {};
	virtual ~IRenderMan( void ) {};

	virtual void			RenderTriangle( const SVertex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderTriangle( const SVertexPos *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderTriangle( const SVertexLit *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderTriangle( const SVertexNormal *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderTriangle( const SVertex2Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderTriangle( const SVertex3Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderTriangle( const SVertexTangent *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;

	virtual void			RenderLine( const SVertex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderLine( const SVertexPos *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderLine( const SVertexLit *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderLine( const SVertexNormal *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;
	virtual void			RenderLine( const SVertex2Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount ) = 0;

	virtual void			Render3DLine( const Core::IVector3f &vStart, const Core::IVector3f &vStop, const IColor &cColor ) = 0;

	// Static Buffer
	virtual AI_RESULT		CreateStaticBuffer( E_VERTEX_TYPE evt, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount, u32 nSkinID, u32 *nSID ) = 0;
	virtual AI_RESULT		Render( u32 nSID ) = 0;
	virtual void			DeleteStaticBuffer( u32 nSID ) = 0;

	// Dynamic Buffer
	virtual void			InvalidateStates( void ) = 0;
	virtual AI_RESULT		Render( E_VERTEX_TYPE evt, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount, u32 nSkinID ) = 0;
	virtual AI_RESULT		FlushAll( void ) = 0;
	virtual AI_RESULT		Flush( E_VERTEX_TYPE evt ) = 0;
	
	virtual IRenderDevice	*GetParentRenderDevice( void ) = 0;
	virtual void			SetActiveCache( u32 nID ) = 0;
	virtual u32				GetActiveCache( void ) = 0;
	virtual void			SetActiveSB( u32 nSID ) = 0;
	virtual u32				GetActiveSB( void ) = 0;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _IRENDERMAN_H_