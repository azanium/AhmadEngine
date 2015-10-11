/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "AhmadEngine.h"
#include "resource.h"
#include <conio.h>

#pragma comment (lib, "Engine.lib")

// The main namespace
using namespace Ahmad;

// The platform namespace
using namespace Ahmad::Platform;

// The Video namespace (RenderDevice is here)
using namespace Ahmad::Video;

// The Core namespace (math function, etc.)
using namespace Ahmad::Core;

// The IO namespace (FileManager, etc.)
using namespace Ahmad::IO;

#define CLASSNAME	"AhmadENGINE"
#define	WIDTH		450
#define HEIGHT		400

// handle for the engine
IAhmadENGINE	*g_Engine = NULL;

// handle for the render device
IRenderDevice	*g_Device = NULL;

// handle for the application instance
HINSTANCE		g_hInstance = NULL;

// handle for the main window
HWND			g_hWnd = NULL;

// status of the active window
BOOL			g_bActive = FALSE;

// flag for fullscreen
bool			g_bFullscreen = false;

// functions declarations
void CreateCube( const Core::IVector3f &pos, f32 width, f32 height, f32 depth,
				 SVertexNormal *pVerts, u16 *pIndices, bool bInsideVisible );

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int Init();
bool startProgram(void);
void endProgram(void);

// entry point for console
int main()
{
	return Init();
}

// entry point for win32
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return Init();
}

int Init()
{
	SVertexNormal	verts[24];
	u16				indis[42];
	HWND			hWnd;
	MSG				msg;

	// get our application instance
	HINSTANCE hInstance = GetModuleHandle(0);
	
	// create the AhmadENGINE
	g_Engine = createAhmadENGINE( "Direct3D" );
	if (!g_Engine) return 1;

	// specific platform form creation for Win32
	c8	title[100] = "AhmadENGINE - Per-Vertex Directional Lighting ";

	// Buat form secara otomatis
	hWnd = IForms::createForm( hInstance, CLASSNAME, title, WIDTH, HEIGHT, g_bFullscreen, WndProc );
	if (!hWnd) return 0;

	// global handle
	g_hInstance = hInstance;
	g_hWnd = hWnd;

	// inisialiasi game engine
	if (!startProgram()) return 1;
	
	c8 *stShaders = new c8[100];

	// Check if the shaders v1.1 exists?
	if (!g_Device->ShaderVersionAvailable(ESV_VSHADER_1_1) || 
		!g_Device->ShaderVersionAvailable(ESV_PSHADER_1_1))
	{
		IForms::AlertOK("Your graphics card doesn't support Vertex and Pixel Shader v1.1", "WARNING!");
		stShaders = "No shaders support.";
		g_Device->EnableShaders(false);
	}
	else
	{
		stShaders = "Using Vertex Shader v1.1 and Pixel Shader v1.1";
	}

	// Grab the Render Manager
	IRenderMan	*render = g_Device->GetRenderMan();

	// Grab the Image Manager
	IImageManager *iman = g_Device->GetImageManager();

	// Create a surface from a file
	ISurface *photo = iman->LoadImageFromFile( "../media/Suhendra Ahmad.bmp" );

	// Create a surface from a file
	ISurface *box = iman->LoadImageFromFile("../media/box.bmp");

	// If one of the surface is not created, display message
	if (!photo || !box) 
	{
		IForms::AlertOK("texture not found", "WARNING!");
		return 1;
	}
	
	// Get the skin manager
	ISkinManager *skin = g_Device->GetSkinManager();

	// Add a skin
	u32 skinID1;
	skin->AddSkin( IMaterial(), &skinID1 );

	// Add a texture from box surface
	skin->AddTexture(skinID1, box);
	
	// Add a texture from box surface
	skin->AddTexture(skinID1, photo);

	// If the surface doesn't support alpha channel, we provide one
	photo->SetTransparency(50);

	// Aktifkan fog
	g_Device->SetFogEnable( true );

	// Ambil seting fog
	SFog fog = g_Device->GetFog();
	
	// Ubah batas akhir fog
	fog.Density = 0.02f;
	fog.PixelFog = true;
	fog.efcType = EFC_EXP2;
	
	// Ganti warna fog
	fog.Color.Set(200,200,200,255);

	// Ubah seting fog
	g_Device->SetFog(fog);

	// Set view stage 0 dan 1, dengan viewport default
	g_Device->SetViewStage(0, 45.0f, NULL);
	g_Device->SetTextureOperation(1, ETO_DARKMAP);

	// Buat matrix untuk rotasi
	IMatrix mat1;

	// rotation angle
	f32 angle = 0;

	// Set the clear color
	g_Device->SetClearColori(200, 200, 200);
		
	// id for the cube 
	u32 sid1;
	
	// create a cube
	CreateCube(IVector3f(0.0f, 0.0f, 0.0f), 8.0f, 8.0f, 8.0f, verts, indis, true);

	// create the static buffer for the cube
	render->CreateStaticBuffer(EVT_VERTEXNORMAL, verts, 24, indis, 42, skinID1, &sid1);
	
	// id for vertex shader and pixels shader
	u32 vid, pid;

	// Load the vertex shader from the file
	g_Device->CreateVertexShader("../media/pervertex.vsh", false, &vid);

	// Load the pixel shader from the file
	g_Device->CreatePixelShader("../media/pervertex.psh", false, &pid);

	// Set the ambient light
	g_Device->SetAmbientLightF(0.3f, 0.3f, 0.3f);

	// The vector for vector in world space and object space
	IVector3f lightDir(0.0f, 0.0f, 1.0f), vlight;

	// The light matrix in object space
	IMatrix   mLight;
	
	// Set the camera lookat
	g_Device->SetViewLookAt(IVector3f(0,5.0f,-15.0f), IVector3f(0,0,0), IVector3f(0,1.0f,0));
    
	bool done = false;
	while (!done)
	{
		// ambil pesan, hapus queue setelah diproses dengan flag PM_REMOVE
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}	
		}
		else
		{	
			//if (g_bActive)	// draw only when it is focused, otherwise it will slow down our Windows
			{
				// Draw only when the device is available
				if (g_Device->IsRunning())
				{				
					// Begin the rendering
					g_Device->BeginRendering(true, true, false);
			
					angle += 0.05f;
					if (angle>2*AI_PI) angle = 0;

					mat1.Identity();

					// y rotation 
					mat1.RotateY(angle); 

					// Set the world matrix
					g_Device->SetWorldMatrix(&mat1);

					// Calculate the inverse matrix for the light
					mLight.InverseOf(mat1);
					vlight = mLight*lightDir;
					g_Device->SetVertexShaderConstantF(20, 1, &vlight);
				
					// Activate vertex shader and pixel shader
					g_Device->ActivateVertexShader(vid, EVT_VERTEXNORMAL);
					g_Device->ActivatePixelShader(pid);

					// Render the Static Vertex Cache
					render->Render(sid1);

					// set the font color
					g_Device->SetFontColor(0, 44, 192, 255);

					// activate the font
					g_Device->SetFontActive( FONT_ARIAL_BOLD );

					// draw the text
					g_Device->DrawText(5, 5, "Per-Vertex Directional Lighting (Using Static Vertex Cache)" );
					g_Device->DrawText(5, 20, "EXP2 Fog Density 0.04f White" );

					// activate the font
					g_Device->SetFontActive( FONT_ARIAL_NORMAL );

					// Set the font color
					g_Device->SetFontColor(255, 10, 10, 255);

					g_Device->DrawText(5, 45, stShaders );
					
					// end the rendering
					g_Device->EndRendering();
				}
			}
		}
	}

	endProgram();
	return (int)msg.wParam;
}

bool startProgram(void)
{
	// Get the render device
	g_Device = g_Engine->GetRenderDevice();
	
	// activate the render device
	g_Device->InitDriver(g_hWnd, IDimension<u32>(WIDTH, HEIGHT), 16, 0, g_bFullscreen);

	return TRUE;
}

void endProgram(void)
{
	// throw away the engine
	SAFE_THROW(g_Engine);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{

	case WM_ACTIVATE:
		{
			g_bActive = (BOOL)LOWORD(wParam);
		} break;

	case WM_KEYDOWN:
		{
			if (wParam == VK_ESCAPE) PostQuitMessage(0);
			if (wParam == VK_F1) 
			{
				IForms::moveForm(hWnd, 0, 0, 400, 400);
			}

		} break;

	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
	return 0;
}

void CreateCube( const Core::IVector3f &pos, f32 width, f32 height, f32 depth,
				 SVertexNormal *pVerts, u16 *pIndices, bool bInsideVisible )
{
	u16	pI[42] = {  0, 2, 1, 0, 3, 2, 4, 5, 6, 5, 7, 6,
					8, 9, 10, 9, 11, 10, 12, 13, 14, 14, 15, 12,
					16, 18, 17, 17, 18, 19, 22, 21, 20, 22, 23, 21 };

	u16	pI2[42] = {  1, 2, 0, 2, 3, 0, 6, 5, 4, 6, 7, 5,
					 10, 9, 8, 10, 11, 9, 14, 13, 12, 12, 15, 14,
					 17, 18, 16, 19, 18, 17, 20, 21, 22, 21, 23, 22 };

	if (bInsideVisible)
		memcpy(pIndices, &pI2, sizeof(u16)*42);
	else
		memcpy(pIndices, &pI, sizeof(u16)*42);

	f32 hw = width/2;
	f32 hh = height/2;
	f32 hd = depth/2;
	f32 n;
	if (bInsideVisible) n = 1; else n = -1;

	// top
	pVerts[0].Pos = IVector3f(pos.x-hw, pos.y+hh, pos.z-hd);
	pVerts[0].TexCoord = IVector2f(0.0f, 0.0f);
	pVerts[1].Pos = IVector3f(pos.x-hw, pos.y+hh, pos.z+hd);
	pVerts[1].TexCoord = IVector2f(0.0f, 1.0f);
	pVerts[2].Pos = IVector3f(pos.x+hw, pos.y+hh, pos.z+hd);
	pVerts[2].TexCoord = IVector2f(1.0f, 1.0f);
	pVerts[3].Pos = IVector3f(pos.x+hw, pos.y+hh, pos.z-hd);
	pVerts[3].TexCoord = IVector2f(1.0f, 0.0f);
	pVerts[0].Normal = pVerts[1].Normal = pVerts[2].Normal = pVerts[3].Normal = IVector3f(0,n,0);

	// back
	pVerts[4].Pos = IVector3f(pos.x-hw, pos.y+hh, pos.z+hd);
	pVerts[4].TexCoord = IVector2f(0.0f, 0.0f);
	pVerts[5].Pos = IVector3f(pos.x+hw, pos.y+hh, pos.z+hd);
	pVerts[5].TexCoord = IVector2f(1.0f, 0.0f);
	pVerts[6].Pos = IVector3f(pos.x-hw, pos.y-hh, pos.z+hd);
	pVerts[6].TexCoord = IVector2f(0.0f, 1.0f);
	pVerts[7].Pos = IVector3f(pos.x+hw, pos.y-hh, pos.z+hd);
	pVerts[7].TexCoord = IVector2f(1.0f, 1.0f);
	pVerts[4].Normal = pVerts[5].Normal = pVerts[6].Normal = pVerts[7].Normal = IVector3f(0,0,n);

	// left
	pVerts[8].Pos = IVector3f(pos.x-hw, pos.y+hh, pos.z-hd);
	pVerts[8].TexCoord = IVector2f(0.0f, 0.0f);
	pVerts[9].Pos = IVector3f(pos.x-hw, pos.y+hh, pos.z+hd);
	pVerts[9].TexCoord = IVector2f(1.0f, 0.0f);
	pVerts[10].Pos = IVector3f(pos.x-hw, pos.y-hh, pos.z-hd);
	pVerts[10].TexCoord = IVector2f(0.0f, 1.0f);
	pVerts[11].Pos = IVector3f(pos.x-hw, pos.y-hh, pos.z+hd);
	pVerts[11].TexCoord = IVector2f(1.0f, 1.0f);
	pVerts[8].Normal = pVerts[9].Normal = pVerts[10].Normal = pVerts[11].Normal = IVector3f(-n,0,0);

	// bottom
	pVerts[12].Pos = IVector3f(pos.x-hw, pos.y-hh, pos.z-hd);
	pVerts[12].TexCoord = IVector2f(0.0f, 0.0f);
	pVerts[13].Pos = IVector3f(pos.x-hw, pos.y-hh, pos.z+hd);
	pVerts[13].TexCoord = IVector2f(0.0f, 1.0f);
	pVerts[14].Pos = IVector3f(pos.x+hw, pos.y-hh, pos.z+hd);
	pVerts[14].TexCoord = IVector2f(1.0f, 1.0f);
	pVerts[15].Pos = IVector3f(pos.x+hw, pos.y-hh, pos.z-hd);
	pVerts[15].TexCoord = IVector2f(1.0f, 0.0f);
	pVerts[12].Normal = pVerts[13].Normal = pVerts[14].Normal = pVerts[15].Normal = IVector3f(0,-n,0);

	// right
	pVerts[16].Pos = IVector3f(pos.x+hw, pos.y+hh, pos.z-hd);
	pVerts[16].TexCoord = IVector2f(0.0f, 0.0f);
	pVerts[17].Pos = IVector3f(pos.x+hw, pos.y+hh, pos.z+hd);
	pVerts[17].TexCoord = IVector2f(1.0f, 0.0f);
	pVerts[18].Pos = IVector3f(pos.x+hw, pos.y-hh, pos.z-hd);
	pVerts[18].TexCoord = IVector2f(0.0f, 1.0f);
	pVerts[19].Pos = IVector3f(pos.x+hw, pos.y-hh, pos.z+hd);
	pVerts[19].TexCoord = IVector2f(1.0f, 1.0f);
	pVerts[16].Normal = pVerts[17].Normal = pVerts[18].Normal = pVerts[19].Normal = IVector3f(n,0,0);

	// front
	pVerts[20].Pos = IVector3f(pos.x-hw, pos.y+hh, pos.z-hd);
	pVerts[20].TexCoord = IVector2f(0.0f, 0.0f);
	pVerts[21].Pos = IVector3f(pos.x+hw, pos.y+hh, pos.z-hd);
	pVerts[21].TexCoord = IVector2f(1.0f, 0.0f);
	pVerts[22].Pos = IVector3f(pos.x-hw, pos.y-hh, pos.z-hd);
	pVerts[22].TexCoord = IVector2f(0.0f, 1.0f);
	pVerts[23].Pos = IVector3f(pos.x+hw, pos.y-hh, pos.z-hd);
	pVerts[23].TexCoord = IVector2f(1.0f, 1.0f);
	pVerts[20].Normal = pVerts[21].Normal = pVerts[22].Normal = pVerts[23].Normal = IVector3f(0,0,-n);
}