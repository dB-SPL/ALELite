// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
//   19 Aug 1999

#if !defined(AFX_STDAFX_H__1A7D6910_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
#define AFX_STDAFX_H__1A7D6910_335C_11D2_8F37_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#include <htmlhelp.h>       // support for HTML help
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WM_USER_DATA_AVAIL  WM_USER
#define WM_USER_DOC_STARTED (WM_USER+1)
#define WM_USER_ACTION (WM_USER+2)

#define ID_TIMER_SOUNDING  1

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__1A7D6910_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
