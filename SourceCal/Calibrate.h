// Calibrate.h : main header file for the CALIBRATE application
//

#if !defined(AFX_CALIBRATE_H__DF634685_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
#define AFX_CALIBRATE_H__DF634685_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCalibrateApp:
// See Calibrate.cpp for the implementation of this class
//

class CCalibrateApp : public CWinApp
{
public:
	CCalibrateApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibrateApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCalibrateApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATE_H__DF634685_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
