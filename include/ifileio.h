/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IFILEIO_H_
#define _IFILEIO_H_

#include "Types.h"
#include "IBuyut.h"

namespace Ahmad
{
namespace IO
{

enum E_FILE_IO {
	EFI_IO_None,
	EFI_IO_Read,
	EFI_IO_Write,
	EFI_IO_ReadWrite,
	EFI_IO_Append
};

class IFileIO:	public IBuyut
{
public:
	IFileIO( void ) {};
	virtual ~IFileIO( void ) {};
	
	// Plain file
	virtual AI_RESULT	Open( const c8 *cFileName, E_FILE_IO status ) = 0;
	virtual AI_RESULT	Close() = 0;
	virtual const c8	*GetFileName( void ) = 0;
	virtual u32			Write( void *data, u32 size ) = 0;
	virtual u32			WriteString( c8 *str ) = 0;
	virtual u32			Read( void *data, u32 size ) = 0;
	virtual u32			ReadString( c8 *str, u32 count ) = 0;
	virtual u32			Seek( s32 pos, bool IsRelative = false ) = 0;
	virtual u32			Size( void ) = 0;
	virtual bool		IsOpen( void ) = 0;
	virtual	void		Flush( void ) = 0;
	virtual	bool		IsFileExists( const c8 *cFilename ) = 0;

};

IFileIO	*createFileIO( const c8 *cFileName, E_FILE_IO status );

}	// namespace IO
}	// namespace Ahmad

#endif