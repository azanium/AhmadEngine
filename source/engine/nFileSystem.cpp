/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifdef WIN32
#include <direct.h>
#endif

#include "nFileSystem.h"
#include "nFileIO.h"

namespace Ahmad
{
namespace IO
{

IFileIO	*nFileSystem::Open( const c8 *fname, E_FILE_IO efiType )
{
	// TODO: add ZIP Reader filesystem
	return new nFileIO( fname, efiType );
}

const c8 *nFileSystem::GetWorkingDirectory( void )
{
#ifdef WIN32
	_getcwd(m_cWorkingDirectory, IO_MAX_PATH);
	return m_cWorkingDirectory;
#endif

	return NULL;
}

bool nFileSystem::SetWorkingDirectory( const c8 *cNewDir )
{
#ifdef WIN32
	return (_chdir( cNewDir ) == 0);
#endif
	
	return false;
}


/**************************************
  FileSystem creation
 **************************************/

IFileSystem *createFileSystem( void )
{
	return new nFileSystem();
}

}	// namespace IO
}	// namespace Ahmad