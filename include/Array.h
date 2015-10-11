/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "Types.h"

namespace Ahmad
{
namespace Core
{

//! This is the class for dynamic array used by the most of the managers of the engine
//! Created by Suhendra Ahmad *_* on Monday, December 26th 2005 3:21pm
//! Enhanced for C++ used, created on Friday, December 30th 2005 10:31pm
template <class T>
class Array
{
private:
	T*	m_pData;
	u32	used;
	u32	allocated;
	u32	grow;

	bool reallocate( u32 newsize )
	{
		T *data_old = m_pData;
		m_pData = new T[newsize];
		if (data_old)
		{
			u32 size = used > newsize ? newsize : used;
			for (u32 i=0; i<size; i++)
				m_pData[i] = data_old[i];
		}
		delete [] data_old;
		allocated = newsize;
		return true;
	}

public:

	Array( void )
	{
		m_pData = NULL;
		used = 0;
		allocated = 0;
		grow = 20;
        reallocate(grow);
	};

	Array( u32 size )
	{
		m_pData = NULL;
		used = 0;
		allocated = 0;
		grow = size;
        reallocate(grow);
	};

	~Array( void )
	{
		if (m_pData) free(m_pData);
	};

	void operator=( Array<T> &other )
	{
		_AE_DEBUG_BREAK_(!m_pData);

		if (m_pData) 
			delete [] m_pData;

		if (other.Allocated() == 0)
			m_pData = 0;
		else
		{
			m_pData = 0;
			allocated = 0;
			reallocate(other.Allocated());
		}
		used = other.Length();

		for (u32 i=0; i<used; i++)
			m_pData[i] = other[i];
	};

	u32 AddLast( const T &item )
	{
		_AE_DEBUG_BREAK_(!m_pData);

		if (used+1 > allocated)
		{
			reallocate(allocated+grow);
		}
		
		m_pData[used++] = item;
		return used-1;
	};

	u32 AddFirst( const T &item )
	{
		if (used+1 > allocated)
			reallocate(allocated+grow);
		
		for (u32 i=0; i<used; i++)
			m_pData[i+1] = m_pData[i];

		used++;

		m_pData[0] = item;

		return 0;
	};

	T& operator []( u32 index )
	{		
		_AE_DEBUG_BREAK_(index>=used);
		
		return m_pData[index];
	};

	const T& operator []( u32 index ) const
	{		
		_AE_DEBUG_BREAK_(index>=used);

		return m_pData[index];
	};

	u32 Length( void ) const
	{
		return used;
	};

	u32 Allocated( void ) const
	{
		return allocated;
	};

	T*  Pointer( void )
	{
		return T;
	};

	void Delete( u32 index )
	{
		_AE_DEBUG_BREAK_(index>=used);

		for (u32 i=index; i<used-1; i++)
			m_pData[i] = m_pData[i+1];
		used--;
	}

	void Clear( void )
	{
		allocated = 0;
		used = 0;
		if (m_pData) 
		{
			delete [] m_pData ;
			m_pData = 0;
		}
	};

	void SetGrow( u32 size )
	{
		grow = size;
	};

};

}	// namespace Core
}	// namespace Ahmad

#endif	// _ARRAY_H_