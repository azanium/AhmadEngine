/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nSBuffer.h"
#include "nRenderMan.h"
#include "ISkinManager.h"
#include "nD3DDriver.h"

namespace Ahmad
{
namespace Video
{

nSBuffer::nSBuffer( E_VERTEX_TYPE evt,
				    LPDIRECT3DDEVICE9 pDevice, nRenderMan *pParent, ISkinManager *pSkinMan,
					const void *pVerts, u32 nVertsCount, const u16 *pIndis, u32 nIndisCount,
					u32 nSID, u32 nSkinID)
{
	u16		*tmpIndis;
	void	*tmpVerts;

	m_pDevice		= pDevice;
	m_pRenderMan	= pParent;
	m_pSkinMan		= pSkinMan;
	m_FVF			= m_pRenderMan->GetFVF(evt);
	m_nStride		= m_pRenderMan->GetStride(evt);
	m_nSkinID		= nSkinID;
	m_nVertsCount	= nVertsCount;
	m_nIndisCount	= nIndisCount;
	m_bReady		= true;
	m_nTrisCount	= nIndisCount/3;
	m_nSID			= nSID;
	m_Skin			= m_pSkinMan->GetSkin(nSkinID);
	m_evt			= evt;
	
	m_pVB = NULL;
	m_pIB = NULL;

	HRESULT	hr;

	if (nIndisCount>0)
	{
		hr = m_pDevice->CreateIndexBuffer(m_nIndisCount*sizeof(u16),
										  D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
										  D3DPOOL_DEFAULT, &m_pIB, NULL);
		if (SUCCEEDED(hr))
		{
			if (SUCCEEDED(m_pIB->Lock(0, 0, (void **)(&tmpIndis), 0)))
			{
				memcpy(tmpIndis, pIndis, m_nIndisCount*sizeof(u16));
				m_pIB->Unlock();
			}
			else m_bReady = false;
			
		} else m_bReady = false;
		
		m_bIndis = true;

	} else m_bIndis = false;

	hr = m_pDevice->CreateVertexBuffer(m_nVertsCount*m_nStride,
									   D3DUSAGE_WRITEONLY, m_FVF,
									   D3DPOOL_DEFAULT, &m_pVB, NULL);
	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(m_pVB->Lock(0, 0, (void **)(&tmpVerts), 0)))
		{
			memcpy(tmpVerts, pVerts, m_nVertsCount*m_nStride);
			m_pVB->Unlock();
		}
		else m_bReady = false;
	} else m_bReady = false;	
}

nSBuffer::~nSBuffer( void )
{
	SAFE_RELEASE(m_pIB);

	SAFE_RELEASE(m_pVB);
}

D3DTEXTUREOP nSBuffer::GetTOP(E_TEXTURE_OP op)
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

HRESULT nSBuffer::Render( void )
{
	if (!m_bReady) return E_FAIL;
	
	if (m_bIndis) m_pDevice->SetIndices(m_pIB);
	
	if (m_pRenderMan->GetActiveSB() != m_nSID)
	{
		if (!m_pRenderMan->GetParentRenderDevice()->UsingShaders())
			m_pDevice->SetFVF(m_FVF);

		m_pDevice->SetStreamSource(0, m_pVB, 0, m_nStride);
		m_pRenderMan->SetActiveSB( m_nSID );
	}

	if (m_pRenderMan->GetParentRenderDevice()->GetActiveSkin() != m_nSkinID)
	{
		IMaterial *mat = &m_pSkinMan->GetMaterial(m_Skin.Material);
		if (!m_pRenderMan->GetParentRenderDevice()->UsingShaders())
		{
			D3DMATERIAL9	d3dmat = {
				mat->Diffuse.GetRedf(), mat->Diffuse.GetGreenf(), mat->Diffuse.GetBluef(), mat->Diffuse.GetAlphaf(),
				mat->Ambient.GetRedf(), mat->Ambient.GetGreenf(), mat->Ambient.GetBluef(), mat->Ambient.GetAlphaf(),
				mat->Specular.GetRedf(), mat->Specular.GetGreenf(), mat->Specular.GetBluef(), mat->Specular.GetAlphaf(),
				mat->Emissive.GetRedf(), mat->Emissive.GetGreenf(), mat->Emissive.GetBluef(), mat->Emissive.GetAlphaf(),
				mat->fShininess };
		
			m_pDevice->SetMaterial(&d3dmat);
		}
		else
		{
			// shader constant
			IColorf color;
			color = mat->Ambient;
			m_pRenderMan->GetParentRenderDevice()->SetPixelShaderConstant(1, 1, (f32 *)&color);
			color = mat->Diffuse;
			m_pRenderMan->GetParentRenderDevice()->SetPixelShaderConstant(2, 1, (f32 *)&color);
			color = mat->Emissive;
			m_pRenderMan->GetParentRenderDevice()->SetPixelShaderConstant(3, 1, (f32 *)&color);
			color = mat->Specular;
			m_pRenderMan->GetParentRenderDevice()->SetPixelShaderConstant(4, 1, (f32 *)&color);
		}
		
		u32 nTex;

		for (nTex=0; nTex<MAX_TEXTURE; nTex++)
			if (m_Skin.Texture[nTex] != MAX_ID)
			{
				m_pRenderMan->GetParentRenderDevice()->SetTexture(nTex, m_pSkinMan->GetTexture(m_Skin.Texture[nTex]));

				m_pDevice->SetTextureStageState(nTex, D3DTSS_TEXCOORDINDEX, 0);
                m_pDevice->SetTextureStageState(nTex, D3DTSS_COLORARG1, D3DTA_TEXTURE);
				m_pDevice->SetTextureStageState(nTex, D3DTSS_COLORARG2, D3DTA_CURRENT);
				m_pDevice->SetTextureStageState(nTex, D3DTSS_COLOROP, 
					GetTOP(m_pRenderMan->GetParentRenderDevice()->GetTextureOperation(nTex)));
			}
			else break;

			m_pDevice->SetTexture(nTex, NULL);
			m_pRenderMan->GetParentRenderDevice()->SetActiveSkin(m_nSkinID);
	}	
	
	HRESULT hr;

	if (m_bIndis)
	{
		if (m_pRenderMan->GetParentRenderDevice()->GetShadeMode() == ESM_POINTSPRITE)
		{
			hr = m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_nVertsCount);
		}
		else if (m_pRenderMan->GetParentRenderDevice()->GetShadeMode() == ESM_LINES)
		{
			hr = m_pDevice->DrawIndexedPrimitive(D3DPT_LINESTRIP, 0, 0, m_nVertsCount, 0, m_nVertsCount);
		}
		else
		{
			hr = m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,
										     m_nVertsCount, 0, m_nTrisCount);
		}
	}
	else
	{
		if (m_pRenderMan->GetParentRenderDevice()->GetShadeMode() == ESM_POINTSPRITE)
		{
			hr = m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, m_nVertsCount);
		}
		else if (m_pRenderMan->GetParentRenderDevice()->GetShadeMode() == ESM_LINES)
		{
			hr = m_pDevice->DrawPrimitive(D3DPT_LINESTRIP, m_nVertsCount, m_nVertsCount);
		}
		else
		{
			hr = m_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_nTrisCount);
		}
	}

	return hr;
}

E_VERTEX_TYPE nSBuffer::GetVertexType( void )
{
	return m_evt;
}

}	// namespace Video
}	// namespace Ahmad