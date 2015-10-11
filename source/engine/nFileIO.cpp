/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "nFileIO.h"

namespace Ahmad
{
namespace IO
{

AI_RESULT nFileIO::Open( const c8 *cFileName, E_FILE_IO status )
{
	if (!cFileName) return AI_FAIL;
	switch (status)
	{
	case EFI_IO_Read:
		{
			m_pFile = fopen( cFileName, "rb" );
		} break;

	case EFI_IO_Write:
		{
			m_pFile = fopen( cFileName, "wb" );
		} break;

	case EFI_IO_ReadWrite:
		{
			m_pFile = fopen( cFileName, "r+b" );
		} break;

	case EFI_IO_Append:
		{
			m_pFile = fopen( cFileName, "ab" );
		} break;

	default: return AI_FAIL;
	}

	m_cFileName = new c8[strlen(cFileName)+1];
	memcpy(m_cFileName, cFileName, strlen(cFileName)+1);

	if (!m_pFile) return AI_FAIL;
	fseek(m_pFile, 0, SEEK_END);
	m_nSize = ftell(m_pFile);
	fseek(m_pFile, 0, SEEK_SET);

	return AI_OK;
}

AI_RESULT nFileIO::Close( void )
{
	if (!m_pFile) return AI_FAIL;
	delete [] m_cFileName;

	fclose(m_pFile);
	return AI_OK;
}

const c8 *nFileIO::GetFileName( void )
{
	return m_cFileName;
}

u32 nFileIO::Write( void *data, u32 size )
{
	if ( (!m_pFile) && ((m_eStatus == EFI_IO_None) || (m_eStatus == EFI_IO_Read)) ) return -1;

	return (u32)fwrite( data, 1, size, m_pFile );
}

u32 nFileIO::WriteString( c8 *str )
{
	if ( (!m_pFile) && ((m_eStatus == EFI_IO_None) || (m_eStatus == EFI_IO_Read)) ) return -1;
	return (u32)fwrite( (void *)str, 1, strlen(str), m_pFile );
}

u32 nFileIO::Read( void *data, u32 size )
{
	if ( (!m_pFile) && ((m_eStatus == EFI_IO_None) || (m_eStatus == EFI_IO_Write)) ) return -1;

	return (u32)fread( data, 1, size, m_pFile );
}

u32 nFileIO::ReadString( c8 *str, u32 count )
{
	u32 ret = Read( (void *)str, count );
	str[count]=0;
	return ret;
}

u32 nFileIO::Seek( s32 pos, bool IsRelative )
{
	if (!m_pFile) return -1;

	return fseek( m_pFile, pos, IsRelative ? SEEK_CUR : SEEK_SET );
}

bool nFileIO::IsFileExists( const c8 *cFileName )
{
	FILE *fp;
	fp = fopen( cFileName, "rb" );

	if (!fp)
	{
		fclose(fp);
		return true;
	}

	return false;
}

/*************************
  FileIO creation
 *************************/
IFileIO	*createFileIO( const c8 *cFileName, E_FILE_IO status )
{
	nFileIO	*fp = new nFileIO( cFileName, status );
	if (fp->IsOpen())
		return fp;
	
	SAFE_THROW(fp);

	return NULL;
}

}	// namespace IO
}	// namespace Ahmad