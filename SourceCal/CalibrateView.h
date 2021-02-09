// CalibrateView.h : interface of the CCalibrateView class
//
/////////////////////////////////////////////////////////////////////////////

#include "CalDlg.h"

#if !defined(AFX_CALIBRATEVIEW_H__DF63468D_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
#define AFX_CALIBRATEVIEW_H__DF63468D_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalibrateView : public CView
{
protected: // create from serialization only
	CCalibrateView();
	DECLARE_DYNCREATE(CCalibrateView)

// Attributes
public:
	CCalibrateDoc* GetDocument();
	CCalDlg *pCalDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibrateView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCalibrateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCalibrateView)
	afx_msg void OnCalibrateGo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CalibrateView.cpp
inline CCalibrateDoc* CCalibrateView::GetDocument()
   { return (CCalibrateDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATEVIEW_H__DF63468D_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
