// SlotAddr.cpp : implementation file
//  14 Jan 1999

#include "stdafx.h"
#include "ALE.h"
#include "SlotAddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlotAddr dialog


CSlotAddr::CSlotAddr(CWnd* pParent /*=NULL*/)
	: CDialog(CSlotAddr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlotAddr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSlotAddr, CDialog)
	//{{AFX_MSG_MAP(CSlotAddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlotAddr message handlers

BOOL CSlotAddr::OnInitDialog() 
{
	CDialog::OnInitDialog();

  if (!bNew)
    ctlSlotAddr().SetWindowText(pSlotStrings->GetAt(pos));

  ctlSlotAddr().SetLimitText(15);
  ctlSlotAddr().SetFocus();
  ctlSlotAddr().SetSel(0, -1);

	return FALSE;  // return TRUE unless you set the focus to a control
}

void CSlotAddr::OnOK() 
{
	CString str;
  int i, len, cntstr;
	
  // Check the Slot Address.  It must contain only upper case letters or 
  // numbers, a maximum of 15 characters, be non-null, and not be a duplicate
  // name.  (The control enforces caps and a length limit of 15.)
  cntstr = pSlotStrings->GetSize();
  ctlSlotAddr().GetWindowText(str);
  len = str.GetLength();
  if (len == 0) {
    MessageBox("The Slot Address cannot be empty (Press Cancel to not make a change).");
    ctlSlotAddr().SetFocus();
    ctlSlotAddr().SetSel(0, -1);
    return;
  }
  // Check for a dup slot address, but if we are editing, accept a string that
  // is unchanged.
  for (i = 0; i < cntstr; i++) {
    if (str == pSlotStrings->GetAt(i) && (bNew || i != pos)) {
      MessageBox("Slot Addresses must be unique.");
      ctlSlotAddr().SetFocus();
      ctlSlotAddr().SetSel(0, -1);
      return;
    }
  }

  // Everything should be OK.  Save the data
  if (bNew) {
    if (pos == -1)
      pos = pSlotStrings->Add(str);
    else
      pSlotStrings->InsertAt(pos, str);
  } else {
    pSlotStrings->SetAt(pos, str);
  }

	CDialog::OnOK();
}
