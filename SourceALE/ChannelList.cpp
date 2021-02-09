// ChannelList.cpp : implementation file
//  20 Jan 2000

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "ChannelList.h"
#include "ALEView.h"
#include "ChannelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelList dialog


CChannelList::CChannelList(CWnd* pParent /*=NULL*/)
	: CDialog(CChannelList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChannelList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CChannelList, CDialog)
	//{{AFX_MSG_MAP(CChannelList)
	ON_BN_CLICKED(IDC_DELETE_CHAN, OnDeleteChan)
	ON_BN_CLICKED(IDC_ADD_CHAN, OnAddChan)
	ON_BN_CLICKED(IDC_EDIT_CHAN, OnEditChan)
	ON_BN_CLICKED(IDC_MOVEUP, OnMoveUp)
	ON_BN_CLICKED(IDC_MOVEDOWN, OnMoveDown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelList message handlers

void CChannelList::DispChannelList()
{
  int size = pData->parrChannels->GetSize();

  ctlList().InsertColumn(0, "Tx Freq", LVCFMT_RIGHT, 64);
  ctlList().InsertColumn(1, "Tx Mode", LVCFMT_CENTER, 60);
  ctlList().InsertColumn(2, "Rx Freq", LVCFMT_RIGHT, 64);
  ctlList().InsertColumn(3, "Rx Mode", LVCFMT_CENTER, 60);
  ctlList().InsertColumn(4, "TR Mode", LVCFMT_CENTER, 60);
  ctlList().InsertColumn(5, "Scan Mode", LVCFMT_CENTER, 80);
  ctlList().InsertColumn(6, "Sound Int", LVCFMT_CENTER, 66);

  CString str;
  struct CALEDoc::CHANNEL ch;
  int index;
  for (int i = 0; i < size; i++) {
    ch = pData->parrChannels->GetAt(i);
    str.Format("%f", ch.dTransFreq);
    index = ctlList().InsertItem(i, str);
    if (ch.eTransMode == CRadio::USB)
      str = "USB";
    else if (ch.eTransMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eTransMode == CRadio::CW)
      str = "CW";
    else if (ch.eTransMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 1, str);
    str.Format("%f", ch.dRecFreq);
    ctlList().SetItemText(index, 2, str);
    if (ch.eRecMode == CRadio::USB)
      str = "USB";
    else if (ch.eRecMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eRecMode == CRadio::CW)
      str = "CW";
    else if (ch.eRecMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 3, str);
    if (ch.eTRMode == CALEDoc::TR)
      str = "Tx/Rx";
    else 
      str = "Rx Only";
    ctlList().SetItemText(index, 4, str);
    if (ch.bScanMode)
      str = "On";
    else 
      str = "Off";
    ctlList().SetItemText(index, 5, str);
    str.Format("%.1f", ch.dSoundInterval);
    ctlList().SetItemText(index, 6, str);
  }

  // Enable or disable the arrow buttons on the dialog box depending on how
  // many channel entries exist.
  if (size >= 2) {
    m_ctlMoveUp.EnableWindow(TRUE);
    m_ctlMoveDown.EnableWindow(TRUE);
  } else {
    m_ctlMoveUp.EnableWindow(FALSE);
    m_ctlMoveDown.EnableWindow(FALSE);
  }
}

BOOL CChannelList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  m_ctlMoveUp.AutoLoad(IDC_MOVEUP, this);
  m_ctlMoveDown.AutoLoad(IDC_MOVEDOWN, this);
	DispChannelList();

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CChannelList::OnDeleteChan() 
{
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;
  BOOL InUse = FALSE;
  int cnt1, cnt2, j, k;

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Channel selected!", "Alert", MB_OK);
  } else {
    pos--;

    // Check if the channel is selected for use anywhere
    cnt1 = pData->parrSelfAddrs->GetSize();
    for (j = 0; j < cnt1; j++) {
      sa = pData->parrSelfAddrs->GetAt(j);
      if (sa.parrValidChannels->GetAt((int) pos)) {
        InUse = TRUE;
        break;
      }
    }
    // Also check the other addresses
    cnt2 = pData->parrOtherAddrs->GetSize();
    if (!InUse) {
      for (j = 0; j < cnt2; j++) {
        oa = pData->parrOtherAddrs->GetAt(j);
        if (oa.parrValidChannels->GetAt((int) pos)) {
          InUse = TRUE;
          break;
        }
      }
    }

    if (InUse) {
      if (MessageBox("This channel is selected for use.  To DELETE anyway, press OK.",
        "Warning", MB_ICONWARNING | MB_OKCANCEL) == IDCANCEL)
        return;
    }

    // Delete all the references to the channel
    for (k = 0; k < cnt1; k++) {
      sa = pData->parrSelfAddrs->GetAt(k);
      sa.parrValidChannels->RemoveAt((int) pos);
    }
    for (k = 0; k < cnt2; k++) {
      oa = pData->parrOtherAddrs->GetAt(k);
      oa.parrValidChannels->RemoveAt((int) pos);
    }

    ctlList().DeleteItem((int) pos);
    pData->parrChannels->RemoveAt((int) pos);
    pData->pDoc->SetModifiedFlag();

    // If the number of channels has gone below 2, disable the arrow buttons.
    if (pData->parrChannels->GetSize() < 2) {
      m_ctlMoveUp.EnableWindow(FALSE);
      m_ctlMoveDown.EnableWindow(FALSE);
    }
  }
}

void CChannelList::OnAddChan() 
{
  // Run the dialog box for an individual channel.  Assume the dialog box
  // checks that all data is correct and complete, and that it will add
  // the new channel to the parrChannels array.

  int j, cnt;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;

	CChannelDlg dlg(this);

  // If there is a channel selected, make the insertion before the selection.
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
    CString str;
    struct CALEDoc::CHANNEL ch;
    int index;
    int i = dlg.pos;
    ch = pData->parrChannels->GetAt(i);
    str.Format("%f", ch.dTransFreq);
    index = ctlList().InsertItem(i, str);
    if (ch.eTransMode == CRadio::USB)
      str = "USB";
    else if (ch.eTransMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eTransMode == CRadio::CW)
      str = "CW";
    else if (ch.eTransMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 1, str);
    str.Format("%f", ch.dRecFreq);
    ctlList().SetItemText(index, 2, str);
    if (ch.eRecMode == CRadio::USB)
      str = "USB";
    else if (ch.eRecMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eRecMode == CRadio::CW)
      str = "CW";
    else if (ch.eRecMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 3, str);
    if (ch.eTRMode == CALEDoc::TR)
      str = "Tx/Rx";
    else 
      str = "Rx Only";
    ctlList().SetItemText(index, 4, str);
    if (ch.bScanMode)
      str = "On";
    else 
      str = "Off";
    ctlList().SetItemText(index, 5, str);
    str.Format("%.1f", ch.dSoundInterval);
    ctlList().SetItemText(index, 6, str);
	
    ctlList().SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);
    pData->pDoc->SetModifiedFlag();

    // The new channel must be added to the Self Address and Other Address
    // valid channels arrays.
    cnt = pData->parrSelfAddrs->GetSize();
    for (j = 0; j < cnt; j++) {
      sa = pData->parrSelfAddrs->GetAt(j);
      sa.parrValidChannels->InsertAt(i, (BOOLEAN) FALSE);
    }

    cnt = pData->parrOtherAddrs->GetSize();
    for (j = 0; j < cnt; j++) {
      oa = pData->parrOtherAddrs->GetAt(j);
      oa.parrValidChannels->InsertAt(i, (BOOLEAN) FALSE);
    }

    // If the number of channels has gone above 1, enable the arrow buttons.
    if (pData->parrChannels->GetSize() >= 2) {
      m_ctlMoveUp.EnableWindow(TRUE);
      m_ctlMoveDown.EnableWindow(TRUE);
    }
  }
  ctlList().SetFocus();
}

void CChannelList::OnEditChan() 
{
  // Run the dialog box for an individual channel.  Assume the dialog box
  // checks that all data is correct and complete, and that it will add
  // the new channel to the parrChannels array.

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL)
    MessageBox("There isn't a Channel selected!", "Alert", MB_OK);
  else {
  	CChannelDlg dlg(this);
    dlg.pos = (int) --pos;
    dlg.pData = pData;
    dlg.bNew = FALSE;
    if (dlg.DoModal() == IDOK) {
      // Delete the old record from the list control and add the modified one.
      int i = (int) pos;
      ctlList().DeleteItem(i);

      CString str;
      struct CALEDoc::CHANNEL ch;
      int index;
      ch = pData->parrChannels->GetAt(i);
      str.Format("%f", ch.dTransFreq);
      index = ctlList().InsertItem(i, str);
      if (ch.eTransMode == CRadio::USB)
        str = "USB";
      else if (ch.eTransMode == CRadio::LSB)
        str = "LSB";
      else if (ch.eTransMode == CRadio::CW)
        str = "CW";
      else if (ch.eTransMode == CRadio::AM)
        str = "AM";
      ctlList().SetItemText(index, 1, str);
      str.Format("%f", ch.dRecFreq);
      ctlList().SetItemText(index, 2, str);
      if (ch.eRecMode == CRadio::USB)
        str = "USB";
      else if (ch.eRecMode == CRadio::LSB)
        str = "LSB";
      else if (ch.eRecMode == CRadio::CW)
        str = "CW";
      else if (ch.eRecMode == CRadio::AM)
        str = "AM";
      ctlList().SetItemText(index, 3, str);
      if (ch.eTRMode == CALEDoc::TR)
        str = "Tx/Rx";
      else 
        str = "Rx Only";
      ctlList().SetItemText(index, 4, str);
      if (ch.bScanMode)
        str = "On";
      else 
        str = "Off";
      ctlList().SetItemText(index, 5, str);
      str.Format("%.1f", ch.dSoundInterval);
      ctlList().SetItemText(index, 6, str);
	
      ctlList().SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED,
        LVIS_SELECTED | LVIS_FOCUSED);
      pData->pDoc->SetModifiedFlag();
    }
  }
  ctlList().SetFocus();
}

void CChannelList::OnMoveUp() 
{
  int cnt1, i;
  BOOLEAN bScr;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;

  POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Channel selected!", "Alert", MB_OK);
  } else if ((int) pos == 1) {
    MessageBox("The selected item is already first!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CALEDoc::CHANNEL ch = pData->parrChannels->GetAt((int) pos);
    pData->parrChannels->RemoveAt((int) pos);
    pos--;
    pData->parrChannels->InsertAt((int) pos, ch);

    // The Self Address and Other Address valid channels arrays need to be
    // rearranged to match the new order.
    cnt1 = pData->parrSelfAddrs->GetSize();
    for (i = 0; i < cnt1; i++) {
      sa = pData->parrSelfAddrs->GetAt(i);
      bScr = sa.parrValidChannels->GetAt((int) ++pos);
      sa.parrValidChannels->RemoveAt((int) pos);
      sa.parrValidChannels->InsertAt((int) --pos, bScr);
    }

    cnt1 = pData->parrOtherAddrs->GetSize();
    for (i = 0; i < cnt1; i++) {
      oa = pData->parrOtherAddrs->GetAt(i);
      bScr = oa.parrValidChannels->GetAt((int) ++pos);
      oa.parrValidChannels->RemoveAt((int) pos);
      oa.parrValidChannels->InsertAt((int) --pos, bScr);
    }

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) ++pos);
    CString str;
    str.Format("%f", ch.dTransFreq);
    int index = ctlList().InsertItem((int) --pos, str);
    if (ch.eTransMode == CRadio::USB)
      str = "USB";
    else if (ch.eTransMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eTransMode == CRadio::CW)
      str = "CW";
    else if (ch.eTransMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 1, str);
    str.Format("%f", ch.dRecFreq);
    ctlList().SetItemText(index, 2, str);
    if (ch.eRecMode == CRadio::USB)
      str = "USB";
    else if (ch.eRecMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eRecMode == CRadio::CW)
      str = "CW";
    else if (ch.eRecMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 3, str);
    if (ch.eTRMode == CALEDoc::TR)
      str = "Tx/Rx";
    else 
      str = "Rx Only";
    ctlList().SetItemText(index, 4, str);
    if (ch.bScanMode)
      str = "On";
    else 
      str = "Off";
    ctlList().SetItemText(index, 5, str);
    str.Format("%.1f", ch.dSoundInterval);
    ctlList().SetItemText(index, 6, str);
    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    pData->pDoc->SetModifiedFlag();
  }
  ctlList().SetFocus();
}

void CChannelList::OnMoveDown()
{
  int cnt1, i;
  BOOLEAN bScr;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't a Channel selected!", "Alert", MB_OK);
  } else if ((int) pos == pData->parrChannels->GetSize()) {
    MessageBox("The selected item is already last!", "Alert", MB_OK);
  } else {
    pos--;
    // Rearrange the array elements
    CALEDoc::CHANNEL ch = pData->parrChannels->GetAt((int) pos);
    pData->parrChannels->RemoveAt((int) pos);
    pos++;
    pData->parrChannels->InsertAt((int) pos, ch);

    // The Self Address and Other Address valid channels arrays need to 
     //be rearranged to match the new order.
    cnt1 = pData->parrSelfAddrs->GetSize();
    for (i = 0; i < cnt1; i++) {
      sa = pData->parrSelfAddrs->GetAt(i);
      bScr = sa.parrValidChannels->GetAt((int) --pos);
      sa.parrValidChannels->RemoveAt((int) pos);
      sa.parrValidChannels->InsertAt((int) ++pos, bScr);
    }

    cnt1 = pData->parrOtherAddrs->GetSize();
    for (i = 0; i < cnt1; i++) {
      oa = pData->parrOtherAddrs->GetAt(i);
      bScr = oa.parrValidChannels->GetAt((int) --pos);
      oa.parrValidChannels->RemoveAt((int) pos);
      oa.parrValidChannels->InsertAt((int) ++pos, bScr);
    }

    // Rearrange the list displayed in the dialog box
    ctlList().DeleteItem((int) --pos);
    CString str;
    str.Format("%f", ch.dTransFreq);
    int index = ctlList().InsertItem((int) ++pos, str);
    if (ch.eTransMode == CRadio::USB)
      str = "USB";
    else if (ch.eTransMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eTransMode == CRadio::CW)
      str = "CW";
    else if (ch.eTransMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 1, str);
    str.Format("%f", ch.dRecFreq);
    ctlList().SetItemText(index, 2, str);
    if (ch.eRecMode == CRadio::USB)
      str = "USB";
    else if (ch.eRecMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eRecMode == CRadio::CW)
      str = "CW";
    else if (ch.eRecMode == CRadio::AM)
      str = "AM";
    ctlList().SetItemText(index, 3, str);
    if (ch.eTRMode == CALEDoc::TR)
      str = "Tx/Rx";
    else 
      str = "Rx Only";
    ctlList().SetItemText(index, 4, str);
    if (ch.bScanMode)
      str = "On";
    else 
      str = "Off";
    ctlList().SetItemText(index, 5, str);
    str.Format("%.1f", ch.dSoundInterval);
    ctlList().SetItemText(index, 6, str);
    // Keep the item that was deleted and reinserted highlighted
    ctlList().SetItemState((int) pos, LVIS_SELECTED | LVIS_FOCUSED,
      LVIS_SELECTED | LVIS_FOCUSED);

    pData->pDoc->SetModifiedFlag();
  }
  ctlList().SetFocus();
}
