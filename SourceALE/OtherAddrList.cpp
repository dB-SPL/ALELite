// OtherAddrList.cpp : implementation file
//  3 Feb 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "OtherAddrList.h"
#include "ALEView.h"
#include "OtherAddrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherAddrList dialog

COtherAddrList::COtherAddrList(CWnd* pParent /*=NULL*/)
	: CDialog(COtherAddrList::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherAddrList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(COtherAddrList, CDialog)
	//{{AFX_MSG_MAP(COtherAddrList)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_ADD_OADDR, OnAddOaddr)
	ON_BN_CLICKED(IDC_DELETE_OADDR, OnDeleteOaddr)
	ON_BN_CLICKED(IDC_EDIT_OADDR, OnEditOaddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherAddrList message handlers

void COtherAddrList::AddOtherAddrList(int i)
{
  CString str;
  struct CALEDoc::OTHERADDR oa;
  int index;

  oa = pData->parrOtherAddrs->GetAt(i);
  index = ctlList().InsertItem(i, oa.strOtherAddr);
  str.Format("%.2f", oa.dReplyWait);
  ctlList().SetItemText(index, 1, str);
}

void COtherAddrList::DispOtherAddrList()
{
  int size = pData->parrOtherAddrs->GetSize();

  ctlList().InsertColumn(0, "Other Address", LVCFMT_LEFT, 120);
  ctlList().InsertColumn(1, "Wait for Reply", LVCFMT_LEFT, 90);

  CString str;
  for (int i = 0; i < size; i++) {
    AddOtherAddrList(i);
  }

  // Enable or disable the arrow buttons on the dialog box depending on how
  // many self address entries exist.
  if (size >= 2) {
    m_ctlMoveUp.EnableWindow(TRUE);
    m_ctlMoveDown.EnableWindow(TRUE);
  } else {
    m_ctlMoveUp.EnableWindow(FALSE);
    m_ctlMoveDown.EnableWindow(FALSE);
  }
}

BOOL COtherAddrList::OnInitDialog() 
{
	CDialog::OnInitDialog();

  m_ctlMoveUp.AutoLoad(IDC_MOVEUP, this);
  m_ctlMoveDown.AutoLoad(IDC_MOVEDOWN, this);
  DispOtherAddrList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void COtherAddrList::OnAddOaddr() 
{
  // Run the dialog box for an individual other address.  Assume the dialog box
  // checks that all data is correct and complete, and that it will add
  // the new other address to the parrOtherAddrs array.

	COtherAddrDlg dlg(this);

  // If there is a other address selected, make the insertion before the selection.
  // If not, put it at the end of the list
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL)
    dlg.pos = -1;
  else
    dlg.pos = (int) --pos;

  dlg.pData = pData;
  dlg.bNew = TRUE;
  if (dlg.DoModal() == IDOK) {
    // Add the new record to the list control
    int i = dlg.pos;
    AddOtherAddrList(i);
	
    ctlList().SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);
    pData->pDoc->SetModifiedFlag();

    // If the number of other addresses has gone above 1, enable the arrow buttons.
    if (pData->parrOtherAddrs->GetSize() >= 2) {
      m_ctlMoveUp.EnableWindow(TRUE);
      m_ctlMoveDown.EnableWindow(TRUE);
    }
  }
  ctlList().SetFocus();
}

void COtherAddrList::OnDeleteOaddr() 
{
  struct CALEDoc::OTHERADDR oa;

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't an Other Address selected!", "Alert", MB_OK);
  } else {
    pos--;
    ctlList().DeleteItem((int) pos);
    oa = pData->parrOtherAddrs->GetAt((int) pos);
    delete oa.parrValidChannels;
    delete oa.pSlotStrings;
    pData->parrOtherAddrs->RemoveAt((int) pos);
    pData->pDoc->SetModifiedFlag();

    // If the number of other addresses has gone below 2, disable the arrow buttons.
    if (pData->parrOtherAddrs->GetSize() < 2) {
      m_ctlMoveUp.EnableWindow(FALSE);
      m_ctlMoveDown.EnableWindow(FALSE);
    }
  }
}

void COtherAddrList::OnEditOaddr() 
{
  // Run the dialog box for an individual other address.  Assume the dialog box
  // checks that all data is correct and complete, and that it will add
  // the new self address to the parrOtherAddrs array.

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL)
    MessageBox("There isn't an Other Address selected!", "Alert", MB_OK);
  else {
  	COtherAddrDlg dlg(this);
    dlg.pos = (int) --pos;
    dlg.pData = pData;
    dlg.bNew = FALSE;
    if (dlg.DoModal() == IDOK) {
      // Delete the old record from the list control and add the modified one.
      int i = (int) pos;
      ctlList().DeleteItem(i);
      AddOtherAddrList(i);

      ctlList().SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,
        LVIS_SELECTED | LVIS_FOCUSED);
      pData->pDoc->SetModifiedFlag();
    }
  }
  ctlList().SetFocus();
}

void COtherAddrList::OnMovedown() 
{
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't an Other Address selected!", "Alert", MB_OK);
  } else if ((int) pos == pData->parrSelfAddrs->GetSize()) {
    MessageBox("The selected item is already last!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CALEDoc::OTHERADDR oa = pData->parrOtherAddrs->GetAt((int) pos);
    pData->parrOtherAddrs->RemoveAt((int) pos);
    pos++;
    pData->parrOtherAddrs->InsertAt((int) pos, oa);

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) --pos);
    AddOtherAddrList((int) ++pos);

    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    pData->pDoc->SetModifiedFlag();
  }
  ctlList().SetFocus();
}

void COtherAddrList::OnMoveup() 
{
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't an Other Address selected!", "Alert", MB_OK);
  } else if ((int) pos == 1) {
    MessageBox("The selected item is already first!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CALEDoc::OTHERADDR oa = pData->parrOtherAddrs->GetAt((int) pos);
    pData->parrOtherAddrs->RemoveAt((int) pos);
    pos--;
    pData->parrOtherAddrs->InsertAt((int) pos, oa);

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) ++pos);
    AddOtherAddrList((int) --pos);

    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    pData->pDoc->SetModifiedFlag();
  }
  ctlList().SetFocus();
}

