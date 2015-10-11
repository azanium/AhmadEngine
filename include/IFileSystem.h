/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IFILESYSTEM_H_
#define _IFILESYSTEM_H_

#include "Types.h"
#include "IBuyut.h"
#include "IFileIO.h"

namespace Ahmad
{
namespace IO
{

class IFileSystem: public IBuyut
{
public:
	IFileSystem( void ) {}
	virtual ~IFileSystem( void ) {}

	virtual IFileIO		*Open( const c8 *fname, E_FILE_IO efiType ) = 0;

	// Directory
	virtual const c8   *GetWorkingDirectory( void ) = 0;
	virtual bool		SetWorkingDirectory( const c8 *cNewDir ) = 0;
};

IFileSystem *createFileSystem( void );

}	// namespace IO
}	// namespace Ahmad

#endif