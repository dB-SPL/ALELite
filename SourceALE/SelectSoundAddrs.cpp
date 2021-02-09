// SelectSoundAddrs.cpp : implementation file
//  5 Apr 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "SelectSoundAddrs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectSoundAddrs dialog


CSelectSoundAddrs::CSelectSoundAddrs(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSoundAddrs::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSoundAddrs)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSelectSoundAddrs, CDialog)
	//{{AFX_MSG_MAP(CSelectSoundAddrs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectSoundAddrs message handlers

BOOL CSelectSoundAddrs::OnInitDialog() 
{
  int count = 0;
  struct CALEDoc::SELFADDR sa;

	CDialog::OnInitDialog();
	
	int numSelfs = pData->parrSelfAddrs->GetSize();

  // Set the list box contents to the possible Self Addresses and
  // select the last ones used.
  for (int i = 0; i < numSelfs; i++) {
    sa = pData->parrSelfAddrs->GetAt(i);
    ctlList().InsertString(count, sa.strSelfAddr);
    ctlList().SetSel(count++, sa.bSoundingOn);
  }

  // If there aren't any Self Addresses, display error and then quit Dialog Box
  if (!count) {
    MessageBox("There aren't any Self Addresses configured.\nFirst enter station info into the Self Addresses section.",
      "Alert", MB_OK);
    EndDialog(IDCANCEL);
  }

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelectSoundAddrs::OnOK()
{
  int i, j, iCurSel, numSelfs, numChans;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::CHANNEL ch;
  CString strSelfAddr, strErr;
  BOOL bAddrOK;

  // If no selection then error
  if ((iCurSel = ctlList().GetSelCount()) == 0) {
    MessageBox("Select one or more of the Addresses (or press Cancel).");
    ctlList().SetFocus();
    return;
  }

  // Check that at least 1 channel has a sounding interval for each address
	numSelfs = pData->parrSelfAddrs->GetSize();
  numChans = pData->parrChannels->GetSize();
  for (i = 0; i < numSelfs; i++) {
    if (ctlList().GetSel(i)) {
      sa = pData->parrSelfAddrs->GetAt(i);
      bAddrOK = FALSE;
      for (j = 0; j < numChans; j++) {
        if (sa.parrValidChannels->GetAt(j)) {
          ch = pData->parrChannels->GetAt(j);
          if (ch.dSoundInterval > 0.0) {
            bAddrOK = TRUE;
            break;
          }
        }
      }
      if (!bAddrOK) {
        strErr.Format("Address %s cannot be selected.\nAt least one of the channels used by this address must have a valid Sound Interval.",
          sa.strSelfAddr);
        MessageBox(strErr);
        ctlList().SetFocus();
        return;
      }
    }
  }

  // All is OK.  Set sounding on for each selected address
  for (i = 0; i < numSelfs; i++) {
    sa = pData->parrSelfAddrs->GetAt(i);
    sa.bSoundingOn = ctlList().GetSel(i);
    pData->parrSelfAddrs->SetAt(i, sa);
  }

	CDialog::OnOK();
}
