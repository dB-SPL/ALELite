// AMDText.cpp : implementation file
//  13 Oct 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "AMDText.h"
#include "ASCII_Set.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAMDText dialog


CAMDText::CAMDText(CWnd* pParent /*=NULL*/)
	: CDialog(CAMDText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAMDText)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CAMDText, CDialog)
	//{{AFX_MSG_MAP(CAMDText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAMDText message handlers

BOOL CAMDText::OnInitDialog() 
{
	CDialog::OnInitDialog();

	ctlText().SetLimitText(90);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAMDText::OnOK() 
{
  int i, len;
  CString strErr;

  ctlText().GetWindowText(strAMDText);
	// Must not be null
  len = strAMDText.GetLength();
  if (len == 0) {
    MessageBox("Enter the text for the message (or press Cancel).");
    ctlText().SetFocus();
    return;
  }

  // All characters must be in the ASCII-64 character set
  for (i = 0; i < len; i++)
    if (ASCII_Set[strAMDText[i]] < ASCII_64) {
      strErr.Format("The character %c is not allowed in AMD messages.", strAMDText[i]);
      MessageBox(strErr);
      ctlText().SetFocus();
      ctlText().SetSel(i, i + 1);
      return;
    }

	CDialog::OnOK();
}
