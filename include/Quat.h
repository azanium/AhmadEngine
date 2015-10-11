/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _QUAT_H_
#define _QUAT_H_

#include "IVector.h"

namespace Ahmad
{
namespace Core
{

// Quaternion class
template <class T>
class IQuat
{
public:
	IVector3<T>	v;

	IQuat( void ) {}
	~IQuat( void ) {}
};

}	// namespace Core
}	// namespace Ahmad

#endif	// _QUAT_H_