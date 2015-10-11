/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IBUYUT_H_
#define _IBUYUT_H_

#include "Types.h"

/********************************************************
   This is the ancestor of the classes on 
   the AhmadENGINE.
   
   Buyut in Sundanese mean the great ancestor,
   and Cicit in Sundanese mean the descendant.
   Sorry, for the language, but i'm not a sundanese,
   i'm a batavian (betawi). I use sundanese
   to respect the city where i live, Bandung.
  
   Cut the crap, i'll show you the usage:
   ========================================
   
   If you want to create an object and delete
   it, you use it like this:
  
   class B;	// class B: public(IBuyut)
   B *pB = createB;
  
   pB->Throw();
  
   Suhendra Ahmad on Mon, December 5th 2005 1:32AM
  *******************************************************/

namespace Ahmad
{

class IBuyut
{
public:
	
	IBuyut(): m_nCicit(1) {}

	virtual ~IBuyut() {}

	void Catch()
	{
		++m_nCicit;
	}

	bool Throw()
	{
		if (m_nCicit<=0) __asm int 3;
		--m_nCicit;

		if (!m_nCicit)
		{
			delete this;
			return true;
		}

		return false;
	}

private:
	i32		m_nCicit;
};

#ifndef SAFE_THROW
#define SAFE_THROW(p) { if (p) { (p)->Throw(); p = NULL; }; }
#endif

}	// namespace Ahmad

#endif	// _IBUYUT_H_