// SelectSelfAddr.cpp : implementation file
//  3 Nov 99

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "SelectRxChannel.h"
#include "SelectSelfAddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectSelfAddr dialog


CSelectSelfAddr::CSelectSelfAddr(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectSelfAddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectSelfAddr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSelectSelfAddr, CDialog)
	//{{AFX_MSG_MAP(CSelectSelfAddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectSelfAddr message handlers

BOOL CSelectSelfAddr::OnInitDialog() 
{
  int count, numOthers, i;
  struct CALEDoc::SELFADDR sa;

	CDialog::OnInitDialog();

  numOthers = pData->parrSelfAddrs->GetSize();
  count = 0;

  for (i = 0; i < numOthers; i++) {
    sa = pData->parrSelfAddrs->GetAt(i);
    ctlSelfList().InsertString(count++, sa.strSelfAddr);
  }

  // If there aren't any Self Addresses, display error and then quit Dialog Box
  if (!count) {
    MessageBox("There aren't any Self Addresses configured.\nFirst enter station info into the Self Addresses section.",
      "Alert", MB_OK);
    EndDialog(IDCANCEL);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelectSelfAddr::OnOK() 
{
  // If no selection in From Address, then error
  if ((iSelfAddr = ctlSelfList().GetCurSel()) == LB_ERR) {
    MessageBox("Select one of the Self Addresses (or press Cancel).");
    ctlSelfList().SetFocus();
    return;
  }

  // Bring up a dialog box, so the user can select the desired channel
	CSelectRxChannel dlg(NULL);

  // Select the channel to receive
  dlg.pData = pData;
  dlg.iSelfAddrMode = 1;
  dlg.iSelfAddr = iSelfAddr;
  if (dlg.DoModal() == IDOK) {
    iChannel = dlg.iChanNum;
  } else {
    CDialog::OnCancel();
    return;
  }

	CDialog::OnOK();
}
