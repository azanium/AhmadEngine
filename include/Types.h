/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _TYPES_H_
#define _TYPES_H_


//! Did we use the Windows as the platform?
#ifdef WIN32
	
	//! we need this to use the type from the windows
	#ifndef _WINDOWS_
	#include <windows.h>
	#endif

	//! dirty, but we need this... hon to? hai (Suhendra Ahmad ceo (at) ainasoft.com) *_*
	#ifndef _INC_STDIO
	#include <stdio.h>
	#endif

#endif	// WIN32

#define AI_PI		3.14159265f
#define AI_PI2		1.5707963f
#define AI_2PI		6.2831853f
#define AI_EPSILON	0.00001f

#define DEG_TO_RAD(x)	(x * AI_PI / 180.0f)
#define RAD_TO_DEG(x)	(x * 180.0f / AI_PI)

#ifndef NULL
#define NULL 0
#endif

// Platform Independent Variable Types
typedef	char			c8;

typedef signed char		s8;
typedef unsigned char	u8;

typedef signed short	s16;
typedef unsigned short	u16;

typedef int				i32;
typedef	int				s32;
typedef unsigned int	u32;

typedef	long			il32;
typedef unsigned long	ul32;

#ifdef _MSC_VER
typedef _int64			i64;
typedef signed _int64	s64;
typedef unsigned _int64	u64;
#endif

typedef float			f32;
typedef double			f64;

static const s8			s8_min = s8(-128);
static const s8			s8_max = s8(127);

static const u8			u8_min = u8(0);
static const u8			u8_max = u8(255);

static const s16		s16_min = s16(-32768);
static const s16		s16_max = s16(32767);

static const u16		u16_min = u16(0);
static const u16		u16_max = s16(65535);

static const s32		s32_min = s32(-2147483647 - 1);
static const s32		s32_max = s32(2147483647);

static const u32		u32_min = u32(0);
static const u32		u32_max = u32(0xffffffff);

#define BIT(x)			(1 << (x))

#define AI_RESULT		il32	// it is equal with HRESULT

#define AI_OK			((AI_RESULT)0x0000000L)
#define AI_FAIL			((AI_RESULT)0x0000001L)
#define AI_INVALID		((AI_RESULT)0x0000002L)
#define AI_OUTOFMEMORY	((AI_RESULT)0x0000003L)
#define AI_BUFFERFULL	((AI_RESULT)0x0000004L)
#define AI_FILENOTFOUND	((AI_RESULT)0x0000005L)

#if defined(WIN32) || defined(XBOX)
	#ifndef AI_IMPORTS
		#define AI_API	__declspec( dllexport )
	#else
		#define AI_API	__declspec( dllimport )
	#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) {if (p) { delete p; p = NULL; }; }
#endif

#ifndef SAFE_DELETE_A
#define SAFE_DELETE_A(p) {if (p) { delete [] p; p = NULL; }; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if (p) { (p)->Release(); p = NULL; }; }
#endif

#ifndef WIN32	// for linux?
	#ifndef HWND
		#define HWND		void *
	#endif

	#ifndef HINSTANCE
		#define HINSTANCE	void *
	#endif
#endif

#if defined(WIN32) || defined(XBOX)
#define _AE_DEBUG_BREAK_( condition )	if (condition) { _asm int 3 }
#else
#define _AE_DEBUG_BREAK_( condition )
#endif

#ifndef MAX_ID
#define MAX_ID	65535
#endif

#endif	// _TYPES_H_