/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _AHMADENGINE_H_
#define _AHMADENGINE_H_

/*************************************************
  
     Engine.dll 
 
  This is the core of AhmadENGINE
  all subsystems including renderer tied up into
  this core.
 *************************************************/
 
#include "Engine.h"


/*************************************************
  
     RenderD3D.dll 
     RenderOGL.dll <not yet implemented>
 
  This is the rendering subsytem of AhmadENGINE
  You can't use this subsystem directly without 
  using the core engine, you must use the core 
  engine to get the pointer to this subsystem
  by calling it like this:
  
  	IAhmadENGINE *engine = createAhmadENGINE(bla..bla..)
  	IRenderDevice *renderer = engine->GetRenderDevice();
  	
  That's the way it is
  
  To do next:
  	Smile *_* you're on AhmadEngine Camera
  	(Suhendra Ahmad - ceo @ ainasoft.com)
  
 *************************************************/
 
#include "RenderDevice.h"

#endif	// _AHMADENGINE_H_