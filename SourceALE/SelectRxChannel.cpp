// SelectRxChannel.cpp : implementation file
//  18 Nov 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "SelectRxChannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectRxChannel dialog


CSelectRxChannel::CSelectRxChannel(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectRxChannel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectRxChannel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CSelectRxChannel, CDialog)
	//{{AFX_MSG_MAP(CSelectRxChannel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectRxChannel message handlers

BOOL CSelectRxChannel::OnInitDialog() 
{
  int i, numChans, index;
  struct CALEDoc::CHANNEL ch;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;
  CString str;

	CDialog::OnInitDialog();
	
	numChans = pData->parrChannels->GetSize();

  // If there aren't any Channels, display error and then quit Dialog Box
  if (!numChans) {
    MessageBox("There aren't any Channels configured.\nFirst enter Channel info in the Edit section.",
      "Alert", MB_OK);
    EndDialog(IDCANCEL);
  }

  // Set the list box contents to the possible Channels
  ctlList().InsertColumn(0, "Tx Freq", LVCFMT_RIGHT, 64);
  ctlList().InsertColumn(1, "Tx Mode", LVCFMT_CENTER, 60);
  ctlList().InsertColumn(2, "Rx Freq", LVCFMT_RIGHT, 64);
  ctlList().InsertColumn(3, "Rx Mode", LVCFMT_CENTER, 60);

  switch (iSelfAddrMode) {
    case 0:
      // List all of the channels 
      for (i = 0; i < numChans; i++) {
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
      }
      break;
    case 1:
      // If iSelfAddrMode = 1, then only list the channels for the specified Self Address
      sa = pData->parrSelfAddrs->GetAt(iSelfAddr);
      for (i = 0; i < numChans; i++) {
        ch = pData->parrChannels->GetAt(i);
        if (sa.parrValidChannels->GetAt(i) &&
            (ch.eTRMode == CALEDoc::TR)) {
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
        }
      }
      break;
    case 2:
      // List the channels that are common to both the specified Self
      // Address and the specified Other Address
      sa = pData->parrSelfAddrs->GetAt(iSelfAddr);
      oa = pData->parrOtherAddrs->GetAt(iOtherAddr);
      for (i = 0; i < numChans; i++) {
        ch = pData->parrChannels->GetAt(i);
        if (sa.parrValidChannels->GetAt(i) && oa.parrValidChannels->GetAt(i) &&
            (ch.eTRMode == CALEDoc::TR)) {
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
        }
      }
      break;
  }

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelectRxChannel::OnOK()
{
  CString strList, strChan;
  int i, ind, numChans;
  struct CALEDoc::CHANNEL ch;
  POSITION pos;

  pos = ctlList().GetFirstSelectedItemPosition();
  // If no selection then error
  if (pos == NULL) {
    MessageBox("Select one of the Channels (or press Cancel).");
    ctlList().SetFocus();
    return;
  }

  ind = ctlList().GetNextSelectedItem(pos);
  strList = ctlList().GetItemText(ind, 0);

  numChans = pData->parrChannels->GetSize();
  for (i = 0; i < numChans; i++) {
    ch = pData->parrChannels->GetAt(i);
    strChan.Format("%f", ch.dTransFreq);
    if (strList == strChan) {
      iChanNum = i;
      break;
    }
  }

	CDialog::OnOK();
}
