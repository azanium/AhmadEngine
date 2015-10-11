/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _NFILEIO_H_
#define _NFILEIO_H_

#include <stdio.h>
#include "IFileIO.h"

namespace Ahmad
{
namespace IO
{

class nFileIO: public IFileIO
{
public:
	nFileIO( void ): m_nSize(0), m_pFile(NULL), m_eStatus(EFI_IO_Read) {};
	
	nFileIO( const c8 *cFileName, E_FILE_IO status ): 
	m_nSize(0), m_pFile(NULL), m_eStatus(EFI_IO_Read)
	{
		Open( cFileName, status );
	}
	
	~nFileIO( void ) { if (IsOpen()) Close(); };

	// Plain file
	AI_RESULT	Open( const c8 *cFileName, E_FILE_IO status );
	AI_RESULT	Close( void );
	const c8	*GetFileName( void );
	u32			Write( void *data, u32 size );
	u32			WriteString( c8 *str );
	u32			Read( void *data, u32 size );
	u32			ReadString( c8 *str, u32 count );
	u32			Seek( s32 pos, bool IsRelative = false );
	u32			Size( void ) { if (m_pFile) return m_nSize; else return 0; }
	bool		IsOpen( void ) { if (m_pFile) return TRUE; else return FALSE; }
	void		Flush( void ) { if (m_pFile) fflush(m_pFile); }
	bool		IsFileExists( const c8 *cFilename );

protected:
	u32			m_nSize;
	FILE		*m_pFile;
	E_FILE_IO	m_eStatus;
	c8			*m_cFileName;
};

}	// namespace IO
}	// namespace Ahmad

#endif	// _NFILESYSTEM_H_
