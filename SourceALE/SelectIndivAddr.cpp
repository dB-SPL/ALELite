// SelectIndivAddr.cpp : implementation file
//  18 Nov 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "SelectRxChannel.h"
#include "SelectIndivAddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectIndivAddr dialog


CSelectIndivAddr::CSelectIndivAddr(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectIndivAddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectIndivAddr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSelectIndivAddr, CDialog)
	//{{AFX_MSG_MAP(CSelectIndivAddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectIndivAddr message handlers

BOOL CSelectIndivAddr::OnInitDialog() 
{
  int count, numOthers, i;
  struct CALEDoc::OTHERADDR oa;
  struct CALEDoc::SELFADDR sa;

	CDialog::OnInitDialog();

  // The default channel mode should be automatic
  CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

  numOthers = pData->parrOtherAddrs->GetSize();
  count = 0;

  for (i = 0; i < numOthers; i++) {
    oa = pData->parrOtherAddrs->GetAt(i);
    if (oa.pSlotStrings->GetSize() == 0) {
      ctlToList().InsertString(count++, oa.strOtherAddr);
    }
  }

  // If there aren't any Other Addresses, display error and then quit Dialog Box
  if (!count) {
    MessageBox("There aren't any Other Addresses configured.\nFirst enter station info into the Other Addresses section.",
      "Alert", MB_OK);
    EndDialog(IDCANCEL);
    return TRUE;
  }

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

void CSelectIndivAddr::OnOK()
{
  int iCurSel, numOthers, numChans, i;
  CString strOtherAddr;
  struct CALEDoc::OTHERADDR oa;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::CHANNEL ch;

  // If no selection in To Address, then error
  if ((iCurSel = ctlToList().GetCurSel()) == LB_ERR) {
    MessageBox("Select one of the To Addresses (or press Cancel).");
    ctlToList().SetFocus();
    return;
  }

  ctlToList().GetText(iCurSel, strOtherAddr);
	numOthers = pData->parrOtherAddrs->GetSize();

  for (i = 0; i < numOthers; i++) {
    oa = pData->parrOtherAddrs->GetAt(i);
    if (oa.pSlotStrings->GetSize() == 0) {
      if (oa.strOtherAddr == strOtherAddr) {
        iOtherAddr = i;
        break;
      }
    }
  }

  // If no selection in From Address, then error
  if ((iSelfAddr = ctlSelfList().GetCurSel()) == LB_ERR) {
    MessageBox("Select one of the Self Addresses (or press Cancel).");
    ctlSelfList().SetFocus();
    return;
  }

  // Do the two addresses have at least one channel in common?
  sa = pData->parrSelfAddrs->GetAt(iSelfAddr);
  numChans = pData->parrChannels->GetSize();
  for (i = 0; i < numChans; i++) {
    ch = pData->parrChannels->GetAt(i);
    if (sa.parrValidChannels->GetAt(i) && oa.parrValidChannels->GetAt(i) &&
        (ch.eTRMode == CALEDoc::TR))
      goto ChannelOK;
  }
  // If here, then we couldn't find a common channel
  MessageBox("There is no common channel used by both addresses.  Select another address (or press Cancel).");
  ctlToList().SetFocus();
  return;

ChannelOK:
  if (IsDlgButtonChecked(IDC_RADIO1))
    eChannelMode = AUTO;
  else {
    eChannelMode = MANUAL;

    // Bring up a dialog box, so the user can select the desired channel
	  CSelectRxChannel dlg(NULL);

    // Select the channel to receive
    dlg.pData = pData;
    dlg.iSelfAddrMode = 2;
    dlg.iSelfAddr = iSelfAddr;
    dlg.iOtherAddr = iOtherAddr;
    if (dlg.DoModal() == IDOK) {
      iChannel = dlg.iChanNum;
    } else {
      CDialog::OnCancel();
      return;
    }
  }

	CDialog::OnOK();
}
