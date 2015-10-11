/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NVCACHE_H_
#define _NVCACHE_H_

#include <d3dx9.h>
#include "Types.h"
#include "IBuyut.h"
#include "ISkin.h"
#include "RenderState.h"
#include "ITexture.h"

namespace Ahmad
{
namespace Video
{

// Parent for this vertex cache
class nRenderMan;
class ISkinManager;

// This is the vertex cache object for the Render Manager
// This implementation only for the Direct3D and hidden from the user
class nVCache: public IBuyut
{
public:
	nVCache( nRenderMan *pParent, LPDIRECT3DDEVICE9 pDevice, ISkinManager *pSkinMan,
			 u32 nVertsMax, u32 nIndisMax, u32 nStride, ul32 FVF, u32 nCacheID );
	~nVCache( void );

	u32				GetSkin( void ) { return m_nSkinID; }
	bool			UsesSkin( u32 nSkinID ) { return m_nSkinID == nSkinID; }
	u32				GetCacheID( void ) { return m_nID; }
	AI_RESULT		SetSkin( u32 nSkinID );
	HRESULT			Add(const void *pVertices, u32 nVerts, const u16 *pIndis, u32 nIndis);
	bool			Empty( void ) { return m_nNumVerts == 0; }
	u32				VerticesCount( void ) { return m_nNumVerts; }

	HRESULT			Render( void );

protected:
	D3DTEXTUREOP				GetTOP( E_TEXTURE_OP op );

	u32							m_nMaxVerts;
	u32							m_nMaxIndis;
	u32							m_nNumVerts;
	u32							m_nNumIndis;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	LPDIRECT3DDEVICE9			m_pDevice;
	nRenderMan					*m_pParent;
	ISkinManager				*m_pSkinMan;
	ul32						m_FVF;
	u32							m_nStride;
	u32							m_nID;
	u32							m_nSkinID;
	ISkin						m_Skin;
	ITexture					*m_pTex2D;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _NVCACHE_H_