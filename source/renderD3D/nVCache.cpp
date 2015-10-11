#include "nVCache.h"
#include "nRenderMan.h"
#include "nSkinManager.h"

namespace Ahmad
{
namespace Video
{

nVCache::nVCache( nRenderMan *pParent, LPDIRECT3DDEVICE9 pDevice, ISkinManager *pSkinMan,
				  u32 nVertsMax, u32 nIndisMax, u32 nStride, ul32 FVF, u32 nCacheID )
{
	m_pParent	= pParent;
	m_pDevice	= pDevice;
	m_pSkinMan	= pSkinMan;
	m_nMaxVerts = nVertsMax;
	m_nMaxIndis = nIndisMax;
	m_nNumVerts = 0;
	m_nNumIndis = 0;
	m_nStride	= nStride;
	m_nID		= nCacheID;
	m_FVF		= FVF;
	m_pTex2D	= NULL;
	
	m_pVB		= NULL;
	m_pIB		= NULL;

	memset(&m_Skin, MAX_ID, sizeof(m_Skin));
	m_nSkinID = MAX_ID;

	HRESULT	hr;
	DWORD	dwFlags = D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC;

	hr = m_pDevice->CreateVertexBuffer(m_nStride*m_nMaxVerts, 
									   dwFlags, 
									   FVF,
									   D3DPOOL_DEFAULT,
									   &m_pVB, 0);

	if (FAILED(hr)) m_pVB = NULL;

	hr = m_pDevice->CreateIndexBuffer(m_nMaxIndis*sizeof(u16),
									  dwFlags,
									  D3DFMT_INDEX16,
									  D3DPOOL_DEFAULT, &m_pIB, 0);

	if (FAILED(hr)) m_pIB = NULL;
}

nVCache::~nVCache( void )
{
	SAFE_RELEASE(m_pVB);
	
	SAFE_RELEASE(m_pIB);
}

HRESULT nVCache::Add(const void *pVertices, u32 nVerts, const u16 *pIndis, u32 nIndis)
{
	DWORD	dwFlags;
	u32		nPosV;
	u32		nPosI;
	u32		nSizeI;
	u32		nSizeV = m_nStride*nVerts;
	BYTE	*tmpVerts = NULL;
	u16		*tmpIndis = NULL;

	if (nVerts>m_nMaxVerts || nIndis>m_nMaxIndis)
		return AI_BUFFERFULL;

	if (nVerts+m_nNumVerts>m_nMaxIndis || nIndis+m_nNumIndis>m_nMaxIndis)
		Render();

	if (m_nNumVerts>0)
	{
		nPosV = m_nStride*m_nNumVerts;
		nPosI = sizeof(u16)*m_nNumIndis;
		dwFlags = D3DLOCK_NOOVERWRITE;
	}
	else
	{
		nPosV = nPosI = 0;
		dwFlags = D3DLOCK_DISCARD;
	}

	if (!pIndis)
		nSizeI = nVerts*sizeof(u16);
	else
		nSizeI = nIndis*sizeof(u16);

	if (FAILED(m_pVB->Lock(nPosV, nSizeV, (void **)&tmpVerts, dwFlags)))
		return E_FAIL;
	if (FAILED(m_pIB->Lock(nPosI, nSizeI, (void **)&tmpIndis, dwFlags)))
	{
		m_pVB->Unlock();
		return E_FAIL;
	}

	memcpy(tmpVerts, pVertices, nSizeV);

	u32 nBase = m_nNumVerts;
	if (!pIndis) nIndis = nVerts;
	
	m_nNumVerts += nVerts;
	
	for (u32 i=0; i<nIndis; i++)
	{
		if (!pIndis)
			tmpIndis[i] = nBase+i;
		else

			tmpIndis[i] = nBase+pIndis[i];
		m_nNumIndis++;
	}
	
	m_pVB->Unlock();
	m_pIB->Unlock();

	return AI_OK;	
}

AI_RESULT nVCache::SetSkin( u32 nSkinID )
{
	if (GetSkin() != nSkinID)
	{
		if (!Empty()) Render();

		ISkin *skin = &m_pSkinMan->GetSkin(nSkinID);
		memcpy(&m_Skin, skin, sizeof(m_Skin));
		m_nSkinID = nSkinID;

		m_pParent->InvalidateStates();
	}
	return AI_OK;
}

D3DTEXTUREOP nVCache::GetTOP(E_TEXTURE_OP op)
{
	switch (op)
	{
		case ETO_NONE:
			return D3DTOP_DISABLE;
			break;

		case ETO_GLOWMAP:
			return D3DTOP_ADDSIGNED;
			break;

		case ETO_DARKMAP:
			return D3DTOP_MODULATE;
			break;
	}

	return D3DTOP_DISABLE;
}

HRESULT nVCache::Render( void )
{
	if (Empty()) return S_OK;
	
	if (m_pParent->GetActiveCache() != m_nID)
	{
		if (!m_pParent->GetParentRenderDevice()->UsingShaders())
			m_pDevice->SetFVF(m_FVF);

		m_pDevice->SetStreamSource(0, m_pVB, 0, m_nStride);
		m_pDevice->SetIndices(m_pIB);
		m_pParent->SetActiveCache(m_nID);
	}

	if (m_pParent->GetParentRenderDevice()->GetActiveSkin() != m_nSkinID)
	{
		IMaterial *mat = &m_pSkinMan->GetMaterial(m_Skin.Material);
		if (!m_pParent->GetParentRenderDevice()->UsingShaders())
		{
			D3DMATERIAL9 mat9 = { 
				mat->Diffuse.GetRedf(), mat->Diffuse.GetGreenf(), mat->Diffuse.GetBluef(), mat->Diffuse.GetAlphaf(),
				mat->Ambient.GetRedf(), mat->Ambient.GetGreenf(), mat->Ambient.GetBluef(), mat->Ambient.GetAlphaf(),
				mat->Specular.GetRedf(), mat->Specular.GetGreenf(), mat->Specular.GetBluef(), mat->Specular.GetAlphaf(),
				mat->Emissive.GetRedf(), mat->Emissive.GetGreenf(), mat->Emissive.GetBluef(), mat->Emissive.GetAlphaf(),
				mat->fShininess };
		
				// for fixed function pipeline only
				m_pDevice->SetMaterial( &mat9 );
		}
		else
		{
			IColorf color;
			color = mat->Ambient;
			m_pParent->GetParentRenderDevice()->SetPixelShaderConstant(1, 1, (f32 *)&color);
			color = mat->Diffuse;
			m_pParent->GetParentRenderDevice()->SetPixelShaderConstant(2, 1, (f32 *)&color);
			color = mat->Emissive;
			m_pParent->GetParentRenderDevice()->SetPixelShaderConstant(3, 1, (f32 *)&color);
			color = mat->Specular;
			m_pParent->GetParentRenderDevice()->SetPixelShaderConstant(4, 1, (f32 *)&color);
		}

		u32 nTex;
		
		for (nTex=0; nTex<MAX_TEXTURE; nTex++)
		{
			if (m_Skin.Texture[nTex] != MAX_ID)
			{
				ITexture *tex = m_pSkinMan->GetTexture(m_Skin.Texture[nTex]);
				m_pParent->GetParentRenderDevice()->SetTexture(nTex, tex);
				
				m_pDevice->SetTextureStageState(nTex, D3DTSS_TEXCOORDINDEX, 0);
				m_pDevice->SetTextureStageState(nTex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				m_pDevice->SetTextureStageState(nTex, D3DTSS_COLORARG2, D3DTA_CURRENT);
				m_pDevice->SetTextureStageState(nTex, D3DTSS_COLOROP,
				GetTOP(m_pParent->GetParentRenderDevice()->GetTextureOperation(nTex)));
			}
			else break;
		}
		m_pParent->GetParentRenderDevice()->SetTexture(nTex, NULL);

		m_pParent->GetParentRenderDevice()->SetActiveSkin(m_nSkinID);
	}

	HRESULT hr;

	if (m_pParent->GetParentRenderDevice()->GetShadeMode() == ESM_POINTSPRITE)
	{
		hr = m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_nNumVerts);
	}
	else if (m_pParent->GetParentRenderDevice()->GetShadeMode() == ESM_LINES)
	{
		hr = m_pDevice->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_nNumVerts, 0, m_nNumVerts);
	}
	else
	{
        hr = m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
										     m_nNumVerts, 0, m_nNumIndis/3);
	}

	if (FAILED(hr)) return E_FAIL;

	m_nNumVerts = 0;
	m_nNumIndis = 0;

	return S_OK;
}

}	// namespace Video
}	// namespace Ahmad
