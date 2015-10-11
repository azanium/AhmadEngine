/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _RENDERD3D_H_
#define _RENDERD3D_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "Prerequsites.h"

#include "IRenderDevice.h"
#include "IVector.h"
#include "IDimension.h"
#include "IMatrix.h"
#include "Array.h"

namespace Ahmad
{
namespace Video
{

/****************************************************
  The implementation for the RenderDevice interface
 ****************************************************/

#define MAX_TEXTURE	8

class nD3DDriver: public IRenderDevice
{
public:
	nD3DDriver( HINSTANCE hDLL );
	~nD3DDriver( void );
	
	AI_RESULT		InitDriver( HWND, HWND *, int, int, int );
	AI_RESULT		InitDriver( HWND, const Core::IDimension<u32> &, int, int, bool );
	void			Release( void );
	bool			IsRunning( void );
	u32				GetScreenWidth( void );
	u32				GetScreenHeight( void );

	AI_RESULT		BeginRendering( bool, bool, bool );
	AI_RESULT		EndRendering( void );
	AI_RESULT		UseWindow( u32 nhWnd );
	AI_RESULT		Clear( bool, bool, bool );
	void			SetClearColor( f32, f32, f32 );
	void			SetClearColori( u32, u32, u32 );
	bool			IsSceneRunning( void ) { return m_bIsSceneRunning; }
	void			SetShadeMode( E_SHADE_MODE esm );
	E_SHADE_MODE	GetShadeMode( void ) { return m_ShadeMode; }
	void			SetFogEnable( bool enabled );
	bool			IsFogEnable( void ) { return m_UseFog; }
	void			SetFog( const IColor &color, f32 fStartFog, f32 fEndFog, 
							f32 fDensity, bool bPixelFog, E_FOG_CALC efc );
	void			SetFog( const SFog &fog );
	SFog			GetFog( void ) { return m_Fog; }
	void			SetMaterial( const IMaterial *mat );
	void			DrawCube( const Core::IVector3f &pos, f32 width, f32 height, f32 depth, bool bInsideVisible, u32 nSkinID );
	void			SetPointSize( f32 size );
	void			SetAmbientLight( const IColor &color );
	void			SetAmbientLight( u32 red, u32 green, u32 blue );
	void			SetAmbientLightF( f32 red, f32 green, f32 blue );

	// Vertex and Pixel Shader
	AI_RESULT		CreateVertexShader( const c8 *fname, bool IsCompiled, u32 *nID );
	AI_RESULT		CreateVertexShader( const void *pData, u32 nLength, bool IsCompiled, u32 *nID );
	AI_RESULT		ActivateVertexShader( u32 vid, E_VERTEX_TYPE evt );
	AI_RESULT		CreatePixelShader( const c8 *fname, bool IsCompiled, u32 *nID );
	AI_RESULT		CreatePixelShader( const void *pData, u32 nLength, bool IsCompiled, u32 *nID );
	AI_RESULT		ActivatePixelShader( u32 pid );
	bool			UsingShaders( void ) { return m_bUsingShaders; }
	void			EnableShaders( bool bUse );
	bool			ShaderVersionAvailable( E_SHADER_VERSION esv );
	void			SetVertexShaderConstant( u32 nReg, u32 num, bool *cBool );
	void			SetVertexShaderConstant( u32 nReg, u32 num, u32 *cInt );
	void			SetVertexShaderConstantF( u32 nReg, u32 num, void *cFloat );
	void			SetPixelShaderConstant( u32 nReg, u32 num, bool *cBool );
	void			SetPixelShaderConstant( u32 nReg, u32 num, u32 *cInt );
	void			SetPixelShaderConstant( u32 nReg, u32 num, f32 *cFloat );
	u32				VertexShaderCount( void );
	u32				PixelShaderCount( void );

	// Skin Manager
	ISkinManager	*GetSkinManager( void );
	void			SetActiveSkin( u32 nSkinID );
	u32				GetActiveSkin( void );

	// Image Manager
	IImageManager	*GetImageManager( void ) { return m_pImageManager; }

	// Render Manager
	IRenderMan		*GetRenderMan( void ) { return m_pRenderMan; }

	// Texture
	void			SetTextureFilter( u32 nSampler, E_TEXTURE_FILTER etf ) ;
	E_TEXTURE_FILTER GetTextureFilter( void );
	ITexture		*CreateTexture( ISurface *surface ); 
	ITexture		*CreateTexture( ISurface *surface, u32 nTexFormat ); 
	ITexture		*CreateTexture( const c8 *textureName );
	void			SetTexture( u32 nStage, ITexture *texture );
	ITexture		*GetLastTexture( u32 nStage );
	AI_RESULT		SetTextureOperation( u32 nStage, E_TEXTURE_OP eto );
	E_TEXTURE_OP	GetTextureOperation( u32 nStage );
	u32				GetMaxTextureCount( void ) { return m_nMaxTexture; }

	// Image the postfix U mean in 3D unit otherwise in screen coordinate unit
	void			Draw2DImage( ISurface *surface, const Core::IRectf &rect );
	void			Draw2DImageU( ISurface *surface, const Core::IRectf &rect );
	void			Draw3DImage( ISurface *surface, const Core::IRectf &rect, f32 z );

	// View mode
	void			SetBackfaceCulling( E_CULL_MODE ecm );
	E_CULL_MODE		GetBackfaceCulling( void ) { return m_BackfaceCulling; }
	void			ResetViewport( void );
	void			SetClippingPlane( f32 fNearPlane, f32 fFarPlane );
	AI_RESULT		SetViewport( u32 nStage );
	AI_RESULT		SetViewMode( u32 nStage, E_VIEW_MODE evm );
	u32				GetViewStage( void );
	E_VIEW_MODE		GetViewMode( void );
	SViewport		GetViewport( u32 nStage );
	void			SetViewStage( u32 nStage, f32 fov, const SViewport *vp ) ;
	void			SetDepthBufferMode( E_DEPTH_MODE edm );
	E_DEPTH_MODE	GetDepthBufferMode( void );
	void			ResetAllMatrix( void );
	void			SetWorldMatrix( const Core::IMatrix *pMatrix );
	Core::IMatrix&	GetWorldMatrix( void );
	void			SetViewLookAt( const Core::IVector3f &pos,
								   const Core::IVector3f &lookat,
								   const Core::IVector3f &up );
	void			CalcViewProjMatrix( void );
	void			CalcWorldViewProjMatrix( void );

	// Font
	AI_RESULT		RegisterFont( const c8 *fontName, bool bBold, bool bItalic, bool bUnderline,
								  u32 nSize, u32 *fontID );
	AI_RESULT		DrawText( s32 x, s32 y, const c8 *text, ... );
	void			SetFontColor( IColor color ) { m_FontColor.Set(color); }
	void			SetFontColor( u32 r, u32 g, u32 b, u32 a ) { m_FontColor.Set(r, g, b, a); }
	void			SetFontActive( u32 nID ) { if (nID < m_nNumFonts) m_nActiveFont = nID; }
	
protected:
	void					InitShaders( void );
	void					ReleaseShaders( void );
	void					CreateVertexDeclaration( E_VERTEX_TYPE evt );
	void					SetVertexDeclaration(  E_VERTEX_TYPE evt );
	void					Init2DMode( void );
	void					PrepareRenderStuffs( void );
	void					InitSubsystems( void );
	void					ReleaseSubsystems( void );
	AI_RESULT				InitD3DDriver( HWND, HWND *, int, int, int, bool );
	AI_RESULT				InitD3DDriver( HWND, const Core::IDimension<u32> &, int, int, bool );
	BOOL					Reset( void );
	void					ReleaseFont( void );

	HWND					m_hWnd[MAX_3DHWND];
	HWND					m_hWndMain;
	u32						m_nNumhWnd;
	u32						m_nActivehWnd;
	bool					m_bFullscreen;
	u32						m_nWidth;
	u32						m_nHeight;
	int						m_nDepth;
	bool					m_bIsRunning;
	bool					m_bStencil;
	IRenderMan				*m_pRenderMan;
	IImageManager			*m_pImageManager;

	f32						m_fNearPlane,
							m_fFarPlane;
	Core::IMatrix			m_View2DMatrix,
							m_Proj2DMatrix,
							m_View3DMatrix,
							m_ProjOrthoMatrix[MAX_STAGE],
							m_ProjPerspMatrix[MAX_STAGE],
							m_WorldMatrix,
							m_ViewProjMatrix,
							m_WorldViewProjMatrix;
	E_VIEW_MODE				m_ViewMode;
	SViewport				m_Viewport[MAX_STAGE];
	u32						m_nStage;
	E_DEPTH_MODE			m_DepthMode;
	E_CULL_MODE				m_BackfaceCulling;
	E_TEXTURE_FILTER		m_TextureFilter;
	ul32					m_nMaxAnisotropy;
	ul32					m_nAnisotropyLevel;
	E_SHADE_MODE			m_ShadeMode;
	SFog					m_Fog;
	bool					m_UseFog;
	LPD3DXFONT				*m_pFont;
	u32						m_nActiveFont;
	u32						m_nNumFonts;
	IColor					m_FontColor;
	ISkinManager			*m_pSkinManager;
	ITexture				*m_pUsedTexture[MAX_TEXTURE];		// DirectX 9.0 support up to 8
	D3DTEXTUREOP			m_TexOp[MAX_TEXTURE];
	u32						m_nMaxTexture;
	f32						m_nPointSize;

	// Vertex and Pixel Shader
	Core::Array<LPDIRECT3DVERTEXDECLARATION9>	m_pVDecl;
	Core::Array<LPDIRECT3DVERTEXSHADER9>		m_pVShader;
	Core::Array<LPDIRECT3DPIXELSHADER9>			m_pPShader;
	bool										m_bUsingShaders;
	bool										m_bShaderExists;
	bool										m_bShaderVersion[ESV_MAXSHADERVERSION];

private:
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pDevice;
	D3DPRESENT_PARAMETERS	m_d3dpp;
	LPDIRECT3DSWAPCHAIN9	m_pChain[MAX_3DHWND];
	D3DCAPS9				m_pd3dCaps;
	HINSTANCE				m_hDLL;
	D3DCOLOR				m_ClearColor;
	bool					m_bIsSceneRunning;
	u32						m_nActiveSkinID;
	BOOL					m_bDeviceLost;

	// Our main hidden executor
	AI_RESULT				Run(void);
};


/***************************************************************
  We use the C to avoid the mess of C++ calling convention
 ***************************************************************/

extern "C"
{
	AI_RESULT		CreateRenderDevice( HINSTANCE, IRenderDevice ** );
	AI_RESULT		ReleaseRenderDevice( IRenderDevice ** );

	typedef AI_RESULT ( *CREATERENDERDEVICE )( HINSTANCE, IRenderDevice ** );
	typedef AI_RESULT ( *RELEASERENDERDEVICE )( IRenderDevice ** );
};

}	// namespace Video
}	// namespace Ahmad

#endif	// _RENDERD3D_H_