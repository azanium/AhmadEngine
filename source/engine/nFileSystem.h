/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NFILESYSTEM_H_
#define _NFILESYSTEM_H_

#include "IFileSystem.h"

namespace Ahmad
{
namespace IO
{

#define	IO_MAX_PATH		1024

class nFileSystem: public IFileSystem
{
public:
	nFileSystem( void ) {}
	~nFileSystem( void ) {}

	IFileIO		*Open( const c8 *fname, E_FILE_IO efiType );

	// Directory
	const c8   *GetWorkingDirectory( void );
	bool		SetWorkingDirectory( const c8 *cNewDir );

protected:
	c8			m_cWorkingDirectory[IO_MAX_PATH];
};

}	// namespace IO
}	// namespace Ahmad

#endif