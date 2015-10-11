/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "AhmadEngine.h"
#include <mmsystem.h>

#pragma comment (lib, "Engine.lib")
#pragma comment (lib, "winmm")

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
#define	WIDTH		800
#define HEIGHT		600

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
BOOL			g_bFullscreen = FALSE;

// function declarations
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
	HWND			hWnd;
	MSG				msg;
	
	// get our application instance
	HINSTANCE hInstance = GetModuleHandle(0);
	
	// create the AhmadENGINE
	g_Engine = createAhmadENGINE( "Direct3D" );
	if (!g_Engine) return 1;

	// specific platform form creation for Win32
	c8	title[100] = "AhmadENGINE - Multi Render";

	// Buat form secara otomatis
	hWnd = IForms::createForm( hInstance, CLASSNAME, title, WIDTH, HEIGHT, g_bFullscreen, WndProc );
	if (!hWnd) return 0;

	// global handle
	g_hInstance = hInstance;
	g_hWnd = hWnd;

	// inisialiasi game engine
	if (!startProgram()) return 1;

	// Grab the Render Manager
	IRenderMan	*render = g_Device->GetRenderMan();

	// Grab the Image Manager
	IImageManager *iman = g_Device->GetImageManager();

	// Create a surface from a file
	ISurface *surface = iman->LoadImageFromFile( "../media/Suhendra Ahmad.bmp" );

	// Create a surface from a file
	ISurface *box = iman->LoadImageFromFile("../media/box.bmp");

	// If one of the surface is not created, display message
	if (!surface || !box) 
	{
		IForms::AlertOK("texture not found", "WARNING!");
		return 1;
	}
	
	// Get the skin manager
	ISkinManager *skin = g_Device->GetSkinManager();

	// Add a skin
	u32 skinID, skinID2;
	skin->AddSkin( IMaterial(), &skinID);
	skin->AddSkin( IMaterial(), &skinID2);

	// Add a texture from a surface
	skin->AddTexture(skinID, box);
	
	// Add a texture from a surface
	skin->AddTexture(skinID2, surface);

	// Add a texture from a surface
	skin->AddTexture(skinID2, box);

	// Get the texture
	ITexture *tex = skin->GetTexture(0);

	// Activate the texture operation using darkmapping
	g_Device->SetTextureOperation(1, ETO_DARKMAP);

	// If the surface doesn't support alpha channel, we provide one
	surface->SetTransparency(100);

	// Aktifkan fog
	g_Device->SetFogEnable( true );

	// Ambil seting fog
	SFog fog = g_Device->GetFog();
	
	// Ubah batas akhir fog
	fog.EndFog = 25.0f;
	fog.Density = 0.04f;
	fog.efcType = EFC_EXP2;
	
	// Ganti warna fog
	fog.Color.Set(255,255,255,255);

	// Ubah seting fog
	g_Device->SetFog(fog);

	// Set view stage 0 dan 1, dengan viewport default
	g_Device->SetViewStage(0, 45.0f, NULL);
	g_Device->SetViewStage(1, 45.0f, NULL);

	// Buat matrix untuk rotasi
	IMatrix mat1;

	// sudut rotasi
	f32 angle;

	// time based movement
	u32 time;

	// Set the clear color
	g_Device->SetClearColori(200, 200, 200);
	g_Device->EnableShaders(false);
    
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
					for (u32 i=0; i<4; i++) {

					// Aktifkan jendela pertama
					g_Device->UseWindow(i);
					
					// Begin the rendering
					g_Device->BeginRendering(true, true, true);

					// calculate the time per milisecond
					time = timeGetTime() % 2000;

					// calculate the angle based on the time
					angle = time * AI_2PI / 2000.0f;

					mat1.Identity();
					switch (i)
					{
						// z rotation for window 0
						case 0:	mat1.RotateZ(angle); break;
						
						// x rotation for window 1
						case 1: mat1.RotateX(angle); break;

						// y rotation for window 2
						case 2: mat1.RotateY(angle); break;

						// z rotation for window 3
						case 3: mat1.RotateZ(angle); break;
					}

					// Set the world matrxi
					g_Device->SetWorldMatrix(&mat1);
	

					// first window
					if (i==0) 
					{							
						// activate the orthogonal projection by using stage 0
						g_Device->SetViewMode(0, EVM_ORTHOGONAL);

						// render a cube
						g_Device->DrawCube(IVector3f(0, 0, 0.0f), 150.0f, 150.0f, 1.0f, false, skinID);

						// set the view lookat
						g_Device->SetViewLookAt(IVector3f(0,0,-25.0f), IVector3f(0,0,0), IVector3f(0,1.0f,0));
					}
					else 
                    {
						// activate the perspective projection using stage 1
						g_Device->SetViewMode(1, EVM_PERSPECTIVE);

						// render cubes
						g_Device->DrawCube(IVector3f(0, 0, 0.0f), 8.0, 8.0, 8.0, true, skinID);
						g_Device->DrawCube(IVector3f(6.0f, 0, 0.0f), 4.0, 4.0, 4.0, true, skinID2);

						// set the view lookat
						g_Device->SetViewLookAt(IVector3f(0,15.0f,-25.0f), IVector3f(0,0,0), IVector3f(0,1.0f,0));
					}
								
					// activate the font
					g_Device->SetFontActive( FONT_VERDANA_BOLD );

					// set the font color
					g_Device->SetFontColor(0, 44, 192, 255);

					switch (i)
					{
						// draw the text for window 0
						case 0:	
						    g_Device->DrawText(5, 5, "Orthogonal Projection - Z Rotation" );
							g_Device->DrawText(5, 20, "EXP2 Fog Density 0.04f White" );
							break;

						// draw the text for window 1
						case 1: 
							g_Device->DrawText(5, 5, "Perspective Projection - X Rotation" );
							g_Device->DrawText(5, 20, "EXP2 Fog Density 0.04f White" );
							break;

						// draw the text for window 2
						case 2: 
							g_Device->DrawText(5, 5, "Perspective Projection - Y Rotation" );
							g_Device->DrawText(5, 20, "EXP2 Fog Density 0.04f White" );
							break;

						// draw the text for window 3
						case 3: 
							g_Device->DrawText(5, 5, "Perspective Projection - Z Rotation" );
							g_Device->DrawText(5, 20, "EXP2 Fog Density 0.04f White" );
							break;
					}

					// Activate the font
					g_Device->SetFontActive( FONT_VERDANA_NORMAL );

					// Draw the text
					g_Device->DrawText(90, 230, "Two-dimensional image");
					
					// Draw 2 dimensional image in 3D units
					g_Device->Draw2DImageU(surface, IRectf(0.0f, 0.7f, 0.23f, 1.0f));

					// end the rendering
					g_Device->EndRendering();
					}
				}
			}
		}
	}

	endProgram();
	return (int)msg.wParam;
}

bool startProgram(void)
{
	RECT	rcWnd;
	int		x=0, y=0;
	
	// Get the render device
	g_Device = g_Engine->GetRenderDevice();

	// ge the client rect
	GetClientRect(g_hWnd, &rcWnd);
	
	// if we don't use the fullscreen, create multiple windows
	if (!g_bFullscreen)
	{
	
	// create a handle to the child window
	HWND	hWnd3D[4];

	// 4 times of window
	for (int i=0; i<4; i++)
	{
		// calculate the x position
		if ( (i == 0) || (i == 2) ) x = 10;
		else x = rcWnd.right/2+10;

		// calculate the y position
		if ( (i == 0) || (i == 1) ) y = 10;
		else y = rcWnd.bottom/2+10;
		
		// create the child window
		hWnd3D[i] = CreateWindowEx(WS_EX_CLIENTEDGE,
								   TEXT("static"),
								   NULL,
								   WS_CHILD | SS_BLACKRECT | WS_VISIBLE,
								   x, y, rcWnd.right/2-20, rcWnd.bottom/2-20,
								   g_hWnd, NULL, g_hInstance, NULL);
	}
	// activate the render device for windowed mode
	
	g_Device->InitDriver(g_hWnd, hWnd3D, 4, 16, 0);
	}
	else
	// activate the render device for fullscreen mode
	g_Device->InitDriver(g_hWnd, IDimension<u32>(WIDTH, HEIGHT), 32, 0, g_bFullscreen);

	return true;
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