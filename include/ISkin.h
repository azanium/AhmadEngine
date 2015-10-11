/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _ISKIN_H_
#define _ISKIN_H_

namespace Ahmad
{
namespace Video
{

class ISkin
{
public:
	u32		Material;
	u32		Texture[8];

	ISkin( void ) {}
	ISkin( u32 mat )
	{
		Material = mat;
	};
	~ISkin( void ) {}

	inline void operator =( const ISkin &skin )
	{
		Material = skin.Material;
		for (u32 i=0; i<8; i++)
			Texture[i] = skin.Texture[i];
	};
};


}	// namespace Video
}	// namespace Ahmad

#endif	// _ISKIN_H_