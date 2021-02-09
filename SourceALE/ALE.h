// ALE.h : main header file for the ALE application
//  3 Feb 1999

#if !defined(AFX_ALE_H__1A7D690E_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
#define AFX_ALE_H__1A7D690E_335C_11D2_8F37_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CALEApp:
// See ALE.cpp for the implementation of this class
//

class CALEApp : public CWinApp
{
public:
	CALEApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CALEApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALE_H__1A7D690E_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
