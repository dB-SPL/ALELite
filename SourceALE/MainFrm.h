// MainFrm.h : interface of the CMainFrame class
//  28 May 1999
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1A7D6912_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
#define AFX_MAINFRM_H__1A7D6912_335C_11D2_8F37_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHelpFinder();
	//}}AFX_MSG
  afx_msg LONG OnDataAvail(UINT, LONG);
  afx_msg LONG OnDocAvail(UINT, LONG);
  afx_msg LONG OnModemActionMessage(UINT, LONG);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1A7D6912_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
