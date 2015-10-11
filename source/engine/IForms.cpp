/*
	Ahmad ENGINE

	Copyright (C) 2005-2006, Suhendra Ahmad
	
	email 	: ceo @ ainasoft.com
	website : http://www.ainasoft.com

	Ainasoft Technology
*/

#include "IForms.h"

namespace Ahmad
{
namespace Platform
{

#if defined(WIN32) || defined(XBOX)

IForms *IForms::gForms = NULL;

void IForms::createForms( void )
{
	if (!gForms)
	{
		gForms = new IForms();
	}
}

void IForms::releaseForms( void )
{
	SAFE_DELETE(gForms);
}

HWND IForms::createForm( HINSTANCE hInstance,
						   const c8 *cClassName,
						   const c8 *cTitle,
						   u32 iWidth,
						   u32 iHeight,
						   bool bFullscreen,
						   WNDPROC WndProc )
{
	WNDCLASS	wc;

	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hIcon			= LoadIcon(0, IDI_WINLOGO);
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= cClassName;
	wc.lpszMenuName		= NULL;
	wc.style			= CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClass(&wc)) return NULL;

	RECT r;
	r.left		= 0;
	r.top		= 0;
	r.right		= iWidth;
	r.bottom	= iHeight;

	DWORD	style = WS_POPUP;
	if (!bFullscreen) style = WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX | WS_SYSMENU | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	AdjustWindowRect(&r, style, FALSE);

	u32 iWinWidth = r.right - r.left;
	u32 iWinHeight = r.bottom - r.top;

	u32 iPosLeft = (GetSystemMetrics(SM_CXSCREEN) - iWinWidth) / 2;
	u32 iPosTop = (GetSystemMetrics(SM_CYSCREEN) - iWinHeight) / 2;

	if (bFullscreen) iPosLeft = iPosTop = 0;
	
	return ::CreateWindow( cClassName, cTitle, style, iPosLeft, iPosTop, iWinWidth, iWinHeight, NULL, NULL, hInstance, NULL );
}

void IForms::setTitle( HWND hWnd, const c8 *cTitle )
{
	SetWindowText(hWnd, (LPCTSTR)cTitle);
}

void IForms::moveForm( HWND hWnd, u32 nLeft, u32 nTop, u32 nWidth, u32 nHeight )
{
	MoveWindow(hWnd, nLeft, nTop, nWidth, nHeight, TRUE);
}

i32 IForms::AlertOK( const c8 *cMessage, const c8 *cTitle )
{
	return MessageBox(0, (LPCTSTR)cMessage, (LPCTSTR)cTitle, MB_OK | MB_ICONEXCLAMATION );
}

i32 IForms::AlertYesNo( const c8 *cMessage, const c8 *cTitle )
{
	return MessageBox(0, (LPCTSTR)cMessage, (LPCTSTR)cTitle, MB_YESNO | MB_ICONEXCLAMATION );
}

i32 IForms::AlertYesNoCancel( const c8 *cMessage, const c8 *cTitle )
{
	return MessageBox(0, (LPCTSTR)cMessage, (LPCTSTR)cTitle, MB_YESNOCANCEL | MB_ICONEXCLAMATION );
}

i32 IForms::AlertRetryCancel( const c8 *cMessage, const c8 *cTitle )
{
	return MessageBox(0, (LPCTSTR)cMessage, (LPCTSTR)cTitle, MB_RETRYCANCEL | MB_ICONEXCLAMATION );
}

AI_RESULT IForms::CreateForm( HINSTANCE hInstance,
								const c8 *cClassName,
								const c8 *cTitle,
								u32 iWidth,
								u32 iHeight,
								bool bFullscreen,
								WNDPROC WndProc )
{
	m_hWnd = createForm( hInstance, cClassName, cTitle, iWidth, iHeight, bFullscreen, WndProc );

	if (!m_hWnd) return AI_FAIL;

	m_hInstance		= hInstance;
	m_cTitle		= (c8 *)cTitle;
	m_nWidth		= iWidth;
	m_nHeight		= iHeight;
	m_bFullscreen	= bFullscreen;

	return AI_OK;
}

void IForms::SetTitle( const c8 *cTitle )
{
	if (cTitle)
	{
		m_cTitle = (c8 *)cTitle;
	}
	else
		m_cTitle = "";
}

void IForms::MoveForm( u32 nNewLeft, u32 nNewTop )
{
	if (!m_bFullscreen)
	{
		moveForm(m_hWnd, nNewLeft, nNewTop, m_nWidth, m_nHeight);
		m_nLeft = nNewLeft;
		m_nTop = nNewTop;
	}
}

void IForms::ResizeForm( u32 nNewWidth, u32 nNewHeight )
{
	moveForm(m_hWnd, m_nLeft, m_nTop, nNewWidth, nNewHeight);
	m_nWidth = nNewWidth;
	m_nHeight = nNewHeight;
}

#endif	// WIN32 || XBOX

}	// namespace Forms
}	// namespace Ahmad