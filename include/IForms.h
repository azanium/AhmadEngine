/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#ifndef _IForms_H_
#define _IForms_H_

#include "Types.h"

namespace Ahmad
{
namespace Platform
{

#if defined(WIN32) || defined(XBOX)

/****************************************************
  There is a problem when we switch the normal form
  into a fullscreen using Direct3D, it won't show
  correctly, so instead of creating your own form
  creation, use this class.

  How can i use this class?
  There are two ways to create a form using this
  class:
  
  ** using the object **
  1. Instantiate the class:
		IForms::createForms();

  2. Call the form creation method
        AI_RESULT res;
		res = IForms->CreateForm(hInstance, CLASSNAME, 
								 "AinaENGINE", WIDTH, HEIGHT, 
								 g_bFullscreen, WndProc );

  3. Grab the handle of the window
        HWND hWnd = IForms->GetHandle();

  4. After you finish, call the release method:
        IForms::releaseForms();

 ** using the static method **
  1. There's no need to instantiate your form, just call :
        HWND hWnd = IForms::CreateForm(hInstance, CLASSNAME, 
									   "AinaENGINE", WIDTH, HEIGHT, 
								       g_bFullscreen, WndProc );

  That was all. Simple isn't it?
  You can use the alert dialog box also by calling
  the AlertXXXX static method like this :
	  
	  IForms::AlertXXX( "Message", "Caption Title" );

  Created by Suhendra Ahmad (AZA) on Mon, November 28th 2005 1:49
  ceo (at) ainasoft.com *_*

 ****************************************************/

class AI_API IForms
{
public:
	IForms(): m_hInstance(0), 
				m_hWnd(NULL), 
				m_nWidth(0),
				m_nHeight(0), 
				m_bFullscreen(FALSE), 
				m_cTitle(NULL) {};
	~IForms() {};

	// Static methods
	static IForms *gForms;
	static void		createForms( void );
	static void		releaseForms( void );

	static HWND		createForm( HINSTANCE hInstance,
				  				const c8 *cClassName,
								const c8 *title,
								u32 iWidth,
								u32 iHeight,
								bool bFullscreen,
								WNDPROC WndProc );

	static void		setTitle( HWND hWnd,
							  const c8 *cTitle );
	static void		moveForm( HWND hWnd, 
							  u32 nLeft,
							  u32 nTop,
							  u32 nWidth,
							  u32 nHeight );
	
	static i32		AlertOK( const c8 *cMessage, const c8 *cTitle );
	static i32		AlertYesNo( const c8 *cMessage, const c8 *cTitle );
	static i32		AlertYesNoCancel( const c8 *cMessage, const c8 *cTitle );
	static i32		AlertRetryCancel( const c8 *cMessage, const c8 *cTitle );

	// Non-static methods
	AI_RESULT		CreateForm( HINSTANCE hInstance,
				  				const c8 *cClassName,
								const c8 *title,
								u32 iWidth,
								u32 iHeight,
								bool bFullscreen,
								WNDPROC WndProc );
	void			SetTitle( const c8 *cTitle );
	void			MoveForm( u32 nNewLeft, u32 nNewTop );
	void			ResizeForm( u32 nNewWidth, u32 nNewHeight );

	HWND			GetHandle( void ) { return m_hWnd; }
	HINSTANCE		GetModule( void ) { return m_hInstance; }
	u32				GetLeft( void ) { return m_nLeft; }
	u32				GetTop( void ) { return m_nTop; }
	u32				GetWidth( void ) { return m_nWidth; }
	u32				GetHeight( void ) { return m_nHeight; }
	bool			IsFullscreen( void ) { return m_bFullscreen; }

protected:
	HINSTANCE		m_hInstance;
	HWND			m_hWnd;
	u32				m_nLeft;
	u32				m_nTop;
	u32				m_nWidth;
	u32				m_nHeight;
	bool			m_bFullscreen;
	c8			   *m_cTitle;
};

#endif	// WIN32 || XBOX

}	// namespace Forms
}	// namespace Ahmad

#endif