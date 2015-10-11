/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NRENDERMAN_H_
#define _NRENDERMAN_H_

#include <d3d9.h>
#include "IRenderMan.h"
#include "IRenderDevice.h"
#include "Array.h"

namespace Ahmad
{
namespace Video
{

class nVCache;
class nSBuffer;

#define MAX_CACHE	10

//! This is the render manager for AhmadENGINE (Suhendra Ahmad: ceo (at) ainasoft.com)
class nRenderMan: public IRenderMan
{
public:
	nRenderMan( LPDIRECT3DDEVICE9 pDevice, IRenderDevice *pRenderDevice );

	~nRenderMan( void );

	void			RenderTriangle( const SVertex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderTriangle( const SVertexPos *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderTriangle( const SVertexLit *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderTriangle( const SVertexNormal *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderTriangle( const SVertex2Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderTriangle( const SVertex3Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderTriangle( const SVertexTangent *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );

	void			RenderLine( const SVertex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderLine( const SVertexPos *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderLine( const SVertexLit *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderLine( const SVertexNormal *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	void			RenderLine( const SVertex2Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount );
	
	void			Render3DLine( const Core::IVector3f &vStart, const Core::IVector3f &vStop, const IColor &cColor );
	
	// Static Buffer
	AI_RESULT		CreateStaticBuffer( E_VERTEX_TYPE evt, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount, u32 nSkinID, u32 *nSID );
	AI_RESULT		Render( u32 nSID );
	void			DeleteStaticBuffer( u32 nSID );

	// Dynamic Buffer
	void			InvalidateStates( void );
	AI_RESULT		Render( E_VERTEX_TYPE evt, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount, u32 nSkinID );
	AI_RESULT		FlushAll( void );
	AI_RESULT		Flush( E_VERTEX_TYPE evt );

	IRenderDevice	*GetParentRenderDevice( void ) { return m_pRenderDevice; }
	void			SetActiveCache( u32 nCacheID ) { m_nActiveCache = nCacheID; }
	u32				GetActiveCache( void ) { return m_nActiveCache; }
	void			SetActiveSB( u32 nSBID ) { m_nActiveSB = nSBID; }
	u32				GetActiveSB( void ) { return m_nActiveSB; }
	
	// for the subsystem of the render manager
	ul32			GetFVF( E_VERTEX_TYPE evtType );
	u32				GetStride( E_VERTEX_TYPE evtType );

protected:
	AI_RESULT		RenderTriangleType( E_VERTEX_TYPE evtType, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount );
	AI_RESULT		RenderLineType( E_VERTEX_TYPE evtType, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount );
	void			SetVertexMode( E_VERTEX_TYPE evtType );

	LPDIRECT3DDEVICE9		m_pDevice;
	IRenderDevice			*m_pRenderDevice;
	E_VERTEX_TYPE			m_LastVertexMode;
	u32						m_nActiveCache;
	u32						m_nActiveSB;
	u32						m_nNumSB;

	nVCache					*m_pVStandard[MAX_CACHE];
	nVCache					*m_pVPos[MAX_CACHE];
	nVCache					*m_pVLit[MAX_CACHE];
	nVCache					*m_pVNormal[MAX_CACHE];
	nVCache					*m_pV2Tex[MAX_CACHE];
	nVCache					*m_pV3Tex[MAX_CACHE];
	nVCache					*m_pVTangent[MAX_CACHE];
	Core::Array<nSBuffer*>	m_pSBuffer;
};

/*************************************
  Render Manager Creation
 *************************************/
IRenderMan	*createRenderMan( LPDIRECT3DDEVICE9 pDevice, IRenderDevice *pRenderDevice );

}	// namespace Video
}	// namespace Ahmad

#endif	// _IRENDERMAN_H_