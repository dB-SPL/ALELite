// ChannelDlg.cpp : implementation file
//  19 Nov 1999

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "ChannelDlg.h"
#include "Dlgfloat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChannelDlg dialog


CChannelDlg::CChannelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChannelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChannelDlg)
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CChannelDlg, CDialog)
	//{{AFX_MSG_MAP(CChannelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelDlg message handlers

BOOL CChannelDlg::OnInitDialog() 
{
  CString str;
  struct CALEDoc::CHANNEL ch;

  CDialog::OnInitDialog();
	
	// Initialize the ComboBox controls
  ctlComboTxMode().AddString("USB");
  ctlComboTxMode().AddString("LSB");
  ctlComboTxMode().AddString("CW");
  ctlComboTxMode().AddString("AM");
  ctlComboRxMode().AddString("USB");
  ctlComboRxMode().AddString("LSB");
  ctlComboRxMode().AddString("CW");
  ctlComboRxMode().AddString("AM");
  ctlComboTRMode().AddString("Rx Only");
  ctlComboTRMode().AddString("Tx/Rx");
  ctlComboScanMode().AddString("On");
  ctlComboScanMode().AddString("Off");

  if (bNew) {
    ctlComboTxMode().SelectString(-1, "USB");
    ctlComboRxMode().SelectString(-1, "USB");
    ctlComboTRMode().SelectString(-1, "Tx/Rx");
    ctlComboScanMode().SelectString(-1, "On");
    ctlSoundInt().SetWindowText("0");
  } else {
    ch = pData->parrChannels->GetAt(pos);
    str.Format("%f", ch.dTransFreq);
    ctlTxFreq().SetWindowText(str);

    if (ch.eTransMode == CRadio::USB)
      str = "USB";
    else if (ch.eTransMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eTransMode == CRadio::CW)
      str = "CW";
    else if (ch.eTransMode == CRadio::AM)
      str = "AM";
    ctlComboTxMode().SelectString(-1, str);

    str.Format("%f", ch.dRecFreq);
    ctlRxFreq().SetWindowText(str);

    if (ch.eRecMode == CRadio::USB)
      str = "USB";
    else if (ch.eRecMode == CRadio::LSB)
      str = "LSB";
    else if (ch.eRecMode == CRadio::CW)
      str = "CW";
    else if (ch.eRecMode == CRadio::AM)
      str = "AM";
    ctlComboRxMode().SelectString(-1, str);

    if (ch.eTRMode == CALEDoc::TR)
      str = "Tx/Rx";
    else
      str = "Rx Only";
    ctlComboTRMode().SelectString(-1, str);

    if (ch.bScanMode)
      str = "On";
    else
      str = "Off";
    ctlComboScanMode().SelectString(-1, str);

    str.Format("%.1f", ch.dSoundInterval);
    ctlSoundInt().SetWindowText(str);
  }
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CChannelDlg::OnOK() 
{
  CString str;
  double dTxFreq, dRxFreq, dSoundInt;
  CRadio::RADIOMODE eTxMode, eRxMode;
  CALEDoc::TRMODE eTRMode;
  BOOLEAN bScanMode;
  struct CALEDoc::CHANNEL ch;

  // Check all the fields for validity
  // May need to make these checks radio specific
  // Is the transmit frequency in range?
  ctlTxFreq().GetWindowText(str);
  if (!_AfxSimpleFloatParse(str, dTxFreq) || (dTxFreq > 30.0) || (dTxFreq < 3.0)) {
    MessageBox("A valid tranmit frequency must be supplied");
    ctlTxFreq().SetFocus();
    ctlTxFreq().SetSel(0, -1);
    return;
  }
	
  // Is the receive frequency in range?
  ctlRxFreq().GetWindowText(str);
  if (!_AfxSimpleFloatParse(str, dRxFreq) || (dRxFreq > 30.0) || (dRxFreq < 3.0)) {
    MessageBox("A valid receive frequency must be supplied");
    ctlRxFreq().SetFocus();
    ctlRxFreq().SetSel(0, -1);
    return;
  }

  // Is the sound interval valid
  ctlSoundInt().GetWindowText(str);
  if (!_AfxSimpleFloatParse(str, dSoundInt) || (dSoundInt < 0.0)) {
    MessageBox("A valid sound interval must be supplied");
    ctlSoundInt().SetFocus();
    ctlSoundInt().SetSel(0, -1);
    return;
  }

  // Receive only cannot be selected with sounding on
  ctlComboTRMode().GetWindowText(str);
  eTRMode = str.Compare("Rx Only") ? CALEDoc::TR : CALEDoc::RECONLY;
  if ((dSoundInt > 0) && (eTRMode == CALEDoc::RECONLY)) {
    MessageBox("Having a channel Receive Only is incompatible with Sounding on the channel.\nEither allow transmissions or set the Sounding Interval to 0.");
    ctlComboTRMode().SetFocus();
    return;
  }

  // Everything should be OK.  Save the data
  ctlComboTxMode().GetWindowText(str);
  if (str.Compare("USB") == 0)
    eTxMode = CRadio::USB;
  else if (str.Compare("LSB") == 0)
    eTxMode = CRadio::LSB;
  else if (str.Compare("CW") == 0)
    eTxMode = CRadio::CW;
  else if (str.Compare("AM") == 0)
    eTxMode = CRadio::AM;
  ctlComboRxMode().GetWindowText(str);
  if (str.Compare("USB") == 0)
    eRxMode = CRadio::USB;
  else if (str.Compare("LSB") == 0)
    eRxMode = CRadio::LSB;
  else if (str.Compare("CW") == 0)
    eRxMode = CRadio::CW;
  else if (str.Compare("AM") == 0)
    eRxMode = CRadio::AM;
  ctlComboScanMode().GetWindowText(str);
  bScanMode = (BOOLEAN) str.Compare("Off");

  ch.dTransFreq = dTxFreq;
  ch.eTransMode = eTxMode;
  ch.dRecFreq = dRxFreq;
  ch.eRecMode = eRxMode;
  ch.eTRMode = eTRMode;
  ch.bScanMode = bScanMode;
  ch.dSoundInterval = dSoundInt;

  if (bNew) {
    if (pos == -1)
      pos = pData->parrChannels->Add(ch);
    else
      pData->parrChannels->InsertAt(pos, ch);
  } else {
    pData->parrChannels->SetAt(pos, ch);
  }
	
	CDialog::OnOK();
}
