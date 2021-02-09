// ALEView.h : interface of the CALEView class
//  25 Oct 1999
/////////////////////////////////////////////////////////////////////////////

#include "SignalQualityDlg.h"
#include "AMDDisplay.h"

#if !defined(AFX_ALEVIEW_H__1A7D6916_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
#define AFX_ALEVIEW_H__1A7D6916_335C_11D2_8F37_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CALEView : public CView
{
protected: // create from serialization only
	CALEView();
	DECLARE_DYNCREATE(CALEView)

// Attributes
public:
	CALEDoc* GetDocument();
  CStatusBar m_wndStatusBar;
  CSignalQualityDlg* pSQDlg;
  CAMDDisplay* pAMDDlg;

private:

// Operations
private:
  BOOL SoundingOn();
  void SoundingOff();

public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALEView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CALEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CALEView)
	afx_msg void OnEditChannel();
	afx_msg void OnEditSelfaddr();
	afx_msg void OnEditOtheraddr();
	afx_msg void OnSoundingStart();
	afx_msg void OnUpdateSoundingStart(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnViewSignalquality();
	afx_msg void OnUpdateViewSignalquality(CCmdUI* pCmdUI);
	afx_msg void OnRxScanning();
	afx_msg void OnUpdateRxScanning(CCmdUI* pCmdUI);
	afx_msg void OnRxOff();
	afx_msg void OnUpdateRxOff(CCmdUI* pCmdUI);
	afx_msg void OnRxSingchan();
	afx_msg void OnUpdateRxSingchan(CCmdUI* pCmdUI);
	afx_msg void OnEditConfig();
	afx_msg void OnIndividualCall();
	afx_msg void OnUpdateIndividualcall(CCmdUI* pCmdUI);
	afx_msg void OnAllcall();
	afx_msg void OnViewAmd();
	afx_msg void OnUpdateViewAmd(CCmdUI* pCmdUI);
	afx_msg void OnSendAMDMessage();
	afx_msg void OnDebugMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ALEView.cpp
inline CALEDoc* CALEView::GetDocument()
   { return (CALEDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALEVIEW_H__1A7D6916_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
