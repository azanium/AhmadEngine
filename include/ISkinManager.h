/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _ISKINMANAGER_H_
#define _ISKINMANAGER_H_

#include "Types.h"
#include "IBuyut.h"
#include "IMaterial.h"
#include "ITexture.h"
#include "ISkin.h"

namespace Ahmad
{
namespace Video
{

#define MAX_SKINID	65535

// Parent class for SkinManager
class IRenderDevice;

class ISkinManager: public IBuyut
{
public:
	ISkinManager( void ) {}
	virtual ~ISkinManager( void ) {}

	virtual AI_RESULT		AddSkin( const IMaterial &mat, u32 *nSkinID ) = 0;
	virtual u32				AddTexture( u32 nSkinID, ISurface *surface ) = 0;
	virtual u32				AddTexture( u32 nSkinID, ITexture *texture ) = 0;
	virtual u32				AddTexture( u32 nSkinID, const c8 *textureName ) = 0;
	virtual void			DeleteSkin( u32 nSkinID ) = 0;
	virtual void			DeleteTexture( u32 nTexID ) = 0;
	virtual void			DeleteMaterial( u32 nMatID ) = 0;

	virtual IRenderDevice	*GetParentRenderDevice( void ) = 0;

	virtual ISkin			GetSkin( u32 nSkinID ) = 0;
	virtual IMaterial		GetMaterial( u32 nMatID ) = 0;
	virtual ITexture		*GetTexture( u32 nTexID ) = 0;
	virtual c8				*GetTextureName( u32 nTexID ) = 0;

	virtual u32				GetSkinCount( void ) = 0;
	virtual u32				GetMaterialCount( void ) = 0;
	virtual u32				GetTextureCount( void ) = 0;
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _ISKINMANAGER_H_