// OtherAddrDlg.cpp : implementation file
//  30 Mar 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "OtherAddrDlg.h"
#include "SlotList.h"
#include "Dlgfloat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherAddrDlg dialog


COtherAddrDlg::COtherAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COtherAddrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherAddrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(COtherAddrDlg, CDialog)
	//{{AFX_MSG_MAP(COtherAddrDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnEditSlots)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherAddrDlg message handlers

BOOL COtherAddrDlg::OnInitDialog() 
{
  CString str;
  struct CALEDoc::OTHERADDR oa;
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
    ctlReplyWait().SetWindowText("0");
  } else {
    oa = pData->parrOtherAddrs->GetAt(pos);
    ctlOtherAddr().SetWindowText(oa.strOtherAddr);
    str.Format("%f", oa.dReplyWait);
    ctlReplyWait().SetWindowText(str);

    for (i = 0; i < size; i++) {
      if (oa.parrValidChannels->GetAt(i))
        ctlChannelList().SetSel(i);
    }

    size = oa.pSlotStrings->GetSize();
    for (i = 0; i < size; i++) {
      ctlSlotList().AddString(oa.pSlotStrings->GetAt(i));
      LocalSlotStrings.Add(oa.pSlotStrings->GetAt(i));
    }
  }

  ctlOtherAddr().SetLimitText(15);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void COtherAddrDlg::OnEditSlots() 
{
  // Run the dialog box for changing the slot addresses.  Assume the dialog box
  // checks that all data is correct and complete.

  CSlotList dlg(this);
  dlg.pSlotStrings = &LocalSlotStrings;
  if (dlg.DoModal() == IDOK) {
    // Redisplay the list contol with the new Slot Addresses.
    ctlSlotList().ResetContent();
    int size = LocalSlotStrings.GetSize();
    for (int i = 0; i < size; i++) {
      ctlSlotList().AddString(LocalSlotStrings.GetAt(i));
    }

    pData->pDoc->SetModifiedFlag();
  }
}

void COtherAddrDlg::OnOK() 
{
	CString str;
  double ReplyWait;
  struct CALEDoc::OTHERADDR oa, oa2;
  int i, len, cntoa;
  BOOLEAN bChanOK, bChanSelected;
	
  // Check the Other Address.  It must contain only upper case letters or 
  // numbers, a maximum of 15 characters, and the address must be unique
  // within the list.  (The control enforces caps and a length
  // limit of 15.)  Null entries are not OK.
  cntoa = pData->parrOtherAddrs->GetSize();
  ctlOtherAddr().GetWindowText(str);
  len = str.GetLength();
  if (len == 0) {
    MessageBox("The other address field cannot be empty.");
    ctlOtherAddr().SetFocus();
    ctlOtherAddr().SetSel(0, -1);
    return;
  }

  for (i = 0; i < len; i++) {
    if (!isalnum(str[i])) {
      MessageBox("Addresses can contain only (upper case) letters and numbers.");
      ctlOtherAddr().SetFocus();
      ctlOtherAddr().SetSel(0, -1);
      return;
    }
  }

  // Check for a dup other address, but if we are editing, accept a string that
  // is unchanged.
  for (i = 0; i < cntoa; i++) {
    if (str == pData->parrOtherAddrs->GetAt(i).strOtherAddr && (bNew || i != pos)) {
      MessageBox("Other Addresses must be unique.");
      ctlOtherAddr().SetFocus();
      ctlOtherAddr().SetSel(0, -1);
      return;
    }
  }
  oa.strOtherAddr = str;

  // Check the Reply Wait.  Reject bad characters, negative numbers.
  // Zero is OK, but null entries will be rejected.
  ctlReplyWait().GetWindowText(str);
  if (!_AfxSimpleFloatParse(str, ReplyWait) || (ReplyWait < 0)) {
    MessageBox("The Reply Wait must be a positive number or zero.");
    ctlReplyWait().SetFocus();
    ctlReplyWait().SetSel(0, -1);
    return;
  }
  oa.dReplyWait = ReplyWait;

  // Set the array of channels selected.
  bChanOK = FALSE;
  oa.parrValidChannels = new (CArray< BOOLEAN, BOOLEAN >);
  len = pData->parrChannels->GetSize();
  for (i = 0; i < len; i++) {
    if (bChanSelected = ctlChannelList().GetSel(i))
      bChanOK = TRUE;
    oa.parrValidChannels->Add(bChanSelected);
  }

  // There must be at least one channel selected
  if (!bChanOK) {
    MessageBox("At least one channel must be selected.");
    ctlChannelList().SetFocus();
    delete oa.parrValidChannels;
    return;
  }

  // Set the string array for the slot addresses
  oa.pSlotStrings = new (CStringArray);
  len = LocalSlotStrings.GetSize();
  for (i = 0; i < len; i++) 
    oa.pSlotStrings->Add(LocalSlotStrings.GetAt(i));

  // Everything should be OK.  Save the data
  if (bNew) {
    if (pos == -1)
      pos = pData->parrOtherAddrs->Add(oa);
    else
      pData->parrOtherAddrs->InsertAt(pos, oa);
  } else {
    oa2 = pData->parrOtherAddrs->GetAt(pos);
    delete oa2.parrValidChannels;
    delete oa2.pSlotStrings;
    pData->parrOtherAddrs->SetAt(pos, oa);
  }

	CDialog::OnOK();
}

