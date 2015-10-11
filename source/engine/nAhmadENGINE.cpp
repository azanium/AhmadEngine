/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nAhmadENGINE.h"
#include "nFileSystem.h"
#include "IForms.h"

namespace Ahmad
{

nAhmadENGINE::nAhmadENGINE( void )
{
	m_hDLL = NULL;

	m_pDevice = NULL;
}

nAhmadENGINE::~nAhmadENGINE( void )
{
	Release();
}

AI_RESULT nAhmadENGINE::CreateDevice( const c8 *deviceName )
{
	AI_RESULT ar;

	/*****************************
	  Render Device Initialization
	 *****************************/
	if (FAILED(ar = createRenderDevice( deviceName ))) return ar;

	/*****************************
	  File System Initialization
	 *****************************/
	m_pFileSystem = IO::createFileSystem();

	return AI_OK;
}

void nAhmadENGINE::Release( void )
{
	/*****************************
	  Render Device Destruction
	 *****************************/
	releaseRenderDevice();
	
	/*****************************
	  FileSystem Destruction
	 *****************************/
	SAFE_THROW(m_pFileSystem);
}


/*******************************************************************
   Render Device DLL Loader
 *******************************************************************/

AI_RESULT nAhmadENGINE::createRenderDevice( const c8 *deviceName )
{
	typedef AI_RESULT ( *CREATERENDERDEVICE )( HINSTANCE, Video::IRenderDevice **, IAhmadENGINE * );

	if (strcmp(deviceName, "Direct3D") == 0)
	{
        m_hDLL = LoadLibrary("renderD3D.dll");
		if (!m_hDLL)
		{
			Platform::IForms::AlertOK( "nAhmadENGINE::CreateDevice(): Error loading library.", "ERROR" );
			return AI_FAIL;
		}
	}
	else
	{
		Platform::IForms::AlertOK( "nAhmadENGINE::CreateDevice(): Rendering API not supported.", "ERROR" );
		return AI_FAIL;
	}

	CREATERENDERDEVICE _CreateRenderDevice = 0;
	HRESULT hr = 0;
	
	_CreateRenderDevice = (CREATERENDERDEVICE)GetProcAddress(m_hDLL, "CreateRenderDevice");
	if (!_CreateRenderDevice)
	{
		MessageBox(0, "nAhmadENGINE::CreateDevice(): Error sniffing library.", "ERROR", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	hr = _CreateRenderDevice(m_hDLL, &m_pDevice, this);
	if (FAILED(hr))
	{
		MessageBox(0, "nAhmadENGINE::CreateDevice(): Device creation error.", "ERROR", MB_OK | MB_ICONERROR);
		m_pDevice = NULL;
		return E_FAIL;
	}

	return S_OK;
}

void nAhmadENGINE::releaseRenderDevice( void )
{
	typedef AI_RESULT ( *RELEASERENDERDEVICE )( Video::IRenderDevice ** );
	RELEASERENDERDEVICE _ReleaseRenderDevice = 0;
	HRESULT hr = 0;

	if (m_hDLL)
	{
		_ReleaseRenderDevice = (RELEASERENDERDEVICE)GetProcAddress(m_hDLL, "ReleaseRenderDevice");
		if (!_ReleaseRenderDevice)
		{
			Platform::IForms::AlertOK( "nAhmadENGINE::~CreateDevice(): Error sniffing release().", "ERROR" );
			return;
		}
	}

	if (m_pDevice)
	{
		hr = _ReleaseRenderDevice(&m_pDevice);
		m_pDevice = NULL;
	}

}


/*****************************
  AhmadENGINE creation 
 *****************************/

AI_API IAhmadENGINE *createAhmadENGINE( const c8 *deviceName )
{
	nAhmadENGINE	*gR;
	AI_RESULT		res;
	gR = new nAhmadENGINE( );
	res = gR->CreateDevice( deviceName );

	if (res != AI_OK) return NULL;

	return gR;
}

}	// namespace Ahmad