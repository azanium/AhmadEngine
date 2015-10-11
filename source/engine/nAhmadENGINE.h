/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NAHMADENGINE_H_
#define _NAHMADENGINE_H_

#include "Types.h"
#include "IAhmadENGINE.h"

namespace Ahmad
{

//! This is the wrapper for renderDevice
class nAhmadENGINE: public IAhmadENGINE
{
public:
	nAhmadENGINE( );
	~nAhmadENGINE( void );



	AI_RESULT				CreateDevice( const c8 *deviceName );

	void					Release( void );

#ifdef WIN32
	HINSTANCE				GetModule( void ) { return m_hDLL; }
#endif

	Video::IRenderDevice	*GetRenderDevice( void ) { return m_pDevice; }

	IO::IFileSystem			*GetFileSystem( void ) { return m_pFileSystem; }

private:

#ifdef WIN32
	HMODULE			m_hDLL;
#endif
	
	AI_RESULT				createRenderDevice( const c8 *deviceName );
	void					releaseRenderDevice( void );

	Video::IRenderDevice	*m_pDevice;
	IO::IFileSystem			*m_pFileSystem;
};

}	// namespace Ahmad

#endif	// _RENDER_H_