/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IAHMADENGINE_H_
#define _IAHMADENGINE_H_

#include "Types.h"
#include "IBuyut.h"
#include "IRenderDevice.h"
#include "IFileSystem.h"

namespace Ahmad
{

/*****************************************************************
  This is the wrapper for all the sub-systems for AhmadENGINE
 *****************************************************************/

class IAhmadENGINE: public IBuyut
{
public:
	IAhmadENGINE( void ) {};
	virtual ~IAhmadENGINE( void ) {};


	virtual AI_RESULT				CreateDevice( const c8 *deviceName ) = 0;
	
	virtual void					Release( void ) = 0;
	
	virtual HINSTANCE				GetModule( void ) = 0;
	
	virtual Video::IRenderDevice	*GetRenderDevice( void ) = 0;
	
	virtual IO::IFileSystem			*GetFileSystem( void ) = 0;
	
};

AI_API IAhmadENGINE *createAhmadENGINE( const c8 *deviceName );

}	// namespace Ahmad

#endif	// _ENGINE_H_