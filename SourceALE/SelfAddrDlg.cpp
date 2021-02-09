// SelfAddrDlg.cpp : implementation file
//  26 Oct 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "SelfAddrDlg.h"
#include "Dlgfloat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfAddrDlg dialog


CSelfAddrDlg::CSelfAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelfAddrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelfAddrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSelfAddrDlg, CDialog)
	//{{AFX_MSG_MAP(CSelfAddrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfAddrDlg message handlers

BOOL CSelfAddrDlg::OnInitDialog() 
{
  CString str;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::CHANNEL ch;
  int i;

	CDialog::OnInitDialog();

  // Add the list of channels to the list box
  int size = pData->parrChannels->GetSize();
  for (i = 0; i < size; i++) {
    ch = pData->parrChannels->GetAt(i);
    str.Format("%f", ch.dRecFreq);
    ctlChannelList().AddString(str);
  }

  if (bNew) {
    ctlSlotWait().SetWindowText("0");
  } else {
    sa = pData->parrSelfAddrs->GetAt(pos);
    ctlSelfAddr().SetWindowText(sa.strSelfAddr);
    ctlNetAddr().SetWindowText(sa.strNetAddr);
    str.Format("%d", sa.m_nSlotWait);
    ctlSlotWait().SetWindowText(str);
    for (i = 0; i < size; i++) {
      if (sa.parrValidChannels->GetAt(i))
        ctlChannelList().SetSel(i);
    }
  }

  ctlSelfAddr().SetLimitText(15);
  ctlNetAddr().SetLimitText(15);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelfAddrDlg::OnOK() 
{
	CString str;
  long SlotWait;
  struct CALEDoc::SELFADDR sa, sa2;
  int i, len1, len2, cntsa;
  BOOLEAN bChSelected, bChOK;
	
  // Check the Self Address.  It must contain only upper case letters or 
  // numbers, a maximum of 15 characters, and the first entry can be a
  // maximum of three characters.  (The control enforces caps and a length
  // limit of 15.)  Null entries are OK.
  cntsa = pData->parrSelfAddrs->GetSize();
  ctlSelfAddr().GetWindowText(str);
  len1 = str.GetLength();
  if (((cntsa == 0) || (pos == 0)) && (len1 > 3)) {
    MessageBox("The first self address cannot be longer than 3 characters");
    ctlSelfAddr().SetFocus();
    ctlSelfAddr().SetSel(0, -1);
    return;
  }
  for (i = 0; i < len1; i++) {
    if (!isalnum(str[i])) {
      MessageBox("Addresses can contain only letters and numbers");
      ctlSelfAddr().SetFocus();
      ctlSelfAddr().SetSel(0, -1);
      return;
    }
  }
  sa.strSelfAddr = str;

  // Check the Network Address.  It must contain only upper case letters or 
  // numbers and a maximum of 15 characters.  (The control enforces caps and a length
  // limit of 15.)  Null entries are OK.
  ctlNetAddr().GetWindowText(str);
  len2 = str.GetLength();
  for (i = 0; i < len2; i++) {
    if (!isalnum(str[i])) {
      MessageBox("Addresses can contain only letters and numbers");
      ctlNetAddr().SetFocus();
      ctlNetAddr().SetSel(0, -1);
      return;
    }
  }
  if ((len1 == 0) && (len2 == 0)) {
    MessageBox("The Self and Network Address cannot both be empty.");
    ctlNetAddr().SetFocus();
    ctlNetAddr().SetSel(0, -1);
    return;
  }
  sa.strNetAddr = str;

  // Check the Slot Wait.  Reject non-integers, bad characters, negative numbers.
  // Zero is OK, but null entries will be rejected.
  ctlSlotWait().GetWindowText(str);
  if (!_AfxSimpleLongParse(str, SlotWait) || (SlotWait < 0)) {
    MessageBox("The Slot Wait must be a positive integer or zero.");
    ctlSlotWait().SetFocus();
    ctlSlotWait().SetSel(0, -1);
    return;
  }
  sa.m_nSlotWait = SlotWait;

  // Set the array of channels selected.
  bChOK = FALSE;
  sa.parrValidChannels = new (CArray< BOOLEAN, BOOLEAN >);
  len1 = pData->parrChannels->GetSize();
  for (i = 0; i < len1; i++) {
    if (bChSelected = ctlChannelList().GetSel(i))
      bChOK = TRUE;
    sa.parrValidChannels->Add(bChSelected);
  }

  // There must be at least one channel selected
  if (!bChOK) {
    MessageBox("At least one channel must be selected.");
    ctlChannelList().SetFocus();
    delete sa.parrValidChannels;
    return;
  }

  // Everything should be OK.  Save the data
  if (bNew) {
    // Default for sounding is off, but don't change preexisting values
    sa.bSoundingOn = FALSE;
    if (pos == -1)
      pos = pData->parrSelfAddrs->Add(sa);
    else
      pData->parrSelfAddrs->InsertAt(pos, sa);
  } else {
    sa2 = pData->parrSelfAddrs->GetAt(pos);
    delete sa2.parrValidChannels;
    pData->parrSelfAddrs->SetAt(pos, sa);
  }

	CDialog::OnOK();
}
