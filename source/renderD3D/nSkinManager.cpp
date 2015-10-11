/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/


#include "nSkinManager.h"

namespace Ahmad
{
namespace Video
{

nSkinManager::nSkinManager( nD3DDriver *pParent )
{
	m_pParent		= pParent;
	//m_pSkins		= NULL;
	//m_pMaterials	= NULL;
	//m_pTextures		= NULL;
	m_nNumSkins		= 0;
	m_nNumMaterials	= 0;
	m_nNumTextures	= 0;
}

nSkinManager::~nSkinManager( void )
{
	for (u32 i=0; i<m_nNumTextures; i++)
		SAFE_THROW(m_pTextures[i]);
		
	m_pTextures.Clear();

	m_pMaterials.Clear();

	m_pSkins.Clear();
}

AI_RESULT nSkinManager::AddSkin( const IMaterial &mat, u32 *nSkinID )
{
	ISkin	skin;
	if (m_nNumSkins >= m_pSkins.Length())
		m_nNumSkins = m_pSkins.AddLast(skin);

	bool bMatFound = false;
	u32	nMat = 0;
	for (nMat=0; nMat<m_nNumMaterials; nMat++)
	{
		if (m_pMaterials[nMat] == mat)
		{
			bMatFound = true;
			break;
		}
	}

	if (bMatFound) m_pSkins[m_nNumSkins].Material = nMat;
	else
	{
		// Add a material to the array of material
		m_nNumMaterials = m_pMaterials.AddLast(mat);

		// assign the material and texture to the skin
		m_pSkins[m_nNumSkins].Material = m_nNumMaterials;

		m_nNumMaterials++;
	}

	for (u32 i=0; i<8; i++)	
		m_pSkins[m_nNumSkins].Texture[i] = MAX_SKINID;

	// skin id
	*nSkinID = m_nNumSkins;

	// increase for the next skin id
	m_nNumSkins++;
	
	return AI_OK;
}

u32 nSkinManager::AddTexture(u32 nSkinID, ITexture *texture)
{
	if (nSkinID >= MAX_SKINID) return MAX_ID;

	bool slotFound = false;
	bool texFound = false;
	u32	slot, nTex;

	// check is there any texture slot for the given skin id
	for (nTex=0; nTex<8; nTex++)
		if (m_pSkins[nSkinID].Texture[nTex] == MAX_SKINID)
		{
			texFound = true;
			break;
		}

	// if it's not then get out and give out of memory flag
	if (!texFound) return MAX_ID;

	// Is there any free slot in the textures array
	for (slot=0; slot<m_nNumTextures; slot++)
		if (strcmp(m_pTextures[slot]->GetTextureName(), texture->GetTextureName()) == 0)
		{
			slotFound = true;
			break;
		}
		
	// If there's not, the we create another 50 slots
    if (!slotFound)
	{
		ITexture *tex = texture;

		// assign the slot with the texture
		m_nNumTextures = m_pTextures.AddLast(tex);
	
		// get the slot id
		slot = m_nNumTextures;

		// increase the number of used textures
		m_nNumTextures++;
	}

	// we have one
	m_pSkins[nSkinID].Texture[nTex] = slot;

	return slot;
}

void nSkinManager::DeleteSkin( u32 nSkinID )
{
	if (nSkinID < m_nNumSkins) m_pSkins.Delete(nSkinID);
}

void nSkinManager::DeleteTexture( u32 nTexID )
{
	if (nTexID < m_nNumTextures) m_pTextures.Delete(nTexID);
}

void nSkinManager::DeleteMaterial( u32 nMatID )
{
	if (nMatID < m_nNumMaterials) m_pMaterials.Delete(nMatID);
}

u32 nSkinManager::AddTexture(u32 nSkinID, ISurface *surface)
{
	ITexture *texture = m_pParent->CreateTexture( surface );
	return AddTexture(nSkinID, texture);
}

u32 nSkinManager::AddTexture(u32 nSkinID, const c8 *textureName )
{
	ITexture *texture = m_pParent->CreateTexture(textureName);
	return AddTexture(nSkinID, texture);
}

ISkin nSkinManager::GetSkin( u32 nSkinID )
{
	if (nSkinID<0 || nSkinID>=m_nNumSkins)
	{
		ISkin	empty;
		memset(&empty, MAX_ID, sizeof(ISkin));
		return empty;
	}
	else
		return m_pSkins[nSkinID];
}

IMaterial nSkinManager::GetMaterial( u32 nMatID )
{
	if (nMatID<0 || nMatID>=m_nNumMaterials)
	{
		IMaterial empty;
		return empty;
	}
	else
		return m_pMaterials[nMatID];
}

ITexture *nSkinManager::GetTexture( u32 nTexID )
{
	if (nTexID<0 || nTexID>=m_nNumTextures)
	{
		return NULL;
	}
	else
		return m_pTextures[nTexID];
}

c8 *nSkinManager::GetTextureName( u32 nTexID )
{
	if (nTexID<0 || nTexID>=m_nNumTextures)
	{
		return 0;
	}
	else
		return m_pTextures[nTexID]->GetTextureName();
}

/*****************************************
  Skin Manager creation
 *****************************************/

ISkinManager *createSkinManager( nD3DDriver *pParent )
{
	return new nSkinManager( pParent );
}

}	// namespace Video
}	// namespace Ahmad
