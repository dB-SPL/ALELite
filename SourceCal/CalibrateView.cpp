// CalibrateView.cpp : implementation of the CCalibrateView class
//

#include "stdafx.h"

#include <mmsystem.h>
#include <dsound.h>  
#include "hdaudio.h"

#include "Calibrate.h"
#include "CalibrateDoc.h"
#include "CalDlg.h"
#include "CalibrateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibrateView

IMPLEMENT_DYNCREATE(CCalibrateView, CView)

BEGIN_MESSAGE_MAP(CCalibrateView, CView)
	//{{AFX_MSG_MAP(CCalibrateView)
	ON_COMMAND(ID_CALIBRATE_GO, OnCalibrateGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibrateView construction/destruction

CCalibrateView::CCalibrateView()
{
	pCalDlg = NULL;
}

CCalibrateView::~CCalibrateView()
{
}

BOOL CCalibrateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCalibrateView drawing

void CCalibrateView::OnDraw(CDC* pDC)
{
	CCalibrateDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CCalibrateView diagnostics

#ifdef _DEBUG
void CCalibrateView::AssertValid() const
{
	CView::AssertValid();
}

void CCalibrateView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCalibrateDoc* CCalibrateView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCalibrateDoc)));
	return (CCalibrateDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCalibrateView message handlers

void CCalibrateView::OnCalibrateGo()
{
	CCalibrateDoc* pDoc = GetDocument();

  if (pCalDlg != NULL) {
    pCalDlg->DestroyWindow();
  } else {
	  pCalDlg = new CCalDlg;
    pCalDlg->view = this;
	  pCalDlg->Create(IDD_DIALOG1, this);
  }

	return;
}

void CCalibrateView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  if ((pCalDlg != NULL) && (lHint == 1L))
    pCalDlg->UpdateValue();
  else
    CView::OnUpdate(pSender, lHint, pHint);
}
