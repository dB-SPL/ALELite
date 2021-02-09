// SelfAddrList.cpp : implementation file
//  3 Feb 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "SelfAddrList.h"
#include "ALEView.h"
#include "SelfAddrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelfAddrList dialog


CSelfAddrList::CSelfAddrList(CWnd* pParent /*=NULL*/)
	: CDialog(CSelfAddrList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelfAddrList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSelfAddrList, CDialog)
	//{{AFX_MSG_MAP(CSelfAddrList)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	ON_BN_CLICKED(IDC_ADD_SADDR, OnAddSaddr)
	ON_BN_CLICKED(IDC_DELETE_SADDR, OnDeleteSaddr)
	ON_BN_CLICKED(IDC_EDIT_SADDR, OnEditSaddr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelfAddrList message handlers

void CSelfAddrList::AddAddrList(int i)
{
  CString str;
  struct CALEDoc::SELFADDR sa;
  int index;

  sa = pData->parrSelfAddrs->GetAt(i);
  index = ctlList().InsertItem(i, sa.strSelfAddr);
  ctlList().SetItemText(index, 1, sa.strNetAddr);
  str.Format("%d", sa.m_nSlotWait);
  ctlList().SetItemText(index, 2, str);
}

void CSelfAddrList::DispSelfAddrList()
{
  int size = pData->parrSelfAddrs->GetSize();

  ctlList().InsertColumn(0, "Self Address", LVCFMT_LEFT, 120);
  ctlList().InsertColumn(1, "Net Address", LVCFMT_LEFT, 120);
  ctlList().InsertColumn(2, "Slot Wait", LVCFMT_LEFT, 64);

  for (int i = 0; i < size; i++) {
    AddAddrList(i);
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

BOOL CSelfAddrList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  m_ctlMoveUp.AutoLoad(IDC_MOVEUP, this);
  m_ctlMoveDown.AutoLoad(IDC_MOVEDOWN, this);
	DispSelfAddrList();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelfAddrList::OnMovedown() 
{
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Self Address selected!", "Alert", MB_OK);
  } else if ((int) pos == 1) {
    MessageBox("The first self address is the base 3 character address and can't be moved.", "Alert", MB_OK);
  } else if ((int) pos == pData->parrSelfAddrs->GetSize()) {
    MessageBox("The selected item is already last!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CALEDoc::SELFADDR sa = pData->parrSelfAddrs->GetAt((int) pos);
    pData->parrSelfAddrs->RemoveAt((int) pos);
    pos++;
    pData->parrSelfAddrs->InsertAt((int) pos, sa);

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) --pos);
    AddAddrList((int) ++pos);

    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    pData->pDoc->SetModifiedFlag();
  }
  ctlList().SetFocus();
}

void CSelfAddrList::OnMoveup() 
{
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Self Address selected!", "Alert", MB_OK);
  } else if ((int) pos == 1) {
    MessageBox("The selected item is already first!", "Alert", MB_OK);
  } else if ((int) pos == 2) {
    MessageBox("The first self address is the base 3 character address and can't be moved.", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CALEDoc::SELFADDR sa = pData->parrSelfAddrs->GetAt((int) pos);
    pData->parrSelfAddrs->RemoveAt((int) pos);
    pos--;
    pData->parrSelfAddrs->InsertAt((int) pos, sa);

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) ++pos);
    AddAddrList((int) --pos);

    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    pData->pDoc->SetModifiedFlag();
  }
  ctlList().SetFocus();
}

void CSelfAddrList::OnAddSaddr() 
{
  // Run the dialog box for an individual self address.  Assume the dialog box
  // checks that all data is correct and complete, and that it will add
  // the new self address to the parrSelfAddrs array.

	CSelfAddrDlg dlg(this);

  // If there is a self address selected, make the insertion before the selection.
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
    AddAddrList(i);
	
    ctlList().SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);
    pData->pDoc->SetModifiedFlag();

    // If the number of self addresses has gone above 1, enable the arrow buttons.
    if (pData->parrSelfAddrs->GetSize() >= 2) {
      m_ctlMoveUp.EnableWindow(TRUE);
      m_ctlMoveDown.EnableWindow(TRUE);
    }
  }
  ctlList().SetFocus();
}

void CSelfAddrList::OnDeleteSaddr()
{
  struct CALEDoc::SELFADDR sa;

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Self Address selected!", "Alert", MB_OK);
  } else if ((int) pos == 1) {
    MessageBox("The first self address is the base 3 character address and can't be deleted.", "Alert", MB_OK);
  } else {
    pos--;
    ctlList().DeleteItem((int) pos);
    sa = pData->parrSelfAddrs->GetAt((int) pos);
    delete sa.parrValidChannels;
    pData->parrSelfAddrs->RemoveAt((int) pos);
    pData->pDoc->SetModifiedFlag();

    // If the number of self addresses has gone below 2, disable the arrow buttons.
    if (pData->parrSelfAddrs->GetSize() < 2) {
      m_ctlMoveUp.EnableWindow(FALSE);
      m_ctlMoveDown.EnableWindow(FALSE);
    }
  }
}

void CSelfAddrList::OnEditSaddr() 
{
  // Run the dialog box for an individual self address.  Assume the dialog box
  // checks that all data is correct and complete, and that it will add
  // the new self address to the parrSelfAddrs array.

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL)
    MessageBox("There isn't a Self Address selected!", "Alert", MB_OK);
  else {
  	CSelfAddrDlg dlg(this);
    dlg.pos = (int) --pos;
    dlg.pData = pData;
    dlg.bNew = FALSE;
    if (dlg.DoModal() == IDOK) {
      // Delete the old record from the list control and add the modified one.
      int i = (int) pos;
      ctlList().DeleteItem(i);
      AddAddrList(i);

      ctlList().SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,
        LVIS_SELECTED | LVIS_FOCUSED);
      pData->pDoc->SetModifiedFlag();
    }
  }
  ctlList().SetFocus();
}
