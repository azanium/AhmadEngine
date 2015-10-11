/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _SKINMANAGER_H_
#define _SKINMANAGER_H_

#include <d3d9.h>
#include "ISkinManager.h"
#include "nD3DDriver.h"
#include "Array.h"

namespace Ahmad
{
namespace Video
{

//! Implementation class for the interface of SkinManager

class nSkinManager: public ISkinManager
{
public:
	nSkinManager( nD3DDriver *pParent );
	~nSkinManager( void );
	
	AI_RESULT		AddSkin( const IMaterial &mat, u32 *nSkinID );
	u32				AddTexture( u32 nSkinID, ISurface *surface );
	u32				AddTexture( u32 nSkinID, ITexture *texture );
	u32				AddTexture( u32 nSkinID, const c8 *textureName );
	void			DeleteSkin( u32 nSkinID );
	void			DeleteTexture( u32 nTexID );
	void			DeleteMaterial( u32 nMatID );

	IRenderDevice	*GetParentRenderDevice( void ) { return m_pParent; }

	ISkin			GetSkin( u32 nSkinID );
	IMaterial		GetMaterial( u32 nMatID );
	ITexture		*GetTexture( u32 nTexID );
	c8				*GetTextureName( u32 nTexID );

	u32				GetSkinCount( void ) { return m_nNumSkins; }
	u32				GetMaterialCount( void ) { return m_nNumMaterials; }
	u32				GetTextureCount( void ) { return m_nNumTextures; }

protected:
	Core::Array<IMaterial>	m_pMaterials;	//	IMaterial		*m_pMaterials;
	Core::Array<ITexture*>	m_pTextures;	//	ITexture		**m_pTextures;
	Core::Array<ISkin>		m_pSkins;		//	ISkin			*m_pSkins;
	u32						m_nNumMaterials;
	u32						m_nNumTextures;
	u32						m_nNumSkins;
	nD3DDriver				*m_pParent;
};

ISkinManager *createSkinManager( nD3DDriver *pParent );

}	// namespace Video
}	// namespace Ahmad

#endif	// _SKINMANAGER_H_