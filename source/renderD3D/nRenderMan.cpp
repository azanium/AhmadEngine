/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include <d3dx9.h>
#include "nRenderMan.h"
#include "IMatrix.h"
#include "nVCache.h"
#include "nSBuffer.h"

namespace Ahmad
{
namespace Video
{

nRenderMan::nRenderMan( LPDIRECT3DDEVICE9 pDevice, IRenderDevice *pRenderDevice ):
m_pDevice(pDevice), m_pRenderDevice(pRenderDevice), m_LastVertexMode((E_VERTEX_TYPE)-1),
m_nActiveCache(MAX_ID) 
{ 
	// Maximum vertex and indices for the vertex cache object
	u32 nVertsMax = 8192;
	u32 nIndisMax = 8192;

	// ID for the vertex cache object
	u32 nID = 1;

	// Number of static buffer object created
	m_nNumSB = 0;

	// The selected vertex cache object
	m_nActiveCache = MAX_ID;

	// The selected static buffer object
	m_nActiveSB = MAX_ID;

	// Create the cache for every vertex type posibility
	for (u32 i=0; i<MAX_CACHE; i++)
	{
		// The cache for the standard vertex type
		m_pVStandard[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
									  nVertsMax, nIndisMax, sizeof(SVertex), 
									  GetFVF(EVT_STANDARD), nID++);

		// The cache for the position vertex type
		m_pVPos[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
				    			 nVertsMax, nIndisMax, sizeof(SVertexPos), 
								 GetFVF(EVT_VERTEXPOS), nID++);

		// The cache for the lit vertex type
		m_pVLit[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
								 nVertsMax, nIndisMax, sizeof(SVertexLit), 
								 GetFVF(EVT_VERTEXLIT), nID++);

		// The cache for the normal vertex type
		m_pVNormal[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
									nVertsMax, nIndisMax, sizeof(SVertexNormal), 
									GetFVF(EVT_VERTEXNORMAL), nID++);

		// The cache for the two texture vertex type
		m_pV2Tex[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
								  nVertsMax, nIndisMax, sizeof(SVertex2Tex), 
								  GetFVF(EVT_VERTEX2TEX), nID++);

		// The cache for the three texture vertex type
		m_pV3Tex[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
				 				  nVertsMax, nIndisMax, sizeof(SVertex3Tex), 
								  GetFVF(EVT_VERTEX3TEX), nID++);

		// The cache for the tangent bumpmap vertex type
		m_pVTangent[i] = new nVCache(this, pDevice, pRenderDevice->GetSkinManager(),
									 nVertsMax, nIndisMax, sizeof(SVertexTangent), 
									 GetFVF(EVT_VERTEXTANGENT), nID++);
	}
}

nRenderMan::~nRenderMan( void )
{
	// Throw any caches object
	for (u32 i=0; i<MAX_CACHE; i++)
	{
		SAFE_THROW(m_pVStandard[i]);
		SAFE_THROW(m_pVPos[i]);
		SAFE_THROW(m_pVLit[i]);
		SAFE_THROW(m_pVNormal[i]);
		SAFE_THROW(m_pV2Tex[i]);
		SAFE_THROW(m_pV3Tex[i]);
		SAFE_THROW(m_pVTangent[i]);
	}

	// Throw any static buffers
	for (u32 i=0; i<m_pSBuffer.Length(); i++)
		DeleteStaticBuffer(i);
	m_pSBuffer.Clear();
}

void nRenderMan::SetVertexMode( E_VERTEX_TYPE evtType )
{
	// Is the vertex already active?
	if (evtType != m_LastVertexMode)
	{
		m_LastVertexMode = evtType;

		// FVF constant Jangan dipindahkan ke dalam SVertex.h supaya tetap independent
		switch (evtType)
		{
			case EVT_STANDARD:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
				break;
			
			case EVT_VERTEXPOS:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
				break;
			
			case EVT_VERTEXLIT:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
				break;
			
			case EVT_VERTEXNORMAL:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
				break;

			case EVT_VERTEX2TEX:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
				break;

			case EVT_VERTEX3TEX:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3);
				break;

			case EVT_VERTEXTANGENT:
				m_pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0));
				break;

			default:
				return;
		}	// switch
	}	// if
}

ul32 nRenderMan::GetFVF( E_VERTEX_TYPE evtType )
{
	// FVF constant Jangan dipindahkan ke dalam SVertex.h supaya tetap independent
	switch (evtType)
	{
		case EVT_STANDARD:
			return (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			break;
		
		case EVT_VERTEXPOS:
			return (D3DFVF_XYZ | D3DFVF_TEX1);
			break;
			
		case EVT_VERTEXLIT:
			return (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
			break;
			
		case EVT_VERTEXNORMAL:
			return (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
			break;

		case EVT_VERTEX2TEX:
			return (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
			break;

		case EVT_VERTEX3TEX:
			return (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3);
			break;

		case EVT_VERTEXTANGENT:
			return (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0));
			break;

		default:
			return 0;
	}	// switch
}

//! The stride of the vertex type
u32 nRenderMan::GetStride( E_VERTEX_TYPE evtType )
{
	switch (evtType)
	{
		case EVT_STANDARD:
			return sizeof(SVertex);
			break;
		
		case EVT_VERTEXPOS:
			return sizeof(SVertexPos);
			break;
			
		case EVT_VERTEXLIT:
			return sizeof(SVertexLit);
			break;
			
		case EVT_VERTEXNORMAL:
			return sizeof(SVertexNormal);
			break;

		case EVT_VERTEX2TEX:
			return sizeof(SVertex2Tex);
			break;

		case EVT_VERTEX3TEX:
			return sizeof(SVertex3Tex);
			break;

		case EVT_VERTEXTANGENT:
			return sizeof(SVertexTangent);
			break;

		default:
			return 0;
	}	// switch
}


/***********************************************
  Master function to render triangles
 ***********************************************/

AI_RESULT nRenderMan::RenderTriangleType( E_VERTEX_TYPE evtType,
										  const void *pVertices, 
										  u32 nVertexCount, 
										  const u16 *pIndices,
										  u32 nTriangleCount )
{
	AI_RESULT	ar = AI_FAIL;
	if ((m_pDevice) && (m_pRenderDevice->IsSceneRunning()))
	{
		SetVertexMode(evtType);

		u32 nStride = 0;
		switch (evtType)
		{
		case EVT_STANDARD:
			nStride = sizeof(SVertex);
			break;

		case EVT_VERTEXPOS:
			nStride = sizeof(SVertexPos);
			break;

		case EVT_VERTEXLIT:
			nStride = sizeof(SVertexLit);
			break;

		case EVT_VERTEXNORMAL:
			nStride = sizeof(SVertexNormal);
			break;

		case EVT_VERTEX2TEX:
			nStride = sizeof(SVertex2Tex);
			break;

		case EVT_VERTEX3TEX:
			nStride = sizeof(SVertex3Tex);
			break;
		
		case EVT_VERTEXTANGENT:
			nStride = sizeof(SVertexTangent);
			break;

		default:
			return AI_FAIL;

		}	// switch
            
		ar = m_pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, nVertexCount, 
			nTriangleCount, pIndices, D3DFMT_INDEX16, pVertices, nStride);
		
		if (FAILED(ar))
		{ 
			// TODO: Add some log here
		}

	}

	return ar;
}

void nRenderMan::RenderTriangle( const SVertex *pVertices, 
								 u32 nVertexCount, 
								 const u16 *pIndices,
								 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_STANDARD, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderTriangle( const SVertexPos *pVertices, 
								 u32 nVertexCount, 
								 const u16 *pIndices,
								 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_VERTEXPOS, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderTriangle( const SVertexLit *pVertices, 
								 u32 nVertexCount, 
								 const u16 *pIndices,
								 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_VERTEXLIT, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderTriangle( const SVertexNormal *pVertices, 
									 u32 nVertexCount, 
									 const u16 *pIndices,
									 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_VERTEXNORMAL, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderTriangle( const SVertex2Tex *pVertices, 
									 u32 nVertexCount, 
									 const u16 *pIndices,
									 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_VERTEX2TEX, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderTriangle( const SVertex3Tex *pVertices, 
								 u32 nVertexCount, 
								 const u16 *pIndices,
								 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_VERTEX3TEX, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderTriangle( const SVertexTangent *pVertices, 
								 u32 nVertexCount, 
								 const u16 *pIndices,
								 u32 nTriangleCount )
{
	if (FAILED(RenderTriangleType( EVT_VERTEXTANGENT, pVertices, nVertexCount, pIndices, nTriangleCount )))
	{
		// TODO: Put some log here
	}
}

/***********************************************
  Master function for Line rendering 
 ***********************************************/

AI_RESULT nRenderMan::RenderLineType( E_VERTEX_TYPE evtType,
									  const void *pVertices, 
									  u32 nVertexCount, 
									  const u16 *pIndices,
									  u32 nTriangleCount )
{
	AI_RESULT	ar;
	if (m_pDevice)
	{
		SetVertexMode(evtType);

		u32 nStride = 0;
		switch (evtType)
		{
		case EVT_STANDARD:
			nStride = sizeof(SVertex);
			break;

		case EVT_VERTEXPOS:
			nStride = sizeof(SVertexPos);
			break;

		case EVT_VERTEXLIT:
			nStride = sizeof(SVertexLit);
			break;

		case EVT_VERTEXNORMAL:
			nStride = sizeof(SVertexNormal);
			break;

		case EVT_VERTEX2TEX:
			nStride = sizeof(SVertex2Tex);
			break;

		case EVT_VERTEX3TEX:
			nStride = sizeof(SVertex3Tex);
			break;

		case EVT_VERTEXTANGENT:
			nStride = sizeof(SVertexTangent);
			break;
			
		default:
			return AI_FAIL;

		}	// switch

		ar = m_pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, nVertexCount, 
			nTriangleCount, pIndices, D3DFMT_INDEX16, pVertices, nStride);
	}

	if (FAILED(ar))
	{
		// TODO: Add some log here
	}

	return ar;
}

void nRenderMan::RenderLine( const SVertex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount )
{
	if (FAILED(RenderLineType(EVT_STANDARD, pVertices, nVertexCount, pIndices, nTriangleCount)))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderLine( const SVertexPos *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount )
{
	if (FAILED(RenderLineType(EVT_VERTEXPOS, pVertices, nVertexCount, pIndices, nTriangleCount)))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderLine( const SVertexLit *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount )
{
	if (FAILED(RenderLineType(EVT_VERTEXLIT, pVertices, nVertexCount, pIndices, nTriangleCount)))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderLine( const SVertexNormal *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount )
{
	if (FAILED(RenderLineType(EVT_VERTEXLIT, pVertices, nVertexCount, pIndices, nTriangleCount)))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::RenderLine( const SVertex2Tex *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nTriangleCount )
{
	if (FAILED(RenderLineType(EVT_VERTEXLIT, pVertices, nVertexCount, pIndices, nTriangleCount)))
	{
		// TODO: Put some log here
	}
}

void nRenderMan::Render3DLine( const Core::IVector3f &vStart, const Core::IVector3f &vStop, const IColor &cColor )
{
	AI_RESULT	ar;

	if ((!m_pDevice) || (!m_pRenderDevice->IsSceneRunning())) return;

	SetVertexMode(EVT_VERTEXLIT);
	SVertexLit	v[2];
	v[0].Pos = vStart;
	v[1].Pos = vStop;
	v[0].Color = cColor;
	v[1].Color = cColor;
	ar = m_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 1, v, sizeof(SVertexLit));

	if (FAILED(ar))
	{
		// TODO: Add some log here
	}
}

AI_RESULT nRenderMan::CreateStaticBuffer( E_VERTEX_TYPE evt, const void *pVertices, 
										  u32 nVertexCount, const u16 *pIndices, 
										  u32 nIndisCount, u32 nSkinID, u32 *nSID )
{
	nSBuffer *sb = new nSBuffer(evt, m_pDevice, this, m_pRenderDevice->GetSkinManager(),
								pVertices, nVertexCount, pIndices, nIndisCount, m_nNumSB, nSkinID);

	m_pSBuffer.AddLast(sb);
	*nSID = m_nNumSB;
	m_nNumSB = m_pSBuffer.Length();
	
	return AI_OK;	
}

AI_RESULT nRenderMan::Render( u32 nSID )
{
	if (nSID >= m_nNumSB) return AI_FAIL;

	m_nActiveCache = MAX_ID;

	if (m_pRenderDevice->UsingShaders() && (m_pRenderDevice->VertexShaderCount() <= 1))
		m_pRenderDevice->ActivateVertexShader(0, m_pSBuffer[nSID]->GetVertexType());

	if (FAILED(m_pSBuffer[nSID]->Render())) return AI_FAIL;

	return AI_OK;
}

void nRenderMan::DeleteStaticBuffer( u32 nSID )
{
	if (nSID < m_nNumSB)
	{
		// Delete the object of the static buffer
		SAFE_DELETE(m_pSBuffer[nSID]);

		// Delete the reference in the array
		m_pSBuffer.Delete(nSID);

		// To remove the error when the user call render(), set the m_nNumSB to 
		// the actual length of static buffer array.
		m_nNumSB = m_pSBuffer.Length();
	}
}

void nRenderMan::InvalidateStates( void )
{
	// Invalidate the skin id
	m_pRenderDevice->SetActiveSkin(MAX_ID);

	// Invalidate the active cache
	SetActiveCache(MAX_ID);

	// Invalidate the static buffer
	SetActiveSB(MAX_ID);
}

AI_RESULT nRenderMan::Render( E_VERTEX_TYPE evt, const void *pVertices, u32 nVertexCount, const u16 *pIndices, u32 nIndisCount, u32 nSkinID )
{
	nVCache	**cache			= NULL,
			 *cacheEmpty	= NULL,
			 *cacheFullest	= NULL;

	m_nActiveSB = MAX_ID;

	switch (evt)
	{
		case EVT_STANDARD:
			cache = m_pVStandard;
			break;

		case EVT_VERTEXPOS:
			cache = m_pVPos;
			break;

		case EVT_VERTEXLIT:
			cache = m_pVLit;
			break;

		case EVT_VERTEXNORMAL:
			cache = m_pVNormal;
			break;

		case EVT_VERTEX2TEX:
			cache = m_pV2Tex;
			break;

		case EVT_VERTEX3TEX:
			cache = m_pV3Tex;
			break;

		case EVT_VERTEXTANGENT:
			cache = m_pVTangent;
			break;
	}
	
	cacheFullest = cache[0];

	if (m_pRenderDevice->UsingShaders() && (m_pRenderDevice->VertexShaderCount() <= 1))
		m_pRenderDevice->ActivateVertexShader(0, evt);

	for (u32 i=0; i<MAX_CACHE; i++)
	{
		if (cache[i]->GetSkin() == nSkinID)
			return cache[i]->Add(pVertices, nVertexCount, pIndices, nIndisCount);

		if (cache[i]->Empty())
			cacheEmpty = cache[i];

		if (cache[i]->VerticesCount() > cacheFullest->VerticesCount())
			cacheFullest = cache[i];
	}

	if (cacheEmpty)
	{
		cacheEmpty->SetSkin(nSkinID);
		return cacheEmpty->Add(pVertices, nVertexCount, pIndices, nIndisCount);
	}

	cacheFullest->Render();
	cacheFullest->SetSkin(nSkinID);
	return cacheFullest->Add(pVertices, nVertexCount, pIndices, nIndisCount);
}

AI_RESULT nRenderMan::FlushAll( void )
{	
	if (!m_pRenderDevice->IsSceneRunning()) return AI_OK;

	for (u32 i=0; i<MAX_CACHE; i++)
	{
		if (!m_pVStandard[i]->Empty())
			if (FAILED(m_pVStandard[i]->Render()))
				return AI_FAIL;

		if (!m_pVPos[i]->Empty())
			if (FAILED(m_pVPos[i]->Render()))
				return AI_FAIL;

		if (!m_pVLit[i]->Empty())
			if (FAILED(m_pVLit[i]->Render()))
				return AI_FAIL;

		if (!m_pVNormal[i]->Empty())
			if (FAILED(m_pVNormal[i]->Render()))
				return AI_FAIL;

		if (!m_pV2Tex[i]->Empty())
			if (FAILED(m_pV2Tex[i]->Render()))
				return AI_FAIL;

		if (!m_pV3Tex[i]->Empty())
			if (FAILED(m_pV3Tex[i]->Render()))
				return AI_FAIL;

		if (!m_pVTangent[i]->Empty())
			if (FAILED(m_pVTangent[i]->Render()))
				return AI_FAIL;
	}

	return AI_OK;
}

AI_RESULT nRenderMan::Flush( E_VERTEX_TYPE evt )
{
	nVCache	**cache = NULL;

	switch (evt)
	{
		case EVT_STANDARD:
			cache = m_pVStandard;
			break;

		case EVT_VERTEXPOS:
			cache = m_pVPos;
			break;

		case EVT_VERTEXLIT:
			cache = m_pVLit;
			break;

		case EVT_VERTEXNORMAL:
			cache = m_pVNormal;
			break;

		case EVT_VERTEX2TEX:
			cache = m_pV2Tex;
			break;

		case EVT_VERTEX3TEX:
			cache = m_pV3Tex;
			break;

		case EVT_VERTEXTANGENT:
			cache = m_pVTangent;
			break;
	}

	// Render the selected cache
	for (u32 i=0; i<MAX_CACHE; i++)
	{
		if (!cache[i]->Empty())
			if (FAILED(cache[i]->Render()))
				return AI_FAIL;
	}

	return AI_OK;
}

/*************************************
  Render Manager Creation
 *************************************/

IRenderMan	*createRenderMan( LPDIRECT3DDEVICE9 pDevice, IRenderDevice *pRenderDevice )
{
	return new nRenderMan( pDevice, pRenderDevice );
}

}	// namespace Video
}	// namespace Ahmad