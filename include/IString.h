/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _STRING_H_
#define _STRING_H_

#include "Types.h"

namespace Ahmad
{
namespace Core
{

class String
{
public:
	String( void ): str(0), len(0) {}

	String( const c8 *string ): str(0), len(0)
	{
		if (string) 
		{
			u32 l = (u32)strlen(string);

			reallocate( l+1 );

			memcpy(str, string, l);
			str[l] = 0;

			len = l;
		}
	};

	String( const String &string ): str(0), len(0)
	{
		*this = string;
	};

	~String( void )
	{
		if (str)
		{
			delete [] str;
			str = NULL;
		}
	}
	
	inline void operator =( const c8 *string )
	{
		if (!string) return;
		u32 l = (u32)strlen(string);

		reallocate( l+1 );

		memcpy(str, string, l);
		str[l] = 0;

		len = l;
	};

	inline void operator =( const String *string )
	{
		*this = *string;
	};


	inline bool operator ==( const c8 *string ) const
	{
		if (strcmp(str, string)==0)
			return true;
		return false;
	};

	inline bool operator ==( String &string ) const
	{
		if (*this == string.ToString())
			return true;
		return false;
	};

	inline bool operator !=( const c8 *string ) const
	{
		if (strcmp(str, string) != 0)
			return true;

		return false;
	};

	inline bool operator !=( String &string ) const
	{
		if (*this != string.ToString())
			return true;

		return false;
	};

	inline String operator +( const c8 *string )
	{
		c8 *tmp = new c8[strlen(str)+strlen(string)+1];
		memcpy(tmp, str, len+1);
		delete [] str;
		strcat(tmp, string);
		return *this;
	};

	inline String operator +( String &string )
	{
		const c8 *tmp = string.ToString();
		reallocate(string.Length()+len+1);
		for (u32 i=0; i<string.Length(); i++)
			str[len+i] = tmp[i];
		str[len+string.Length()] = 0x0;

		return *this;
	};

	inline c8	operator []( s32 index )
	{
		if (!str) return 0;

		return str[index];
	};

	const c8 *ToString( void )
	{
		return str;
	};

	u32	Length( void )
	{
		return len;
	};

private:

	bool reallocate( u32 size )
	{
		if (len<size)
		{
			str = (c8 *)realloc(str, size);
			return true;
		}
		return false;
	}

	c8	*str;
	u32	len;
};

}	// namespace Core
}	// namespace Ahmad

#endif	// _STRING_H_