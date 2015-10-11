/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NSBUFFER_H_
#define _NSBUFFER_H_

#include <d3dx9.h>
#include "Types.h"
#include "IBuyut.h"
#include "SVertex.h"
#include "ISkin.h"
#include "RenderState.h"

namespace Ahmad
{
namespace Video
{

class nRenderMan;
class ISkinManager;

class nSBuffer: public IBuyut
{
public:
	nSBuffer( E_VERTEX_TYPE evt,
		     LPDIRECT3DDEVICE9 pDevice, nRenderMan *pParent, ISkinManager *pSkinMan,
			 const void *pVerts, u32 nVertsCount, const u16 *pIndis, u32 nIndisCount,
			 u32 nSID, u32 nSkinID );
	~nSBuffer( void );

	HRESULT			Render( void );

	E_VERTEX_TYPE	GetVertexType( void );

protected:
	D3DTEXTUREOP			GetTOP( E_TEXTURE_OP op );

	u32						m_nSID;
	u32						m_nStride;
	u32						m_nVertsCount;
	u32						m_nIndisCount;
	u32						m_nTrisCount;
	bool					m_bIndis;
	DWORD					m_FVF;
	u32						m_nSkinID;
	LPDIRECT3DVERTEXBUFFER9	m_pVB;
	LPDIRECT3DINDEXBUFFER9	m_pIB;
	LPDIRECT3DDEVICE9		m_pDevice;
	nRenderMan				*m_pRenderMan;
	ISkinManager			*m_pSkinMan;
	ISkin					m_Skin;
	bool					m_bReady;
	E_VERTEX_TYPE			m_evt;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _NSBUFFER_H_