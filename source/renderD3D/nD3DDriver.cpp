/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nD3DDriver.h"
#include "nRenderMan.h"
#include "nTexture.h"
#include "nImageManager.h"
#include "nSkinManager.h"

namespace Ahmad
{
namespace Video
{

/****************************************************
  This is the D3D9 Renderer code
 ****************************************************/

// The pointer for the drivers
nD3DDriver		*g_pD3DDriver = NULL;

//! Direct 3D driver constructor
nD3DDriver::nD3DDriver( HINSTANCE hDLL )
{
	// Direct 3D Device
	m_pDevice			= NULL;

	// Direct 3D Interface
	m_pD3D				= NULL;

	// DLL handle for this driver
	m_hDLL				= hDLL;

	// The clear color
	m_ClearColor		= D3DCOLOR_COLORVALUE(0, 0, 0, 1.0f);

	// Main window
	m_hWndMain			= NULL;

	// Number of target window
	m_nNumhWnd			= 0;

	// Current active window
	m_nActivehWnd		= 0;

	// If the driver is running this flag is set to true
	m_bIsRunning		= false;

	// If the rendering state begin, this flag is set to true
	m_bIsSceneRunning	= false;

	// The stencil capabilites
	m_bStencil			= false;

	// If the Direct 3D device happenes to be lost, this flag is set to true
	m_bDeviceLost		= FALSE;

	// Maximum supported texture for single pass
	m_nMaxTexture		= 1;

	// singleton? dirty little hack
	g_pD3DDriver		= this;

	// Clear the used texture
	for (u32 i=0; i<MAX_TEXTURE; i++)
		m_pUsedTexture[i] = NULL;
}

//! Direct 3D Driver destructor
nD3DDriver::~nD3DDriver(void)
{
	// call the release method
	Release();
}

//! Release method. Contains all of the deletion of objects.
void nD3DDriver::Release(void)
{
	// Release created font
	ReleaseFont();

	// Release all the subsystems
	ReleaseSubsystems();

	// Release the Shaders
	ReleaseShaders();

	// Release the Direct 3D device
	SAFE_RELEASE(m_pDevice);

	// Release the Direct 3D object
	SAFE_RELEASE(m_pD3D);
}

//! Preparing the whole subsystem
void nD3DDriver::InitSubsystems( void )
{
	/*********************************
	  Image Manager
     *********************************/
	m_pImageManager = createImageManager( this );

	/*********************************
	  Skin Manager
     *********************************/
	m_pSkinManager = createSkinManager( this );

	/*********************************
	  Render Manager
     *********************************/
	m_pRenderMan = createRenderMan(m_pDevice, this);
}


//! Releasing the whole subsystems
void nD3DDriver::ReleaseSubsystems( void )
{
	/*********************************
	  Skin Manager 
	 *********************************/
	SAFE_THROW(m_pSkinManager);

	/*********************************
	  Image Manager
     *********************************/
	SAFE_THROW(m_pImageManager);

	/*********************************
	  Render Manager 
	 *********************************/
	SAFE_THROW(m_pRenderMan);

}

//! Driver initialization for single window target
AI_RESULT nD3DDriver::InitDriver( HWND hWnd, 
								  const Core::IDimension<u32> &size, 
								  int nDepth,
								  int nStencil,
								  bool bFullscreen )
{
	AI_RESULT	ar;

	// If the driver is not currently created, get out
	if (m_bIsRunning) return AI_FAIL;

	/**************************************
	  Initialize Direct3D Device Driver
     **************************************/
	ar = InitD3DDriver( hWnd, size, nDepth, nStencil, bFullscreen );

	if (FAILED(ar)) return AI_FAIL;
	
	/**************************************
	  Initialize the whole subsystems
     **************************************/
	InitSubsystems();

	/**************************************
	  Initialize the Render Stuffs
     **************************************/
	PrepareRenderStuffs();

	// TODO: Add some addition settings here
	

	return ar;
}

//! Driver initialization for multiple target window
AI_RESULT	nD3DDriver::InitDriver( HWND hWnd, 
									HWND *hWnds, 
									int nNumWnd, 
									int nDepth,
									int nStencil )
{
	AI_RESULT	ar;

	// If the driver is not currently created, get out
	if (m_bIsRunning) return AI_FAIL;

	/**************************************
	  Initialize Direct3D Device Driver
     **************************************/
	ar = InitD3DDriver( hWnd, hWnds, nNumWnd, nDepth, nStencil, false );

	if (FAILED(ar)) return AI_FAIL;

	/**************************************
	  Initialize the whole subsystems
     **************************************/
	InitSubsystems();

	/**************************************
	  Initialize the Render Stuffs
     **************************************/
	PrepareRenderStuffs();

	/**************************************
	  Activate the first window
     **************************************/
	UseWindow(0);

	// TODO: Add some addition settings here

	return ar;
}

//! Put some render state and other stuffs here...
void nD3DDriver::PrepareRenderStuffs( void )
{
	// Initialize the vertex and pixel shaders
	InitShaders();

	// Set the ambient light
	SetAmbientLight(255, 255, 255);

	// Set the rendering shade mode
	SetPointSize(0.1f);
	SetShadeMode(ESM_SOLID);

	// Set the alpha blending
	m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
	m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	
	// Set the texture operation
	SetTextureOperation(0, ETO_DARKMAP);
	SetTextureOperation(1, ETO_NONE);
	SetTextureOperation(2, ETO_NONE);
	SetTextureOperation(3, ETO_NONE);
	SetTextureOperation(4, ETO_NONE);
	SetTextureOperation(5, ETO_NONE);
	SetTextureOperation(6, ETO_NONE);
	SetTextureOperation(7, ETO_NONE);

    // Disable lighting
	m_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Set the defaul materials
	D3DMATERIAL9 mtrl;
	memset(&mtrl, 0, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_pDevice->SetMaterial(&mtrl);

	// Set to bilinear texture filter
	SetTextureFilter( 0, ETF_BILINEAR );

	// Set the fog
	SetFogEnable( false );
	SetFog( IColor(0, 255, 255, 255), 00.0f, 50.0f, 0.04f, false, EFC_LINEAR);

	// Reset all the viewport stages
	ResetViewport();
	
	// Set the clipping plane
	SetClippingPlane(0.01f, 1000.0f);
	
	// Reset the view3d Matrix;
	m_View3DMatrix.Identity();

	// Set the first viewing stage
	SetViewStage(0, 45.0f, &m_Viewport[0]);

	// Now activate the viewport
	SetViewport(0);

	// Then, we switch into the perspective viewing mode
	SetViewMode(0, EVM_PERSPECTIVE);

	// Set the world matrix into identity matrix 
	SetWorldMatrix( NULL );

	// Set the depth buffer mode
	//SetDepthBufferMode( EDM_READWRITE );

	// Register the Standard Font
	u32 nFontID;
	m_pFont = NULL;
	m_nNumFonts = 0;
	m_nActiveFont = 0;
	RegisterFont("Verdana", false, false, false, 10, &nFontID);
	RegisterFont("Verdana", true, false, false, 10, &nFontID);
	RegisterFont("Arial", false, false, false, 10, &nFontID);
	RegisterFont("Arial", true, false, false, 10, &nFontID);
	RegisterFont("Times New Roman", false, false, false, 10, &nFontID);
	RegisterFont("Times New Roman", true, false, false, 10, &nFontID);
	SetFontActive(0);
    SetFontColor(255, 255, 255, 255);
}

void nD3DDriver::InitShaders( void )
{
	D3DCAPS9	caps;
	m_bUsingShaders = false;
	m_bShaderExists = false;

	if (m_pDevice->GetDeviceCaps(&caps))
		return;
	
	// Check for Shaders v1.1
	if (caps.VertexShaderVersion < D3DVS_VERSION(1,1))
		return;
	m_bShaderVersion[u32(ESV_VSHADER_1_1)] = true;

	if (caps.PixelShaderVersion < D3DPS_VERSION(1,1))
		return;
	m_bShaderVersion[u32(ESV_PSHADER_1_1)] = true;

	// Check for Shaders v1.4
	if (caps.VertexShaderVersion >= D3DVS_VERSION(1,4))
		m_bShaderVersion[u32(ESV_VSHADER_1_4)] = true;

	if (caps.PixelShaderVersion >= D3DPS_VERSION(1,4))
		m_bShaderVersion[u32(ESV_PSHADER_1_4)] = true;

	// Check for Shaders v2.0
	if (caps.VertexShaderVersion >= D3DVS_VERSION(2,0))
		m_bShaderVersion[u32(ESV_VSHADER_2_0)] = true;

	if (caps.PixelShaderVersion >= D3DPS_VERSION(2,0))
		m_bShaderVersion[u32(ESV_PSHADER_2_0)] = true;

	// Check for Shaders v3.0
	if (caps.VertexShaderVersion >= D3DVS_VERSION(3,0))
		m_bShaderVersion[u32(ESV_VSHADER_3_0)] = true;

	if (caps.PixelShaderVersion >= D3DPS_VERSION(3,0))
		m_bShaderVersion[u32(ESV_PSHADER_3_0)] = true;

	// Set the vertex declarations
	LPDIRECT3DVERTEXDECLARATION9	pVD;	
	u32 id;
	for (u32 i=0; i<u32(EVT_MAXVERTEXTYPE); i++)
	{
		id = m_pVDecl.AddLast(pVD);
		m_pVDecl[id] = NULL;
	}

	for (u32 i=0; i<u32(EVT_MAXVERTEXTYPE); i++)
		CreateVertexDeclaration(E_VERTEX_TYPE(i));
	
	m_pDevice->SetFVF(NULL);
	m_bUsingShaders = true;
	m_bShaderExists = true;

	// Built-in vertex shader in vshader array slot 0
	const char vshader[] =
		"vs.1.1				\n"\
		"dcl_position0	v0	\n"\
		"dcl_normal0	v3	\n"\
		"dcl_texcoord0	v6	\n"\
		"def c4, 1, 1, 1, 1 \n"\
		"m4x4 oPos, v0, c0	\n"\
		"mov oD0, c4		\n"\
		"mov oT0, v6		\n";
	
	// Create our built in vertex shader
	CreateVertexShader((void *)vshader, (u32)strlen(vshader), false, NULL);
}

void nD3DDriver::ReleaseShaders( void )
{
	// Release the vertex declaration
	for (u32 i=0; i<m_pVDecl.Length(); i++)
		SAFE_RELEASE(m_pVDecl[i]);
	m_pVDecl.Clear();

	// Release the Vertex Shader
	for (u32 i=0; i<m_pVShader.Length(); i++)
		SAFE_RELEASE(m_pVShader[i]);
	m_pVShader.Clear();

	// Release the Pixel Shader
	for (u32 i=0; i<m_pPShader.Length(); i++)
		SAFE_RELEASE(m_pPShader[i]);
	m_pPShader.Clear();
}

//! The begin rendering state
AI_RESULT nD3DDriver::BeginRendering( bool bClearPixel, bool bClearDepth, bool bClearStencil )
{
	// If the device is not available, get out
	if (!m_pDevice) return AI_FAIL;

	// Deactivate the flag that the scene is up and running
	m_bIsSceneRunning = false;

	HRESULT	ar;

	// If the device is lost in previous frame, test it
	if (m_bDeviceLost)
	{
		if (FAILED(ar = m_pDevice->TestCooperativeLevel()))
		{
			if (ar == D3DERR_DEVICELOST)
				return AI_FAIL;

			if (ar == D3DERR_DEVICENOTRESET)
				Reset();
		}
	}

	// Set the right flag for the scene clearing
	DWORD flags = 0;
	if (bClearPixel) flags |= D3DCLEAR_TARGET;
	if (bClearDepth) flags |= D3DCLEAR_ZBUFFER;
	if (bClearStencil && m_bStencil) flags |= D3DCLEAR_STENCIL;

	// Clear the scene
	if (FAILED(m_pDevice->Clear(0, NULL, flags, m_ClearColor, 1.0f, 0)))
		return AI_FAIL;

	// Activate the Direct3D begin scene state
	if (FAILED(m_pDevice->BeginScene()))
		return AI_FAIL;

	// Activate the flag for the scene is running
	m_bIsSceneRunning = true;

	// OK, no problem sir!
	return AI_OK;
}

//! End rendering state
AI_RESULT nD3DDriver::EndRendering( void )
{
	// If the scene is not currenty in rendering state, get out
	if (!m_bIsSceneRunning) return AI_FAIL;

	// If the Direct 3D device is not available, get out
	if (!m_pDevice) return AI_FAIL;

	// Flush all the vertex caches available
	m_pRenderMan->FlushAll();

	HRESULT	ar;

	// Call the end rendering state of Direct 3D
	if (FAILED(m_pDevice->EndScene()))
		return AI_FAIL;	

	// If we use the multiple target, call the Present from the swap chain
	if (!m_bFullscreen && m_nNumhWnd>0)
		ar = m_pChain[m_nActivehWnd]->Present(NULL, NULL, NULL, NULL, 0);
	else
	
	// Otherwise, call from the device instead
		ar = m_pDevice->Present(NULL, NULL, NULL, NULL);

	// Check for any device lost
	if (ar == D3DERR_DEVICELOST)
		m_bDeviceLost = TRUE;
	else
	if (FAILED(ar) && ar != D3DERR_INVALIDCALL)
	{
		return AI_FAIL;
	}

	// No more rendering state
	m_bIsSceneRunning = false;

	return AI_OK;
}

//! Direct3D reset method
BOOL nD3DDriver::Reset( void )
{
	// Reset the Direct 3D, this method is called once the device is lost
	if (FAILED(m_pDevice->Reset(&m_d3dpp)))
	{
		return FALSE;
	}

	// Disable the related flag
	m_bDeviceLost = FALSE;
	m_bIsSceneRunning = FALSE;

	// Yes sir, it's true sir!
	return TRUE;
}

//! Clear the scene
AI_RESULT nD3DDriver::Clear(bool bClearPixel, bool bClearDepth, bool bClearStencil)
{
	// If the device is not currently available, get out
	if (!m_pDevice)	return AI_FAIL;

	// The flags for clearing the scene
	DWORD flags = 0;
	if (bClearPixel) flags |= D3DCLEAR_TARGET;
	if (bClearDepth) flags |= D3DCLEAR_ZBUFFER;
	if (bClearStencil && m_bStencil) flags |= D3DCLEAR_STENCIL;

	// If the scene is awake, sleep 'im now
	if (m_bIsSceneRunning)
		m_pDevice->EndScene();

	// Clear the scene
	if (FAILED(m_pDevice->Clear(0, NULL, flags, m_ClearColor, 1.0f, 0)))
		return AI_FAIL;

	// Is the scene is sleeping, wake him now
	if (m_bIsSceneRunning)
		m_pDevice->BeginScene();

	// OK baby
	return AI_OK;
}

//! Set the scene clear color in range 0-1
void nD3DDriver::SetClearColor(f32 red, f32 green, f32 blue)
{
	m_ClearColor = D3DCOLOR_COLORVALUE(red, green, blue, 1.0f);
}

//! Set the scene clear color in range 0-255
void nD3DDriver::SetClearColori(u32 red, u32 green, u32 blue)
{
	m_ClearColor = D3DCOLOR_COLORVALUE((f32)red/255, (f32)green/255, (f32)blue/255, 1.0f);
}

inline DWORD FTODW( f32 value ) { return *((DWORD *)&value); }

void nD3DDriver::SetShadeMode( E_SHADE_MODE esm )
{
	if (!m_pDevice) return;

	if (m_ShadeMode != esm)
	{
		m_pRenderMan->FlushAll();
	}

	if (esm == ESM_WIREFRAME)
	{
		m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else
	{
		if (esm == ESM_POINTS)
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
		else
			m_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	if (esm == ESM_POINTSPRITE)
	{
		if (m_nPointSize>0.0f)
		{
			m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_POINTSCALE_A, FTODW(0.0f));
			m_pDevice->SetRenderState(D3DRS_POINTSCALE_B, FTODW(0.0f));
			m_pDevice->SetRenderState(D3DRS_POINTSCALE_C, FTODW(0.0f));
		}
		else
		{
			m_pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
			m_pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
		}
	}

	m_ShadeMode = esm;
}

//! Enable the fog support
void nD3DDriver::SetFogEnable( bool enabled )
{
	// Yara, yara, yara...
	if (!m_pDevice) return;
	
	// Flush all the vertex cache	
	if (m_UseFog != enabled)
		m_pRenderMan->FlushAll();

	// Enable the use of fog 
	m_UseFog = enabled;

	// Tell the Direct 3D that we need to enable the fog
	m_pDevice->SetRenderState(D3DRS_FOGENABLE, enabled ? TRUE : FALSE);
}

//! Fog complicated setting
void nD3DDriver::SetFog( const IColor &color, f32 fStartFog, f32 fEndFog, 
						 f32 fDensity, bool bPixelFog, E_FOG_CALC efc )
{
	// Neeee....
	if (!m_pDevice) return;

	// Set the render state of the fog color
	m_pDevice->SetRenderState(D3DRS_FOGCOLOR, color.Color );
	
	// Set the fog structure
	m_Fog.Set( color, fStartFog, fEndFog, fDensity, bPixelFog, efc );

	// choose the right setting
	switch (efc)
	{
		case EFC_LINEAR:
			m_pDevice->SetRenderState(bPixelFog ? D3DRS_FOGTABLEMODE : D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
			m_pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fStartFog));
			m_pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&fEndFog));
			break;
		case EFC_EXP:
			m_pDevice->SetRenderState(bPixelFog ? D3DRS_FOGTABLEMODE : D3DRS_FOGVERTEXMODE, D3DFOG_EXP);
			m_pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&fDensity));
			break;
		case EFC_EXP2:
			m_pDevice->SetRenderState(bPixelFog ? D3DRS_FOGTABLEMODE : D3DRS_FOGVERTEXMODE, D3DFOG_EXP2);
			m_pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&fDensity));
			break;
	}
}

//! Fog simple setting
void nD3DDriver::SetFog( const SFog &fog )
{
	// Just call the fog complicated setting
	SetFog( fog.Color, fog.StartFog, fog.EndFog, fog.Density, fog.PixelFog, fog.efcType );
}

//! Setting the material for the device (i never use it)
void nD3DDriver::SetMaterial( const IMaterial *mat )
{
	D3DMATERIAL9 m;

	if (mat)
	{
		m.Ambient.r = (f32)mat->Ambient.GetRed() / 255.0f;
		m.Ambient.g = (f32)mat->Ambient.GetGreen() / 255.0f;
		m.Ambient.b = (f32)mat->Ambient.GetBlue() / 255.0f;
		m.Diffuse.r = (f32)mat->Diffuse.GetRed() / 255.0f;
		m.Diffuse.g = (f32)mat->Diffuse.GetGreen() / 255.0f;
		m.Diffuse.b = (f32)mat->Diffuse.GetBlue() / 255.0f;
		m.Emissive.r = (f32)mat->Emissive.GetRed() / 255.0f;
		m.Emissive.g = (f32)mat->Emissive.GetGreen() / 255.0f;
		m.Emissive.b = (f32)mat->Emissive.GetBlue() / 255.0f;
		m.Specular.r = (f32)mat->Specular.GetRed() / 255.0f;
		m.Specular.g = (f32)mat->Specular.GetGreen() / 255.0f;
		m.Specular.b = (f32)mat->Specular.GetBlue() / 255.0f;
		m.Power = mat->fShininess;
	}
	else
	{
		memset(&m, 0, sizeof(m));
		m.Diffuse.r = m.Ambient.r = 1.0f;
		m.Diffuse.g = m.Ambient.g = 1.0f;
		m.Diffuse.b = m.Ambient.b = 1.0f;
		m.Diffuse.a = m.Ambient.a = 1.0f;
	};
	memset(&m, 0, sizeof(m));
	m_pDevice->SetMaterial(&m);
}

void nD3DDriver::SetPointSize( f32 size )
{
	if (m_nPointSize != size)
	{
		m_nPointSize = size;
		m_pDevice->SetRenderState(D3DRS_POINTSIZE, FTODW(m_nPointSize));
		m_pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FTODW(0.0f));
	}
}

void nD3DDriver::SetAmbientLight( const IColor &color )
{
	m_pRenderMan->FlushAll();

	if (m_bUsingShaders)
	{
		f32 fcolor[4] = { color.GetRedf(), color.GetGreenf(), color.GetBluef(), color.GetAlphaf() };
		m_pDevice->SetVertexShaderConstantF(4, fcolor, 1);
	}
	
	m_pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(color.GetRed(), color.GetGreen(), color.GetBlue()));
}

void nD3DDriver::SetAmbientLight( u32 red, u32 green, u32 blue )
{
	m_pRenderMan->FlushAll();

	if (m_bUsingShaders)
	{
		f32 fcolor[4] = { (f32)red/255, (f32)green/255, (f32)blue/255, 1.0f };
		m_pDevice->SetVertexShaderConstantF(4, fcolor, 1);
	}
	
	m_pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(red, green, blue));
}

void nD3DDriver::SetAmbientLightF( f32 red, f32 green, f32 blue )
{
	m_pRenderMan->FlushAll();

	IColor color((u32)red*255, (u32)green*255, (u32)blue*255, 255);
	
	if (m_bUsingShaders)
	{
		f32 fcolor[4] = { red, green, blue, 1.0f };
		m_pDevice->SetVertexShaderConstantF(4, fcolor, 1);
	}
	
	m_pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(color.GetRed(), color.GetGreen(), color.GetBlue()));
}

/**********************************
  Vertex and Pixel Shader
 **********************************/

AI_RESULT nD3DDriver::CreateVertexShader( const c8 *fname, bool IsCompiled, u32 *nID )
{
	LPD3DXBUFFER	pShader = NULL;
	HANDLE			hFile, hMap;
	DWORD			*pVS = NULL;
	HRESULT			hr = AI_OK;

	if (IsCompiled)
	{
		hFile = ::CreateFile((LPCTSTR)fname, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return AI_FILENOTFOUND;
		
		hMap = ::CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		pVS = (DWORD *)::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	}
	else
	{
        hr = D3DXAssembleShaderFromFile(fname, NULL, NULL, 0, &pShader, NULL);

		if (SUCCEEDED(hr))
		{
			pVS = (DWORD *)pShader->GetBufferPointer();
		}
		else return AI_FAIL;
	}
	
	HRESULT hvs;
	
	LPDIRECT3DVERTEXSHADER9	vsh;
	u32 id = m_pVShader.AddLast(vsh);
	m_pVShader[id] = NULL;
	
	hvs = m_pDevice->CreateVertexShader(pVS, &m_pVShader[id]);
	if (FAILED(hvs))
		return AI_FAIL;

	if (nID) *nID = id;
	
	SAFE_RELEASE(pShader);
	
	if (IsCompiled)
	{
		UnmapViewOfFile(pVS);
		CloseHandle(hMap);
		CloseHandle(hFile);
	}

	return AI_OK;
}

AI_RESULT nD3DDriver::CreateVertexShader( const void *pData, u32 nLength, bool IsCompiled, u32 *nID )
{
	LPD3DXBUFFER	pShader = NULL;
	DWORD			*pVS = NULL;
	HRESULT			hr = AI_OK;

	if (IsCompiled)
	{
		pVS = (DWORD *)pData;
	}
	else
	{
        hr = D3DXAssembleShader((char *)pData, nLength, NULL, NULL, 0, &pShader, NULL);

		if (SUCCEEDED(hr))
		{
			pVS = (DWORD *)pShader->GetBufferPointer();
		}
		else return AI_FAIL;
	}
	
	HRESULT hvs;
	
	LPDIRECT3DVERTEXSHADER9	vsh;
	u32 id = m_pVShader.AddLast(vsh);
	m_pVShader[id] = NULL;
	
	hvs = m_pDevice->CreateVertexShader(pVS, &m_pVShader[id]);
	if (FAILED(hvs))
		return AI_FAIL;

	if (nID) *nID = id;
	
	SAFE_RELEASE(pShader);

	return AI_OK;
}

AI_RESULT nD3DDriver::ActivateVertexShader( u32 vid, E_VERTEX_TYPE evt )
{
	if (!m_bUsingShaders) return AI_FAIL;
	if (vid >= m_pVShader.Length()) return AI_FAIL;
	
	m_pRenderMan->FlushAll();

	SetVertexDeclaration(evt);

	if (FAILED(m_pDevice->SetVertexShader(m_pVShader[vid])))
	{
		return AI_FAIL;
	}

	return AI_OK;
}

AI_RESULT nD3DDriver::CreatePixelShader( const c8 *fname, bool IsCompiled, u32 *nID )
{
	LPD3DXBUFFER	pCode = NULL;
	HANDLE			hFile, hMap;
	DWORD			*pPS = NULL;
	HRESULT			hr = S_OK;

	if (IsCompiled)
	{
		hFile = ::CreateFile((LPCTSTR)fname, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
			return AI_FILENOTFOUND;
		
		hMap = ::CreateFileMapping(hFile, 0, PAGE_READONLY, 0, 0, 0);
		pPS = (DWORD *)::MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
	}
	else
	{
        hr = D3DXAssembleShaderFromFile(fname, NULL, NULL, NULL, &pCode, NULL);

		if (SUCCEEDED(hr))
		{
			pPS = (DWORD *)pCode->GetBufferPointer();
		}
		else return AI_FAIL;
	}
	
	HRESULT hps;
	
	LPDIRECT3DPIXELSHADER9	psh;
	u32 id = m_pPShader.AddLast(psh);
	m_pPShader[id] = NULL;

	hps = m_pDevice->CreatePixelShader(pPS, &m_pPShader[id]);
	if (FAILED(hps))
		return AI_FAIL;

	if (nID) *nID = id;
	
	SAFE_RELEASE(pCode);

	if (IsCompiled)
	{
		UnmapViewOfFile(pPS);
		CloseHandle(hMap);
		CloseHandle(hFile);
	}

	return AI_OK;
}

AI_RESULT nD3DDriver::CreatePixelShader( const void *pData, u32 nLength, bool IsCompiled, u32 *nID )
{
	LPD3DXBUFFER	pCode = NULL;
	DWORD			*pPS = NULL;
	HRESULT			hr = S_OK;

	if (IsCompiled)
	{
		pPS = (DWORD *)pData;
	}
	else
	{
        hr = D3DXAssembleShader((char *)pData, nLength, NULL, NULL, NULL, &pCode, NULL);

		if (SUCCEEDED(hr))
		{
			pPS = (DWORD *)pCode->GetBufferPointer();
		}
		else return AI_FAIL;
	}
	
	HRESULT hps;
	
	LPDIRECT3DPIXELSHADER9	psh;
	u32 id = m_pPShader.AddLast(psh);
	m_pPShader[id] = NULL;

	hps = m_pDevice->CreatePixelShader(pPS, &m_pPShader[id]);
	if (FAILED(hps))
		return AI_FAIL;

	if (nID) *nID = id;
	
	SAFE_RELEASE(pCode);

	return AI_OK;
}

AI_RESULT nD3DDriver::ActivatePixelShader( u32 pid )
{
	if (!m_bUsingShaders) return AI_FAIL;
	if (pid >= m_pPShader.Length()) return AI_FAIL;

	m_pRenderMan->FlushAll();
	
	if (FAILED(m_pDevice->SetPixelShader(m_pPShader[pid])))
	{

		return AI_FAIL;
	}

	return AI_OK;
}

void nD3DDriver::EnableShaders( bool bUse )
{
	if (!m_bShaderExists) return;

	if (m_bUsingShaders == bUse) return;

	m_pRenderMan->FlushAll();
	m_pRenderMan->InvalidateStates();

    m_bUsingShaders = bUse;

	if (!m_bUsingShaders)
	{
		m_pDevice->SetVertexDeclaration(NULL);
		m_pDevice->SetVertexShader(NULL);
		m_pDevice->SetPixelShader(NULL);
	}
	else
	{
		m_pDevice->SetFVF(NULL);
	}

	// Reset the viewing mode
	SetViewMode(m_nStage, m_ViewMode);
}

bool nD3DDriver::ShaderVersionAvailable( E_SHADER_VERSION esv )
{
	return m_bShaderVersion[u32(esv)];
}

void nD3DDriver::SetVertexShaderConstant( u32 nReg, u32 num, bool *cBool )
{
	m_pDevice->SetVertexShaderConstantB(nReg, (BOOL *)cBool, num);
}

void nD3DDriver::SetVertexShaderConstant( u32 nReg, u32 num, u32 *cInt )
{
	m_pDevice->SetVertexShaderConstantI(nReg, (int *)cInt, num);
}

void nD3DDriver::SetVertexShaderConstantF( u32 nReg, u32 num, void *cFloat )
{
	m_pDevice->SetVertexShaderConstantF(nReg, (float *)cFloat, num);
}

void nD3DDriver::SetPixelShaderConstant( u32 nReg, u32 num, bool *cBool )
{
	m_pDevice->SetPixelShaderConstantB(nReg, (BOOL *)cBool, num);
}

void nD3DDriver::SetPixelShaderConstant( u32 nReg, u32 num, u32 *cInt )
{
	m_pDevice->SetPixelShaderConstantB(nReg, (int *)cInt, num);
}

void nD3DDriver::SetPixelShaderConstant( u32 nReg, u32 num, f32 *cFloat )
{
	m_pDevice->SetPixelShaderConstantF(nReg, (float *)cFloat, num);
}

u32 nD3DDriver::VertexShaderCount( void )
{
	return m_pVShader.Length();
}

u32 nD3DDriver::PixelShaderCount( void )
{
	return m_pPShader.Length();
}

void nD3DDriver::CreateVertexDeclaration( E_VERTEX_TYPE evt )
{
	switch (evt)
	{
		case EVT_STANDARD:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
					{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
					{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;

		case EVT_VERTEXPOS:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;

		case EVT_VERTEXLIT:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
					{ 0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;

		case EVT_VERTEXNORMAL:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
					{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;

		case EVT_VERTEX2TEX:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
					{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;

		case EVT_VERTEX3TEX:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
					{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
					{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;

		case EVT_VERTEXTANGENT:
			{
				D3DVERTEXELEMENT9 vel[] =
				{
					{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
					{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
					{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
					D3DDECL_END()
				};
				m_pDevice->CreateVertexDeclaration(vel, &m_pVDecl[u32(evt)]);
			} break;
	}
}

void nD3DDriver::SetVertexDeclaration( E_VERTEX_TYPE evt )
{
	m_pDevice->SetVertexDeclaration(m_pVDecl[u32(evt)]);
}

//! Create a cube (using vertex cache)
void nD3DDriver::DrawCube( const Core::IVector3f &pos, f32 width, f32 height, f32 depth, bool bInsideVisible, u32 nSkinID )
{
	u16	pI[42] = {  0, 2, 1, 0, 3, 2, 4, 5, 6, 5, 7, 6,
					8, 9, 10, 9, 11, 10, 12, 13, 14, 14, 15, 12,
					16, 18, 17, 17, 18, 19, 22, 21, 20, 22, 23, 21 };

	u16	pI2[42] = {  1, 2, 0, 2, 3, 0, 6, 5, 4, 6, 7, 5,
					 10, 9, 8, 10, 11, 9, 14, 13, 12, 12, 15, 14,
					 17, 18, 16, 19, 18, 17, 20, 21, 22, 21, 23, 22 };
	
	SVertexNormal	pVerts[24];

	u16 *pIndices;

	if (bInsideVisible)
		pIndices = pI2;
	else
		pIndices = pI;

	f32 hw = width/2;
	f32 hh = height/2;
	f32 hd = depth/2;
	f32 n;
	if (bInsideVisible) n = 1; else n = -1;

	// top
	pVerts[0].Pos = Core::IVector3f(pos.x-hw, pos.y+hh, pos.z-hd);
	pVerts[0].TexCoord = Core::IVector2f(0.0f, 0.0f);
	pVerts[1].Pos = Core::IVector3f(pos.x-hw, pos.y+hh, pos.z+hd);
	pVerts[1].TexCoord = Core::IVector2f(0.0f, 1.0f);
	pVerts[2].Pos = Core::IVector3f(pos.x+hw, pos.y+hh, pos.z+hd);
	pVerts[2].TexCoord = Core::IVector2f(1.0f, 1.0f);
	pVerts[3].Pos = Core::IVector3f(pos.x+hw, pos.y+hh, pos.z-hd);
	pVerts[3].TexCoord = Core::IVector2f(1.0f, 0.0f);
	pVerts[0].Normal = pVerts[1].Normal = pVerts[2].Normal = pVerts[3].Normal = Core::IVector3f(0,n,0);

	// back
	pVerts[4].Pos = Core::IVector3f(pos.x-hw, pos.y+hh, pos.z+hd);
	pVerts[4].TexCoord = Core::IVector2f(0.0f, 0.0f);
	pVerts[5].Pos = Core::IVector3f(pos.x+hw, pos.y+hh, pos.z+hd);
	pVerts[5].TexCoord = Core::IVector2f(1.0f, 0.0f);
	pVerts[6].Pos = Core::IVector3f(pos.x-hw, pos.y-hh, pos.z+hd);
	pVerts[6].TexCoord = Core::IVector2f(0.0f, 1.0f);
	pVerts[7].Pos = Core::IVector3f(pos.x+hw, pos.y-hh, pos.z+hd);
	pVerts[7].TexCoord = Core::IVector2f(1.0f, 1.0f);
	pVerts[4].Normal = pVerts[5].Normal = pVerts[6].Normal = pVerts[7].Normal = Core::IVector3f(0,0,n);

	// left
	pVerts[8].Pos = Core::IVector3f(pos.x-hw, pos.y+hh, pos.z-hd);
	pVerts[8].TexCoord = Core::IVector2f(0.0f, 0.0f);
	pVerts[9].Pos = Core::IVector3f(pos.x-hw, pos.y+hh, pos.z+hd);
	pVerts[9].TexCoord = Core::IVector2f(1.0f, 0.0f);
	pVerts[10].Pos = Core::IVector3f(pos.x-hw, pos.y-hh, pos.z-hd);
	pVerts[10].TexCoord = Core::IVector2f(0.0f, 1.0f);
	pVerts[11].Pos = Core::IVector3f(pos.x-hw, pos.y-hh, pos.z+hd);
	pVerts[11].TexCoord = Core::IVector2f(1.0f, 1.0f);
	pVerts[8].Normal = pVerts[9].Normal = pVerts[10].Normal = pVerts[11].Normal = Core::IVector3f(-n,0,0);

	// bottom
	pVerts[12].Pos = Core::IVector3f(pos.x-hw, pos.y-hh, pos.z-hd);
	pVerts[12].TexCoord = Core::IVector2f(0.0f, 0.0f);
	pVerts[13].Pos = Core::IVector3f(pos.x-hw, pos.y-hh, pos.z+hd);
	pVerts[13].TexCoord = Core::IVector2f(0.0f, 1.0f);
	pVerts[14].Pos = Core::IVector3f(pos.x+hw, pos.y-hh, pos.z+hd);
	pVerts[14].TexCoord = Core::IVector2f(1.0f, 1.0f);
	pVerts[15].Pos = Core::IVector3f(pos.x+hw, pos.y-hh, pos.z-hd);
	pVerts[15].TexCoord = Core::IVector2f(1.0f, 0.0f);
	pVerts[12].Normal = pVerts[13].Normal = pVerts[14].Normal = pVerts[15].Normal = Core::IVector3f(0,-n,0);

	// right
	pVerts[16].Pos = Core::IVector3f(pos.x+hw, pos.y+hh, pos.z-hd);
	pVerts[16].TexCoord = Core::IVector2f(0.0f, 0.0f);
	pVerts[17].Pos = Core::IVector3f(pos.x+hw, pos.y+hh, pos.z+hd);
	pVerts[17].TexCoord = Core::IVector2f(1.0f, 0.0f);
	pVerts[18].Pos = Core::IVector3f(pos.x+hw, pos.y-hh, pos.z-hd);
	pVerts[18].TexCoord = Core::IVector2f(0.0f, 1.0f);
	pVerts[19].Pos = Core::IVector3f(pos.x+hw, pos.y-hh, pos.z+hd);
	pVerts[19].TexCoord = Core::IVector2f(1.0f, 1.0f);
	pVerts[16].Normal = pVerts[17].Normal = pVerts[18].Normal = pVerts[19].Normal = Core::IVector3f(n,0,0);

	// front
	pVerts[20].Pos = Core::IVector3f(pos.x-hw, pos.y+hh, pos.z-hd);
	pVerts[20].TexCoord = Core::IVector2f(0.0f, 0.0f);
	pVerts[21].Pos = Core::IVector3f(pos.x+hw, pos.y+hh, pos.z-hd);
	pVerts[21].TexCoord = Core::IVector2f(1.0f, 0.0f);
	pVerts[22].Pos = Core::IVector3f(pos.x-hw, pos.y-hh, pos.z-hd);
	pVerts[22].TexCoord = Core::IVector2f(0.0f, 1.0f);
	pVerts[23].Pos = Core::IVector3f(pos.x+hw, pos.y-hh, pos.z-hd);
	pVerts[23].TexCoord = Core::IVector2f(1.0f, 1.0f);
	pVerts[20].Normal = pVerts[21].Normal = pVerts[22].Normal = pVerts[23].Normal = Core::IVector3f(0,0,-n);
	
	m_pRenderMan->Render(EVT_VERTEXNORMAL, pVerts, 28, pIndices, 42, nSkinID);
}

//! Direct3D Driver Initialization
AI_RESULT nD3DDriver::Run(void)
{
	HWND	hwnd;

	// Create the interface object of direct3d 9
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION);
	
	if (!m_pD3D) return AI_FAIL;
	
	// Get the display mode
	D3DDISPLAYMODE	d3ddm;
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return AI_FAIL;
	}

	// Fill up the Direct3D Present Parameter
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;	//D3DSWAPEFFECT_FLIP;
	m_d3dpp.Windowed				= m_bFullscreen ? FALSE : TRUE;
	m_d3dpp.BackBufferFormat		= d3ddm.Format;
	m_d3dpp.EnableAutoDepthStencil	= TRUE;
	m_d3dpp.hDeviceWindow			= hwnd = m_hWnd[0];
	m_d3dpp.BackBufferCount			= 1;

	// My TV Tunner overlay runs very slow caused by this flag
	//m_d3dpp.PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE; 

	if (m_bFullscreen)
	{
		m_d3dpp.hDeviceWindow				= hwnd = m_hWndMain;
		m_d3dpp.BackBufferCount				= 0;
		m_d3dpp.BackBufferWidth				= m_nWidth;
		m_d3dpp.BackBufferHeight			= m_nHeight;
		m_d3dpp.BackBufferFormat			= D3DFMT_R5G6B5;
		m_d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		
		// Thanks to niko for the bloody code
		if (m_nDepth == 32 && !m_bStencil)
			m_d3dpp.BackBufferFormat	= D3DFMT_A8R8G8B8;
	}

	// TODO: Antialiasing

	// We use HAL only no pure software rendering
	D3DDEVTYPE	devType = D3DDEVTYPE_HAL;

	if (m_bStencil)
	{
		m_d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
		if (!FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT,
											  devType,
											  m_d3dpp.BackBufferFormat,
											  D3DUSAGE_DEPTHSTENCIL,
											  D3DRTYPE_SURFACE,
											  D3DFMT_D24S8)))
		{
			if (FAILED(m_pD3D->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
													  devType,
													  m_d3dpp.BackBufferFormat,
													  m_d3dpp.BackBufferFormat,
													  D3DFMT_D24S8)))
			{
				m_bStencil = false;
			}
		}
		else
			m_bStencil = false;
	}

	// If we don't use the stencil choose 16 instead of D24S8
	if (!m_bStencil)
		m_d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;

	// Create the device, we use brute force to check it 3 times using hardware, mixed and software
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									devType,
									hwnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&m_d3dpp,
									&m_pDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										devType,
										hwnd,
										D3DCREATE_MIXED_VERTEXPROCESSING,
										&m_d3dpp,
										&m_pDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											devType,
											hwnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&m_d3dpp,
											&m_pDevice)))
			{
				return AI_FAIL;
			}	// CreateDevice SOFTWARE
		}	// CreateDevice MIXED
	}	// CreateDevice HARDWARE

	// Device creation is failed
    if (!m_pDevice)
		return AI_FAIL;

	// Now set the additional swap chains
	if ((m_nNumhWnd > 0) && !m_bFullscreen)
		for (u32 i = 0; i<m_nNumhWnd; i++)
		{
			m_d3dpp.hDeviceWindow = m_hWnd[i];
			m_pDevice->CreateAdditionalSwapChain(&m_d3dpp, &m_pChain[i]);
		}

	// Get the device capabilites
	if (FAILED(m_pDevice->GetDeviceCaps(&m_pd3dCaps)))
		return AI_FAIL;

	// Get the anisotropic max value
	m_nMaxAnisotropy = m_pd3dCaps.MaxAnisotropy;

	// Maximu textures in a single pass
	m_nMaxTexture = m_pd3dCaps.MaxSimultaneousTextures;

	// check the stencil capabilities
	if (m_bStencil &&
		(!(m_pd3dCaps.StencilCaps & D3DSTENCILCAPS_INCRSAT) ||
		 !(m_pd3dCaps.StencilCaps & D3DSTENCILCAPS_DECRSAT) ||
		 !(m_pd3dCaps.StencilCaps & D3DSTENCILCAPS_KEEP)))
	{
		m_bStencil = false;
	}

	//TODO: Render states for AntiAliasing

	// Yes sir, the driver is up and running now
	m_bIsRunning = true;

	// No sir, the scene is not up and running yet
	m_bIsSceneRunning = false;

	// The scene width and height
	m_nWidth = m_d3dpp.BackBufferWidth;
	m_nHeight = m_d3dpp.BackBufferHeight;

	// OK sir!
	return AI_OK;
}

//! Switching between target window
AI_RESULT nD3DDriver::UseWindow(u32 nhWnd)
{
	if (!m_pDevice) return AI_FAIL;

	// If it is fullscreen or no multiple window, get out
	if (m_bFullscreen) return AI_OK;
	else if (nhWnd >= m_nNumhWnd) return AI_FAIL;

	// The Direct3D surface
	LPDIRECT3DSURFACE9	pBack = NULL;

	// Now we add the viewport adjustment here
	RECT	rect;
	GetClientRect(m_hWnd[nhWnd], &rect);
	m_nWidth = rect.right;
	m_nHeight = rect.bottom;

	// Get the backbuffer
	if (FAILED(m_pChain[nhWnd]->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBack)))
		return AI_FAIL;

	// Set the render target to this surface
	m_pDevice->SetRenderTarget(0, pBack);

	// Release the bloody surface
	pBack->Release();

	// Set this window the current selected window
	m_nActivehWnd = nhWnd;

	// Tired to explain sir.
	return AI_OK;
}

//! Multiple Rendering Windows
AI_RESULT nD3DDriver::InitD3DDriver(HWND hWnd, 
									HWND *hWnd3D, 
									int nNumhWnd, 
									int nDepth,
									int nStencil,
									bool bFullscreen)
{
	// Is the window is more than one?
	if (nNumhWnd > 0)
	{
		// Yes, copy the window to the variable of multiple window
		if (nNumhWnd > MAX_3DHWND) nNumhWnd = MAX_3DHWND;
		memcpy(&m_hWnd[0], hWnd3D, nNumhWnd*sizeof(HWND));		
		m_nNumhWnd = nNumhWnd;
	}
	else
	{
		// No, set things right
		m_hWnd[0] = hWnd;
		m_nNumhWnd = 0;
	}
	
	// Choose the main window
	m_hWndMain = hWnd;
	
	// If stencil bits is valid activate the stencil flag
	if (nStencil>0) m_bStencil = true;

	// Get the depth
	m_nDepth = nDepth;

	// Get the fullscreen flag
	m_bFullscreen = bFullscreen;

	// Initialize the Direct 3D Driver (read above)
	Run();
	
	// *_*
	return AI_OK;
}

//! Single Rendering Window
AI_RESULT nD3DDriver::InitD3DDriver(HWND hWnd, 
									const Core::IDimension<u32> &dim,
									int nDepth,
									int nStencil,
									bool bFullscreen)
{
	// No multiple window
	m_nNumhWnd = 0;

	// Set the related window handle
	m_hWndMain = m_hWnd[0] = hWnd;
	
	// If the stencil bits is valid, set the stencil flag
	if (nStencil>0) m_bStencil = true;

	// Get the size of the window
	RECT	rect;
	GetClientRect(hWnd, &rect);
	
	// To fix the bugs on the viewport changing, we test the current client rect
	if (rect.right != dim.Width) m_nWidth = rect.right;
	else m_nWidth = dim.Width;
	
	// To fix the bugs on the viewport changing, we test the current client rect
	if (rect.bottom != dim.Height) m_nHeight = rect.bottom;
	else m_nHeight = dim.Height;

	// Get the depth
	m_nDepth = nDepth;

	// Get the fullscreen
	m_bFullscreen = bFullscreen;

	// Initialize the driver
	Run();
	
	// *_*
	return AI_OK;
}

//! Is the driver running?
bool nD3DDriver::IsRunning( void )
{
	return m_bIsRunning;
}

//! Get the screen width
u32 nD3DDriver::GetScreenWidth( void )
{
	return m_nWidth;
}

//! Get the screen height
u32 nD3DDriver::GetScreenHeight( void )
{
	return m_nHeight;
}


/*********************************************************
  Skin Manager
 *********************************************************/

//! Get the SkinManager pointer
ISkinManager *nD3DDriver::GetSkinManager( void )
{
	return m_pSkinManager;
}

//! Set the active skin (user should not use this)
void nD3DDriver::SetActiveSkin( u32 nSkinID )
{
	m_nActiveSkinID = nSkinID;
}

//! Get the active skin 
u32 nD3DDriver::GetActiveSkin( void )
{
	return m_nActiveSkinID;
}


//! Texture Filtering (read on the RenderState.h)
void nD3DDriver::SetTextureFilter( u32 nSampler, E_TEXTURE_FILTER etf )
{
	// So desu ne...
    if (!m_pDevice) return;

	// Set the right filtering renderstate for the right flag 
	switch (etf)
	{
		m_TextureFilter = etf;

		case ETF_NEAREST:
			{
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MINFILTER, D3DTEXF_POINT);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
			} break;

		case ETF_BILINEAR:
			{
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			} break;

		case ETF_TRILINEAR:
			{
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			} break;

		case ETF_ANISOTROPIC:
			{
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
				m_pDevice->SetSamplerState(nSampler, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
				//m_pDevice->SetSamplerState(nSampler, D3DSAMP_MAXANISOTROPY, m_nMaxAnisotropy);
			} break;
	}
}

//! Get the actual texture filtering
E_TEXTURE_FILTER nD3DDriver::GetTextureFilter( void )
{
	return m_TextureFilter;
}

// Create a texture object from a surface (read on ITexture.h)
ITexture *nD3DDriver::CreateTexture( ISurface *surface )
{
	// Instantiate a new texture object from a surface
	nTexture *tex = new nTexture( surface->GetSurfaceName(), m_pDevice, surface );

	// If it's available return the right pointer to this texture object
	if (tex->IsAvailable())
		return tex;

	// Otherwise, throw the texture
	SAFE_THROW(tex);

	// Return NULL pointer
	return NULL;
}

//! Create a texture object from a surface with editable texture format (read on RenderState.h)
ITexture *nD3DDriver::CreateTexture( ISurface *surface, u32 nTexFormat )
{
	// Create the texture object from a surface with texture format
	nTexture *tex = new nTexture( surface->GetSurfaceName(), m_pDevice, surface, nTexFormat );

	// If it's available, return this texture object
	if (tex->IsAvailable())
		return tex;

	// Otherwise, throw our texture object
	SAFE_THROW(tex);

	// Return the NULL pointer
	return NULL;
}

//! Create a texture object from a texture filename
ITexture *nD3DDriver::CreateTexture( const c8 *textureName )
{
	// If the texturename is null then exit and give null pointer also
	if (!textureName) return NULL;

	// Get the image manager
	IImageManager *iman = GetImageManager();

	// Load a file and put it in the surface
	ISurface *surface = iman->LoadImageFromFile( textureName );

	// If there's no problem with the surface creation, create a texture from it
	if (surface)
	{
		// Instantiate the texture object from a surface
		ITexture *texture = CreateTexture(surface);

		// Throw away the surface
		SAFE_THROW(surface);

		// Return the right texture object
		return texture;
	}

	// Otherwise, return NULL pointer
	return NULL;
};

//! Activate a texture object
void nD3DDriver::SetTexture( u32 nStage, ITexture *texture )
{
	if (texture)
		m_pDevice->SetTexture(nStage, (LPDIRECT3DTEXTURE9)(texture->GetTexture()));
	else
		m_pDevice->SetTexture(nStage, NULL);

	m_pUsedTexture[nStage] = texture;
}

ITexture *nD3DDriver::GetLastTexture( u32 nStage )
{
	if (nStage<0 || nStage>MAX_TEXTURE)
		return NULL;
	else
		return m_pUsedTexture[nStage];
}

AI_RESULT nD3DDriver::SetTextureOperation( u32 nStage, E_TEXTURE_OP eto )
{
	D3DCAPS9	caps;
	m_pDevice->GetDeviceCaps(&caps);

	if (caps.MaxSimultaneousTextures < (nStage+1))
		return AI_FAIL;

	switch (eto)
	{
		case ETO_NONE:
			m_TexOp[nStage] = D3DTOP_DISABLE;
			break;

		case ETO_GLOWMAP:
			m_TexOp[nStage] = D3DTOP_ADDSIGNED;
			break;

		case ETO_DARKMAP:
			m_TexOp[nStage] = D3DTOP_MODULATE;
			break;
	}

	return AI_OK;
}

E_TEXTURE_OP nD3DDriver::GetTextureOperation( u32 nStage )
{
	D3DCAPS9	caps;

	m_pDevice->GetDeviceCaps(&caps);

	if (caps.MaxSimultaneousTextures < (nStage+1))
		return ETO_NONE;

	switch (m_TexOp[nStage])
	{
		case D3DTOP_DISABLE:
			return ETO_NONE;
			break;

		case D3DTOP_ADDSIGNED:
			return ETO_GLOWMAP;
			break;

		case D3DTOP_MODULATE:
			return ETO_DARKMAP;
			break;
	}

	return ETO_NONE;
}

/*********************************************************
  Image
 *********************************************************/

void nD3DDriver::Draw2DImage( ISurface *surface, const Core::IRectf &rect )
{
	if (!surface) return;

	E_VIEW_MODE		oldMode;
	Core::IMatrix	oldWorld;
	E_CULL_MODE		oldCull;
	bool			oldFog;

	oldMode = GetViewMode();
	oldWorld = GetWorldMatrix();
	oldCull = GetBackfaceCulling();
	oldFog = IsFogEnable();
	u32 nStage = GetViewStage();
	
	if (oldFog) SetFogEnable(false);

	if (oldMode != EVM_TWODIMENSION)
	{
		SetWorldMatrix(NULL);
		SetViewMode(nStage, EVM_TWODIMENSION);
	}

	if (oldCull != ECM_CCW)
		SetBackfaceCulling(ECM_CCW);

    ITexture *tex = CreateTexture( surface );
	SVertexPos	v[4];
	v[0].Pos = Core::IVector3f(rect.Left, rect.Top, 1.0f);
	v[0].TexCoord = Core::IVector2f(0.0f, 0.0f);
	v[1].Pos = Core::IVector3f(rect.Right, rect.Top, 1.0f);
	v[1].TexCoord = Core::IVector2f(1.0f, 0.0f);
	v[2].Pos = Core::IVector3f(rect.Left, rect.Bottom, 1.0f);
	v[2].TexCoord = Core::IVector2f(0.0f, 1.0f);
	v[3].Pos = Core::IVector3f(rect.Right, rect.Bottom, 1.0f);
	v[3].TexCoord = Core::IVector2f(1.0f, 1.0f);

	u16 indices[6] = { 0, 1, 2, 1, 3, 2 };

	ITexture	*texture = GetLastTexture(0);
	u32			skinID;
	ISkin		skin;

	// Add a skin to the SkinManager
	m_pSkinManager->AddSkin(IMaterial(), &skinID);

	// Add the texture
	u32 nTex = m_pSkinManager->AddTexture(skinID, tex);

	// Render the vertices using skin
	m_pRenderMan->Render(EVT_VERTEXPOS, v, 4, indices, 6, skinID);
	
	// Get the skin
	skin = m_pSkinManager->GetSkin(skinID);

	// Delete the texture for this skin
	if (nTex != MAX_ID) m_pSkinManager->DeleteTexture(skin.Texture[nTex]);

	// Delete the skin
	m_pSkinManager->DeleteSkin(skinID);

	SAFE_THROW(tex);

	if (oldMode != EVM_TWODIMENSION)
	{
		SetViewMode(nStage, oldMode);
		SetWorldMatrix(&oldWorld);
	}
	
	if (oldCull != ECM_CCW)
		SetBackfaceCulling(oldCull);

	if (oldFog) 
		SetFogEnable(true);
}

void nD3DDriver::Draw2DImageU( ISurface *surface, const Core::IRectf &rect )
{
	Core::IRectf r(rect.Left*m_nWidth, rect.Top*m_nHeight, rect.Right*m_nWidth, rect.Bottom*m_nHeight);
	Draw2DImage( surface, r );
}

void nD3DDriver::Draw3DImage( ISurface *surface, const Core::IRectf &rect, f32 z )
{
	if (!surface) return;

	E_VIEW_MODE		oldMode;
	Core::IMatrix	oldWorld;
	Core::IMatrix	oldView;
	E_CULL_MODE		oldCull;

	oldMode = GetViewMode();
	oldWorld = GetWorldMatrix();
	oldCull = GetBackfaceCulling();

	if (oldMode != EVM_PERSPECTIVE)
	{
		u32 nStage = GetViewStage();
		SetViewMode(nStage, EVM_PERSPECTIVE);
	}

	SetWorldMatrix(NULL);
	if (oldCull != ECM_CW)
		SetBackfaceCulling(ECM_CW);

    ITexture *tex = CreateTexture( surface );
	SVertexPos	v[4];
	v[0].Pos = Core::IVector3f(rect.Left, rect.Top, z);
	v[0].TexCoord = Core::IVector2f(0.0f, 0.0f);
	v[1].Pos = Core::IVector3f(rect.Right, rect.Top, z);
	v[1].TexCoord = Core::IVector2f(1.0f, 0.0f);
	v[2].Pos = Core::IVector3f(rect.Left, rect.Bottom, z);
	v[2].TexCoord = Core::IVector2f(0.0f, 1.0f);
	v[3].Pos = Core::IVector3f(rect.Right, rect.Bottom, z);
	v[3].TexCoord = Core::IVector2f(1.0f, 1.0f);

	u16 indices[6] = { 0, 1, 2, 1, 3, 2 };

	SetTexture(0, tex);
	m_pRenderMan->Render(EVT_VERTEXPOS, v, 4, indices, 6, 0);
	SetTexture(0, NULL);
	SAFE_THROW(tex);

	if (oldMode != EVM_PERSPECTIVE)
	{
		u32 nStage = GetViewStage();
		SetViewMode(nStage, oldMode);
	}
	SetWorldMatrix(&oldWorld);
	
	if (oldCull != ECM_CW)
		SetBackfaceCulling(oldCull);
}


/*********************************************************
  View Mode
 *********************************************************/

void nD3DDriver::SetBackfaceCulling( E_CULL_MODE ecm )
{
	if (m_pDevice && (ecm != m_BackfaceCulling))
	{
		m_pRenderMan->FlushAll();

		m_BackfaceCulling = ecm;
		switch (ecm)
		{
			case ECM_NONE:
                m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				break;

			case ECM_CW:
                m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
				break;

			case ECM_CCW:
                m_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				break;
		}
	}
}

void nD3DDriver::ResetViewport( void )
{
	if (m_nNumhWnd>0 && !m_bFullscreen) 
	{
		RECT rect;
		for (u32 win=0; win<m_nNumhWnd; win++)
		{
			GetClientRect(m_hWnd[win], &rect);
			SViewport vp(0, 0, rect.right, rect.bottom);
			for (u32 i=0; i<MAX_STAGE; i++)
				m_Viewport[i] = vp;
		}
	}
	else
	{
		SViewport vp(0, 0, m_nWidth, m_nHeight);
		for (u32 i=0; i<MAX_STAGE; i++)
			m_Viewport[i] = vp;
	}
}

void nD3DDriver::SetClippingPlane( f32 fNearPlane, f32 fFarPlane )
{
	m_fNearPlane = fNearPlane;
	m_fFarPlane = fFarPlane;

	if (m_fNearPlane <= 0.0f) m_fNearPlane = 0.01f;
	if (m_fFarPlane < 1.0f) m_fFarPlane = 1.0f;
	if (m_fFarPlane < m_fNearPlane)
	{
		f32 temp = m_fFarPlane;
		m_fFarPlane = m_fNearPlane;
		m_fNearPlane = temp;
	}

	Init2DMode();

	// Ganti semua anggota matrix yang berhubungan dengan clipping plane
	f32 q1 = 1.0f/(m_fFarPlane - m_fNearPlane);
	f32 p1 = m_fNearPlane*q1;
	f32 q2 = m_fFarPlane*q1;
	f32 p2 = -m_fNearPlane*q2;
	for (u32 i=0; i<MAX_STAGE; i++)
	{
		m_ProjOrthoMatrix[i]._33 = q1;
		m_ProjOrthoMatrix[i]._43 = p1;
		m_ProjPerspMatrix[i]._33 = q2;
		m_ProjPerspMatrix[i]._43 = p2;
	}
}

void nD3DDriver::Init2DMode( void )
{
	// 2D Projection Matrix 
	m_Proj2DMatrix.BuildOrthogonalProjectionMatrix( m_nWidth, m_nHeight, m_fNearPlane, m_fFarPlane );

	// Stupid translation for 2D viewing matrix
	m_View2DMatrix.Identity();
	m_View2DMatrix._11 = m_View2DMatrix._33 = m_View2DMatrix._44 = 1.0f;
	m_View2DMatrix._22 = -1.0f;

	f32 tx, ty, tz;
	tx = m_nWidth * 0.5f - m_nWidth;
	ty = m_nHeight - m_nHeight * 0.5f;
	tz = m_fNearPlane + 0.1f;

	m_View2DMatrix._41 = tx;
	m_View2DMatrix._42 = ty;
	m_View2DMatrix._43 = tz;
}

AI_RESULT nD3DDriver::SetViewport( u32 nStage )
{
	if (nStage>=MAX_STAGE) return AI_FAIL;
	
	D3DVIEWPORT9	vp;	
	
	m_pRenderMan->FlushAll();

	vp.X = m_Viewport[nStage].X;
	vp.Y = m_Viewport[nStage].Y;
	vp.Width = m_Viewport[nStage].Width;
	vp.Height = m_Viewport[nStage].Height;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	if (FAILED(m_pDevice->SetViewport( &vp )))
		return AI_FAIL;

	return AI_OK;
}

AI_RESULT nD3DDriver::SetViewMode( u32 nStage, E_VIEW_MODE evm )
{
	if ((nStage >= MAX_STAGE) || (!m_bIsRunning)) return AI_FAIL;
	
	m_ViewMode = evm;
	m_nStage = nStage;

	m_pRenderMan->FlushAll();

	D3DVIEWPORT9	vp;	

	vp.X = m_Viewport[nStage].X;
	vp.Y = m_Viewport[nStage].Y;
	vp.Width = m_Viewport[nStage].Width;
	vp.Height = m_Viewport[nStage].Height;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;

	if (FAILED(m_pDevice->SetViewport( &vp )))
		return AI_FAIL;

	if (evm == EVM_TWODIMENSION)
	{
		if (!m_bUsingShaders)
		{
			if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)&m_View2DMatrix)))
				return AI_FAIL;

			if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&m_Proj2DMatrix)))
				return AI_FAIL;
		}
	}
	else
	{
		if (!m_bUsingShaders)
		{
			if (FAILED(m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)&m_View3DMatrix)))
				return AI_FAIL;
 
			if (evm == EVM_ORTHOGONAL)
			{
				if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&m_ProjOrthoMatrix[nStage])))
					return AI_FAIL;
			}
			else
			{
				if (FAILED(m_pDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&m_ProjPerspMatrix[nStage])))
					return AI_FAIL;
			}
		}

		CalcViewProjMatrix();
		CalcWorldViewProjMatrix();
	}

	return AI_OK;
}

void nD3DDriver::SetViewStage( u32 nStage, f32 fov, const Video::SViewport *vp )
{
	if (nStage >= MAX_STAGE) return;

	if (!vp)
		m_Viewport[nStage] = SViewport(0, 0, m_nWidth, m_nHeight);
	else
		m_Viewport[nStage] = *vp;

	f32 Aspect = (f32)(m_nWidth / m_nHeight);

	m_ProjPerspMatrix[nStage].BuildPerspectiveProjectionMatrixLH( fov, Aspect, m_fNearPlane, m_fFarPlane );
	
	m_ProjOrthoMatrix[nStage].BuildOrthogonalProjectionMatrix( m_nWidth, m_nHeight, m_fNearPlane, m_fFarPlane );
}

u32 nD3DDriver::GetViewStage( void )
{
	return m_nStage;
}

E_VIEW_MODE	nD3DDriver::GetViewMode( void )
{
	return m_ViewMode;
}

SViewport nD3DDriver::GetViewport( u32 nStage )
{
	if (nStage < MAX_STAGE) return m_Viewport[nStage];
	return SViewport(0, 0, m_nWidth, m_nHeight);
}

void nD3DDriver::ResetAllMatrix()
{
	
}

void nD3DDriver::SetWorldMatrix( const Core::IMatrix *pMatrix )
{
	m_pRenderMan->FlushAll();

	if (!pMatrix)
		m_WorldMatrix.Identity();
	else
		memcpy(&m_WorldMatrix, pMatrix, sizeof(Core::IMatrix));

	CalcWorldViewProjMatrix();

	if (m_bUsingShaders)
	{
		Core::IMatrix mat;
		mat.TransposeOf( m_WorldViewProjMatrix );
		m_pDevice->SetVertexShaderConstantF(0, (float *)&mat, 4);
	}
	m_pDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&m_WorldMatrix);
}

Core::IMatrix& nD3DDriver::GetWorldMatrix( void )
{
	return m_WorldMatrix;
}

void nD3DDriver::SetViewLookAt( const Core::IVector3f &pos,
								const Core::IVector3f &lookat,
								const Core::IVector3f &up )
{
	m_View3DMatrix.BuildViewMatrix( pos, lookat, up );
	
	if (!m_bUsingShaders)
		m_pDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)&m_View3DMatrix);

	CalcViewProjMatrix();
	CalcWorldViewProjMatrix();

	if (m_bUsingShaders)
	{
		Core::IMatrix mat;
		mat.TransposeOf( m_WorldViewProjMatrix );
		m_pDevice->SetVertexShaderConstantF(0, (float *)&mat, 4);
	}
}

void nD3DDriver::CalcViewProjMatrix( void )
{
	Core::IMatrix *view;
	Core::IMatrix *proj;
	Core::IMatrix *res;

	if (m_ViewMode == EVM_TWODIMENSION)
	{
		proj = &m_Proj2DMatrix;
		view = &m_View2DMatrix;
	}
	else
	{
		view = &m_View3DMatrix;
		
		if (m_ViewMode == EVM_ORTHOGONAL)
			proj = &m_ProjOrthoMatrix[m_nStage];
		else
			proj = &m_ProjPerspMatrix[m_nStage];
		
	}
	res = &m_ViewProjMatrix;
	(*res) = (*view) * (*proj);
}

void nD3DDriver::CalcWorldViewProjMatrix( void )
{
	Core::IMatrix *view;
	Core::IMatrix *proj;
	Core::IMatrix *res;
	Core::IMatrix *world;

	world = &m_WorldMatrix;

	if (m_ViewMode == EVM_TWODIMENSION)
	{
		proj = &m_Proj2DMatrix;
		view = &m_View2DMatrix;
	}
	else
	{
		view = &m_View3DMatrix;
		
		if (m_ViewMode == EVM_ORTHOGONAL)
			proj = &(m_ProjOrthoMatrix[m_nStage]);
		else
			proj = &(m_ProjPerspMatrix[m_nStage]);		
	}
	res = &m_WorldViewProjMatrix;
	(*res) = ((*world) * (*view)) * (*proj);
}

void nD3DDriver::SetDepthBufferMode( E_DEPTH_MODE edm )
{
	if (!m_bIsRunning) return;

	m_DepthMode = edm;

	if (edm == EDM_NONE)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	
	else if (edm == EDM_READ)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	else if (edm == EDM_READWRITE)
	{
		m_pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

E_DEPTH_MODE nD3DDriver::GetDepthBufferMode( void )
{
	return m_DepthMode;
}


/**************************************
  Font
 **************************************/

AI_RESULT nD3DDriver::RegisterFont( const c8 *fontName, bool bBold, bool bItalic, bool bUnderline,
									u32 nSize, u32 *fontID )
{
	if (!fontID) return AI_INVALID;

	HDC hdc = GetDC(NULL);
	s32 nHeight = -MulDiv(nSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	
	u32 nWeight = bBold ? FW_BOLD : FW_NORMAL;

	HFONT font = ::CreateFont(nHeight, 0, 0, 0,
							  nWeight,
							  bItalic, bUnderline, false,
							  DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
							  CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
							  VARIABLE_PITCH, fontName);
	
	if (!font) return AI_FAIL;

	m_pFont = (LPD3DXFONT *)realloc(m_pFont, sizeof(LPD3DXFONT)*(m_nNumFonts+1));

	HRESULT hr = D3DXCreateFont(m_pDevice, font, &m_pFont[m_nNumFonts]);

	DeleteObject(font);

	if (FAILED(hr))
		return AI_FAIL;
	else
	{
		(*fontID) = m_nNumFonts;
		m_nNumFonts++;
	}

	return AI_OK;
}

AI_RESULT nD3DDriver::DrawText( s32 x, s32 y, const c8 *text, ... )
{
	char teks[1024];
	char *pArgs;

	pArgs = (char *)&text+sizeof(text);
	vsprintf(teks, text, pArgs);

	bool bShaders = UsingShaders();
	if (bShaders) EnableShaders(false);

	if (m_nActiveFont >= m_nNumFonts) return AI_FAIL;

	bool fog = IsFogEnable();
	if (fog) SetFogEnable(false);

	RECT rc = { x, y, 0, 0 };
	m_pFont[m_nActiveFont]->Begin();

		m_pFont[m_nActiveFont]->DrawText(teks, -1, &rc, DT_SINGLELINE | DT_CALCRECT, 0);
		m_pFont[m_nActiveFont]->DrawText(teks, -1, &rc, DT_SINGLELINE , m_FontColor.Color); 

	m_pFont[m_nActiveFont]->End();

	if (fog) SetFogEnable(true);

	if (bShaders) EnableShaders(true);
	return AI_OK;
}

void nD3DDriver::ReleaseFont( void )
{
	for (u32 i=0; i<m_nNumFonts; i++)
	{
		SAFE_RELEASE(m_pFont[i]);
	}
	SAFE_DELETE(m_pFont);
}


/*********************************************************
  Initialization 
 *********************************************************/
 
AI_RESULT CreateRenderDevice(HINSTANCE hInstance, 
							 IRenderDevice **pDevice)
{
	if (!*pDevice)
	{
		*pDevice = new nD3DDriver(hInstance);
		return AI_OK;
	}
	return AI_FAIL;
}

AI_RESULT ReleaseRenderDevice(IRenderDevice **pDevice)
{
	if (!*pDevice)
	{
		return AI_FAIL;
	}

	delete *pDevice;
	*pDevice = NULL;
	return AI_OK;
}

}	// namespace Video
}	// namespace Ahmad