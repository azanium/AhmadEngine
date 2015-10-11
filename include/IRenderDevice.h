/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IRENDERDEVICE_H_
#define _IRENDERDEVICE_H_

#include "Types.h"
#include "IBuyut.h"
#include "IDimension.h"
#include "ISkinManager.h"
#include "IRenderMan.h"
#include "ISurface.h"
#include "ITexture.h"
#include "RenderState.h"
#include "IRect.h"
#include "IImageManager.h"
#include "IMaterial.h"

namespace Ahmad
{
namespace Video
{

/****************************************************
  Maximum swap chained window allowed
 ****************************************************/

#define MAX_3DHWND	8		// eight multiple windows swap chain
#define MAX_STAGE	4		// 4 stages of projection matrix


/*****************************************************
  The interface declaration for the Render Device
 *****************************************************/

class IRenderDevice: public IBuyut
{
public:
	IRenderDevice() {};
	virtual ~IRenderDevice( void ) {};

	virtual AI_RESULT		InitDriver( HWND hWnd, 
										HWND *hWnds, 
										int nNumWnd, 
										int nDepth,
										int nStencil ) = 0;
	
	virtual AI_RESULT		InitDriver( HWND hWnd, 
										const Core::IDimension<u32> &range, 
										int nDepth, 
										int nStencil, 
										bool bFullscren ) = 0;

	virtual void			Release( void ) = 0;
	virtual bool			IsRunning( void ) = 0;
	virtual	u32				GetScreenWidth( void ) = 0;
	virtual	u32				GetScreenHeight( void ) = 0;

	// Rendering
	virtual AI_RESULT		BeginRendering( bool bClearPixel, 
											bool bClearDepth, 
											bool bClearStencil ) = 0;
	virtual AI_RESULT		EndRendering( void ) = 0;
	virtual AI_RESULT		UseWindow( u32 nhWnd ) = 0;
	virtual AI_RESULT		Clear( bool bClearPixel, 
								   bool bClearDepth, 
								   bool bClearStencil ) = 0;
	virtual void			SetClearColor( f32, f32, f32 ) = 0;
	virtual void			SetClearColori( u32, u32, u32 ) = 0;
	virtual bool			IsSceneRunning( void ) = 0;
	virtual void			SetShadeMode( E_SHADE_MODE esm ) = 0;
	virtual E_SHADE_MODE	GetShadeMode( void ) = 0;
	virtual	void			SetFogEnable( bool enabled ) = 0;
	virtual bool			IsFogEnable( void ) = 0;
	virtual void			SetFog( const IColor &color, f32 fStartFog, f32 fEndFog, 
									f32 fDensity, bool bPixelFog, E_FOG_CALC efc ) = 0;
	virtual void			SetFog( const SFog &fog ) = 0;
	virtual SFog			GetFog( void ) = 0;
	virtual void			SetMaterial( const IMaterial *mat ) = 0;
	virtual void			DrawCube( const Core::IVector3f &pos, f32 width, f32 height, f32 depth, bool bInsideVisible, u32 nSkinID ) = 0;
	virtual void			SetPointSize( f32 size ) = 0;
	virtual void			SetAmbientLight( const IColor &color ) = 0;
	virtual void			SetAmbientLight( u32 red, u32 green, u32 blue ) = 0;
	virtual void			SetAmbientLightF( f32 red, f32 green, f32 blue ) = 0;

	// Vertex and Pixel Shader
	virtual AI_RESULT		CreateVertexShader( const c8 *fname, bool IsCompiled, u32 *nID ) = 0;
	virtual AI_RESULT		CreateVertexShader( const void *pData, u32 nLength, bool IsCompiled, u32 *nID ) = 0;
	virtual AI_RESULT		ActivateVertexShader( u32 vid, E_VERTEX_TYPE evt ) = 0;
	virtual AI_RESULT		CreatePixelShader( const c8 *fname, bool IsCompiled, u32 *nID ) = 0;
	virtual AI_RESULT		CreatePixelShader( const void *pData, u32 nLength, bool IsCompiled, u32 *nID ) = 0;	
	virtual AI_RESULT		ActivatePixelShader( u32 pid ) = 0;
	virtual bool			UsingShaders( void ) = 0;
	virtual void			EnableShaders( bool bUse ) = 0;
	virtual bool			ShaderVersionAvailable( E_SHADER_VERSION esv ) = 0;
	virtual void			SetVertexShaderConstant( u32 nReg, u32 num, bool *cBool ) = 0;
	virtual void			SetVertexShaderConstant( u32 nReg, u32 num, u32 *cInt ) = 0;
	virtual void			SetVertexShaderConstantF( u32 nReg, u32 num, void *cFloat ) = 0;
	virtual void			SetPixelShaderConstant( u32 nReg, u32 num, bool *cBool ) = 0;
	virtual void			SetPixelShaderConstant( u32 nReg, u32 num, u32 *cInt ) = 0;
	virtual void			SetPixelShaderConstant( u32 nReg, u32 num, f32 *cFloat ) = 0;
	virtual u32				VertexShaderCount( void ) = 0;
	virtual u32				PixelShaderCount( void ) = 0;

	// Skin Manager
	virtual ISkinManager	*GetSkinManager( void ) = 0;
	virtual void			SetActiveSkin( u32 nSkinID ) = 0;
	virtual u32				GetActiveSkin( void ) = 0;

	// Image Manager
	virtual IImageManager	*GetImageManager( void ) = 0;

	// Render Manager
	virtual IRenderMan		*GetRenderMan( void ) = 0;

	// Texture
	virtual void			SetTextureFilter( u32 nSampler, E_TEXTURE_FILTER etf ) = 0;
	virtual E_TEXTURE_FILTER GetTextureFilter( void ) = 0;
	virtual ITexture		*CreateTexture( ISurface *surface ) = 0; 
	virtual ITexture		*CreateTexture( ISurface *surface, u32 nTexFormat ) = 0; 
	virtual ITexture		*CreateTexture( const c8 *textureName ) = 0;
	virtual void			SetTexture( u32 nStage, ITexture *texture ) = 0;
	virtual AI_RESULT		SetTextureOperation( u32 nStage, E_TEXTURE_OP eto ) = 0;
	virtual E_TEXTURE_OP	GetTextureOperation( u32 nStage ) = 0;
	virtual u32				GetMaxTextureCount( void ) = 0;

	// Image
	virtual void			Draw2DImage( ISurface *surface, const Core::IRectf &rect ) = 0;
	virtual void			Draw2DImageU( ISurface *surface, const Core::IRectf &rect ) = 0;
	virtual void			Draw3DImage( ISurface *surface, const Core::IRectf &rect, f32 z ) = 0;

	// Viewing
	virtual void			SetBackfaceCulling( E_CULL_MODE ecm ) = 0;
	virtual E_CULL_MODE		GetBackfaceCulling( void ) = 0;
	virtual void			ResetViewport( void ) = 0;
	virtual void			SetClippingPlane( f32 fNearPlane, f32 fFarPlane ) = 0;
	virtual AI_RESULT		SetViewport( u32 nStage ) = 0;
	virtual	AI_RESULT		SetViewMode( u32 nStage, E_VIEW_MODE evm ) = 0;
	virtual u32				GetViewStage( void ) = 0;
	virtual E_VIEW_MODE		GetViewMode( void ) = 0;
	virtual SViewport		GetViewport( u32 nStage ) = 0;
	virtual void			SetViewStage( u32 nStage, f32 fov, const SViewport *vp ) = 0;
	virtual void			SetDepthBufferMode( E_DEPTH_MODE edm ) = 0;
	virtual E_DEPTH_MODE	GetDepthBufferMode( void ) = 0;
	virtual void			ResetAllMatrix( void ) = 0;
	virtual void			SetWorldMatrix( const Core::IMatrix *pMatrix ) = 0;
	virtual Core::IMatrix&	GetWorldMatrix( void ) = 0;			
	virtual void			SetViewLookAt( const Core::IVector3f &pos,
										   const Core::IVector3f &lookat,
										   const Core::IVector3f &up ) = 0;

	// Font
	virtual AI_RESULT		RegisterFont( const c8 *fontName, bool bBold, bool bItalic, bool bUnderline,
										  u32 nSize, u32 *fontID ) = 0;
	virtual AI_RESULT		DrawText( s32 x, s32 y, const c8 *text, ... ) = 0;
	virtual void			SetFontColor( IColor color ) = 0;
	virtual void			SetFontColor( u32 r, u32 g, u32 b, u32 a ) = 0;
	virtual void			SetFontActive( u32 nID ) = 0;

};

}	// namespace Video
}	// namespace Ahmad

#endif	// _IRENDERDEVICE_H_	