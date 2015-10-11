/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IMATERIAL_H_
#define _IMATERIAL_H_

#include "Types.h"
#include "IColor.h"

namespace Ahmad
{
namespace Video
{

class IMaterial
{
public:
	IColor	Ambient;
	IColor	Diffuse;
	IColor	Specular;
	IColor	Emissive;
	f32		fShininess;

	IMaterial( void )
	{
		Ambient.Set(255, 255, 255, 255);
		Diffuse.Set(255, 255, 255, 255);
		Specular.Set(0, 0, 0, 0);
		Emissive.Set(0, 0, 0, 0);
		fShininess = 0.0f;
	};

	IMaterial( const IColor *ambient, const IColor *diffuse,
			   const IColor *specular, const IColor *emissive,
			   f32 shininess )
	{
		Ambient = *ambient;
		Diffuse = *diffuse;
		Specular = *specular;
		Emissive = *emissive;
		fShininess = shininess;
	};

	inline void	Set( const IColor *ambient, const IColor *diffuse,
					 const IColor *specular, const IColor *emissive,
			        f32 shininess )
	{
		Ambient = *ambient;
		Diffuse = *diffuse;
		Specular = *specular;
		Emissive = *emissive;
		fShininess = shininess;
	};

	inline void operator =( const IMaterial &mat )
	{
		Ambient = mat.Ambient;
		Diffuse = mat.Diffuse;
		Specular = mat.Specular;
		Emissive = mat.Emissive;
		fShininess = mat.fShininess;
	};

	inline bool operator ==( const IMaterial &mat ) const
	{
		if ((Ambient == mat.Ambient) &&
			(Diffuse == mat.Diffuse) &&
			(Specular == mat.Specular) &&
			(Emissive == mat.Emissive) &&
			(fShininess == mat.fShininess))
			return true;

		return false;
	};

	inline bool operator !=( const IMaterial &mat ) const
	{
		if ((Ambient == mat.Ambient) &&
			(Diffuse == mat.Diffuse) &&
			(Specular == mat.Specular) &&
			(Emissive == mat.Emissive) &&
			(fShininess == mat.fShininess))
			return true;

		return false;
	};
};

}	// namespace Video
}	// namespace Ahmad

#endif