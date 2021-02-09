// CalibrateDoc.cpp : implementation of the CCalibrateDoc class
//

#include "stdafx.h"

#include "Calibrate.h"
#include "CalibrateDoc.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibrateDoc

IMPLEMENT_DYNCREATE(CCalibrateDoc, CDocument)

BEGIN_MESSAGE_MAP(CCalibrateDoc, CDocument)
	//{{AFX_MSG_MAP(CCalibrateDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibrateDoc construction/destruction

CCalibrateDoc::CCalibrateDoc()
{
	// TODO: add one-time construction code here

}

CCalibrateDoc::~CCalibrateDoc()
{
}

BOOL CCalibrateDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCalibrateDoc serialization

void CCalibrateDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCalibrateDoc diagnostics

#ifdef _DEBUG
void CCalibrateDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCalibrateDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCalibrateDoc commands
void CCalibrateDoc::DataAvail() {
  UpdateAllViews(NULL, 1L, NULL); 
}

