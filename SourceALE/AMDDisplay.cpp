// AMDDisplay.cpp : implementation file
//  26 Oct 99

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "ALEView.h"
#include "AMDDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAMDDisplay dialog


CAMDDisplay::CAMDDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CAMDDisplay::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAMDDisplay)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CAMDDisplay, CDialog)
	//{{AFX_MSG_MAP(CAMDDisplay)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAMDDisplay message handlers

BOOL CAMDDisplay::OnInitDialog() 
{
  int size, i, len;
  CString str;
  CHARFORMAT cf;
  PARAFORMAT pf;
  struct CALEDoc::AMDMESSAGE amd;

	CDialog::OnInitDialog();
  cf.cbSize = sizeof CHARFORMAT;
  pf.cbSize = sizeof(PARAFORMAT);
  iMsg = 0;

  // Write a title line
  str = "AMD Messages\n";
  len = str.GetLength();
  ctlEdit().ReplaceSel(str);
  ctlEdit().SetSel(0, len);
  cf.dwMask = CFM_BOLD | CFM_SIZE;
  cf.dwEffects = CFE_BOLD;
  cf.yHeight = 240;
  ctlEdit().SetSelectionCharFormat(cf);
  // Modify the paragraph format so that the text is centered. 
  pf.dwMask = PFM_ALIGNMENT;
  pf.wAlignment = PFA_CENTER;
  ctlEdit().SetParaFormat(pf);
  ctlEdit().SetSel(-1, -1);
  iMsg += len;

  // Set the size back to 10 point
  cf.dwMask = CFM_BOLD | CFM_SIZE;
  cf.dwEffects = 0;
  cf.yHeight = 200;
  ctlEdit().SetSelectionCharFormat(cf);

	size = pData->parrAMDMessages->GetSize();
  for (i = 0; i < size; i++) {
    amd = pData->parrAMDMessages->GetAt(i);
    str = "From " + amd.strFrom;
    iMsg += 5;
    len = amd.strFrom.GetLength();
    ctlEdit().ReplaceSel(str);
    ctlEdit().SetSel(iMsg, iMsg + len);
    iMsg += len;
    cf.dwMask = CFM_BOLD;
    cf.dwEffects = CFE_BOLD;
    ctlEdit().SetSelectionCharFormat(cf);
    ctlEdit().SetSel(-1, -1);
    cf.dwEffects = 0;
    ctlEdit().SetSelectionCharFormat(cf);

    str = " to " + amd.strTo;
    iMsg += 4;
    len = amd.strTo.GetLength();
    ctlEdit().ReplaceSel(str);
    ctlEdit().SetSel(iMsg, iMsg + len);
    iMsg += len;
    cf.dwMask = CFM_BOLD;
    cf.dwEffects = CFE_BOLD;
    ctlEdit().SetSelectionCharFormat(cf);
    ctlEdit().SetSel(-1, -1);
    cf.dwEffects = 0;
    ctlEdit().SetSelectionCharFormat(cf);

    str = " on " + amd.strFreq;
    iMsg += 4;
    len = amd.strFreq.GetLength();
    ctlEdit().ReplaceSel(str);
    ctlEdit().SetSel(iMsg, iMsg + len);
    iMsg += len;
    cf.dwMask = CFM_BOLD;
    cf.dwEffects = CFE_BOLD;
    ctlEdit().SetSelectionCharFormat(cf);
    ctlEdit().SetSel(-1, -1);
    cf.dwEffects = 0;
    ctlEdit().SetSelectionCharFormat(cf);

    str = " MHz. at " + amd.strTime + "\n";
    iMsg += 9;
    len = amd.strTime.GetLength();
    ctlEdit().ReplaceSel(str);
    ctlEdit().SetSel(iMsg, iMsg + len);
    iMsg += len + 1;
    cf.dwMask = CFM_BOLD;
    cf.dwEffects = CFE_BOLD;
    ctlEdit().SetSelectionCharFormat(cf);
    ctlEdit().SetSel(-1, -1);
    cf.dwEffects = 0;
    ctlEdit().SetSelectionCharFormat(cf);

    // Set the size back to 10 point
    cf.dwMask = CFM_SIZE;
    cf.dwEffects = 0;
    cf.yHeight = 200;
    ctlEdit().SetSelectionCharFormat(cf);

    pf.dwMask = PFM_STARTINDENT;
    pf.dxStartIndent = 200;
    ctlEdit().SetParaFormat(pf);
    str = amd.strMessage + "\n\n";
    ctlEdit().ReplaceSel(str);
    iMsg += str.GetLength();

    pf.dwMask = PFM_STARTINDENT;
    pf.dxStartIndent = 0;
    ctlEdit().SetParaFormat(pf);
  }

	return TRUE;  // return TRUE
}

void CAMDDisplay::OnCancel()
{
  DestroyWindow();
}

void CAMDDisplay::PostNcDestroy()
{
  view->pAMDDlg = NULL;
  delete this;
}

void CAMDDisplay::AMDModified(LONG index)
// A new AMD message has been received.  Add it to the display
{
  struct CALEDoc::AMDMESSAGE amd;
  int len;
  CString str;
  CHARFORMAT cf;
  PARAFORMAT pf;

  SetFocus();

  amd = pData->parrAMDMessages->GetAt(index);
  cf.cbSize = sizeof CHARFORMAT;

  str = "From " + amd.strFrom;
  iMsg += 5;
  len = amd.strFrom.GetLength();
  ctlEdit().ReplaceSel(str);
  ctlEdit().SetSel(iMsg, iMsg + len);
  iMsg += len;
  cf.dwMask = CFM_BOLD;
  cf.dwEffects = CFE_BOLD;
  ctlEdit().SetSelectionCharFormat(cf);
  ctlEdit().SetSel(-1, -1);
  cf.dwEffects = 0;
  ctlEdit().SetSelectionCharFormat(cf);

  str = " to " + amd.strTo;
  iMsg += 4;
  len = amd.strTo.GetLength();
  ctlEdit().ReplaceSel(str);
  ctlEdit().SetSel(iMsg, iMsg + len);
  iMsg += len;
  cf.dwMask = CFM_BOLD;
  cf.dwEffects = CFE_BOLD;
  ctlEdit().SetSelectionCharFormat(cf);
  ctlEdit().SetSel(-1, -1);
  cf.dwEffects = 0;
  ctlEdit().SetSelectionCharFormat(cf);

  str = " on " + amd.strFreq;
  iMsg += 4;
  len = amd.strFreq.GetLength();
  ctlEdit().ReplaceSel(str);
  ctlEdit().SetSel(iMsg, iMsg + len);
  iMsg += len;
  cf.dwMask = CFM_BOLD;
  cf.dwEffects = CFE_BOLD;
  ctlEdit().SetSelectionCharFormat(cf);
  ctlEdit().SetSel(-1, -1);
  cf.dwEffects = 0;
  ctlEdit().SetSelectionCharFormat(cf);

  str = " MHz. at " + amd.strTime + "\n";
  iMsg += 9;
  len = amd.strTime.GetLength();
  ctlEdit().ReplaceSel(str);
  ctlEdit().SetSel(iMsg, iMsg + len);
  iMsg += len + 1;
  cf.dwMask = CFM_BOLD;
  cf.dwEffects = CFE_BOLD;
  ctlEdit().SetSelectionCharFormat(cf);
  ctlEdit().SetSel(-1, -1);
  cf.dwEffects = 0;
  ctlEdit().SetSelectionCharFormat(cf);

  // Set the size back to 10 point
  cf.dwMask = CFM_SIZE;
  cf.dwEffects = 0;
  cf.yHeight = 200;
  ctlEdit().SetSelectionCharFormat(cf);

  pf.dwMask = PFM_STARTINDENT;
  pf.dxStartIndent = 200;
  ctlEdit().SetParaFormat(pf);
  str = amd.strMessage + "\n\n";
  ctlEdit().ReplaceSel(str);
  iMsg += str.GetLength();

  pf.dwMask = PFM_STARTINDENT;
  pf.dxStartIndent = 0;
  ctlEdit().SetParaFormat(pf);
}
