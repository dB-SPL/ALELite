// SlotList.cpp : implementation file
//  14 Jan 1999

#include "stdafx.h"
#include "ALE.h"
#include "SlotList.h"
#include "SlotAddr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSlotList dialog


CSlotList::CSlotList(CWnd* pParent /*=NULL*/)
	: CDialog(CSlotList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSlotList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSlotList, CDialog)
	//{{AFX_MSG_MAP(CSlotList)
	ON_BN_CLICKED(IDC_ADD_SADDR, OnAddSaddr)
	ON_BN_CLICKED(IDC_DELETE_SADDR, OnDeleteSaddr)
	ON_BN_CLICKED(IDC_EDIT_SADDR, OnEditSaddr)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMovedown)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSlotList message handlers

void CSlotList::DispSlotList()
{
  int index;

  int size = pSlotStrings->GetSize();

  ctlList().InsertColumn(0, "Slot Address", LVCFMT_LEFT, 120);

  for (int i = 0; i < size; i++) {
    index = ctlList().InsertItem(i, pSlotStrings->GetAt(i));
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

BOOL CSlotList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  m_ctlMoveUp.AutoLoad(IDC_MOVEUP, this);
  m_ctlMoveDown.AutoLoad(IDC_MOVEDOWN, this);
	DispSlotList();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSlotList::OnMovedown() 
{
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Slot Address selected!", "Alert", MB_OK);
  } else if ((int) pos == pSlotStrings->GetSize()) {
    MessageBox("The selected item is already last!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CString str = pSlotStrings->GetAt((int) pos);
    pSlotStrings->RemoveAt((int) pos);
    pos++;
    pSlotStrings->InsertAt((int) pos, str);

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) --pos);
    ctlList().InsertItem((int) ++pos, str);

    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);
  }
  ctlList().SetFocus();
}

void CSlotList::OnMoveup() 
{
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Slot Address selected!", "Alert", MB_OK);
  } else if ((int) pos == 1) {
    MessageBox("The selected item is already first!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CString str = pSlotStrings->GetAt((int) pos);
    pSlotStrings->RemoveAt((int) pos);
    pos--;
    pSlotStrings->InsertAt((int) pos, str);

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) ++pos);
    ctlList().InsertItem((int) --pos, str);

    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);
  }
  ctlList().SetFocus();
}

void CSlotList::OnDeleteSaddr() 
{
  CString str;

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Slot Address selected!", "Alert", MB_OK);
  } else {
    pos--;
    ctlList().DeleteItem((int) pos);
    pSlotStrings->RemoveAt((int) pos);

    // If the number of self addresses has gone below 2, disable the arrow buttons.
    if (pSlotStrings->GetSize() < 2) {
      m_ctlMoveUp.EnableWindow(FALSE);
      m_ctlMoveDown.EnableWindow(FALSE);
    }
  }
}

void CSlotList::OnAddSaddr() 
{
  // Run the dialog box for an individual slot address.

	CSlotAddr dlg(this);

  // If there is a slot address selected, make the insertion before the selection.
  // If not, put it at the end of the list
	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL)
    dlg.pos = -1;
  else
    dlg.pos = (int) --pos;

  dlg.pSlotStrings = pSlotStrings;
  dlg.bNew = TRUE;
  if (dlg.DoModal() == IDOK) {
    // Add the new record to the list control
    int i = dlg.pos;
    ctlList().InsertItem((int) i, pSlotStrings->GetAt(i));
	
    ctlList().SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    // If the number of self addresses has gone above 1, enable the arrow buttons.
    if (pSlotStrings->GetSize() >= 2) {
      m_ctlMoveUp.EnableWindow(TRUE);
      m_ctlMoveDown.EnableWindow(TRUE);
    }
  }
  ctlList().SetFocus();
}

void CSlotList::OnEditSaddr() 
{
  // Run the dialog box for an individual slot address.

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL)
    MessageBox("There isn't a Slot Address selected!", "Alert", MB_OK);
  else {
  	CSlotAddr dlg(this);
    dlg.pos = (int) --pos;
    dlg.pSlotStrings = pSlotStrings;
    dlg.bNew = FALSE;
    if (dlg.DoModal() == IDOK) {
      // Delete the old string from the list control and add the modified one.
      int i = (int) pos;
      ctlList().DeleteItem(i);
      ctlList().InsertItem((int) i, pSlotStrings->GetAt(i));

      ctlList().SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED,
        LVIS_SELECTED | LVIS_FOCUSED);
    }
  }
  ctlList().SetFocus();
}
