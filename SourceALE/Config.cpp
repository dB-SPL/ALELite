// Config.cpp : implementation file
//  30 Jun 1999

#include "stdafx.h"
#include "DlgFloat.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog


CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CConfig, CDialog)
	//{{AFX_MSG_MAP(CConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig message handlers

BOOL CConfig::OnInitDialog() 
{
  CString str;

  CDialog::OnInitDialog();

  // Initialize the ComboBox controls
  ctlComboRadio().AddString("ICOM");

  ctlComboCommPort().AddString("COM1");
  ctlComboCommPort().AddString("COM2");
  ctlComboCommPort().AddString("COM3");
  ctlComboCommPort().AddString("COM4");

  ctlComboBaud().AddString("19200");
  ctlComboBaud().AddString("9600");
  ctlComboBaud().AddString("4800");
  ctlComboBaud().AddString("2400");
  ctlComboBaud().AddString("1200");

  if (pData->pConfig->eRadioType == ICOM)
    ctlComboRadio().SelectString(-1, "ICOM");

  ctlRadioAddr().SetLimitText(2);
  str.Format("%2X", pData->pConfig->bRadioAddress);
  ctlRadioAddr().SetWindowText(str);

  if (ctlComboCommPort().SelectString(-1, pData->pConfig->strCommPort) == CB_ERR) {
    ctlComboCommPort().AddString(pData->pConfig->strCommPort);
    ctlComboCommPort().SelectString(-1, pData->pConfig->strCommPort);
  }

  switch (pData->pConfig->lBaud) {
    case 19200:
      ctlComboBaud().SelectString(-1, "19200");
      break;
    case 9600:
      ctlComboBaud().SelectString(-1, "9600");
      break;
    case 4800:
      ctlComboBaud().SelectString(-1, "4800");
      break;
    case 2400:
      ctlComboBaud().SelectString(-1, "2400");
      break;
    case 1200:
      ctlComboBaud().SelectString(-1, "1200");
      break;
  }

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CConfig::OnOK() 
{
  COMMPROP CommProp;
  CString strCommPort, str;
  long bRadioAddress;
  long lBaud;
  DWORD mask;

	// Assume the Radio is OK

  // Check that the Radio Address is between 1 and 77 (hex)
  ctlRadioAddr().GetWindowText(str);
  if (!SimpleHexParse(str, bRadioAddress) || (bRadioAddress <= 0) || (bRadioAddress > 127)) {
    MessageBox("The Radio Address must be a hexadecimal number between 1 and 7F.");
    ctlRadioAddr().SetFocus();
    ctlRadioAddr().SetSel(0, -1);
    return;
  }

  // Comm Ports may have non-standard names.  Check and see if this computer has the named comm port.
  // Try to open comm port
  ctlComboCommPort().GetWindowText(strCommPort);
  if (hComm != 0)
    CloseHandle(hComm);
  if ((hComm = CreateFile(strCommPort, GENERIC_READ | GENERIC_WRITE,
    0, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
    MessageBox("The selected Serial Comm Port is busy or doesn't exist.");
    ctlComboCommPort().SetFocus();
    hComm = 0;
    return;
  }

  // Check if this baud rate is supported on this comm port
  GetCommProperties(hComm, &CommProp);
  ctlComboBaud().GetWindowText(str);
  _AfxSimpleLongParse(str, lBaud);
  switch (lBaud) {
    case 1200:
      mask = BAUD_1200;
      break;
    case 2400:
      mask = BAUD_2400;
      break;
    case 4800:
      mask = BAUD_4800;
      break;
    case 9600:
      mask = BAUD_9600;
      break;
    case 19200:
      mask = BAUD_19200;
      break;
  }

  if (!(CommProp.dwSettableBaud & mask)) {
    MessageBox("The Serial Port does not support this baud rate.");
    ctlComboCommPort().SetFocus();
    CloseHandle(hComm);
    return;
  }

  // Everything is OK.  Save the configuration.
  ctlComboRadio().GetWindowText(str);
  if (str == "ICOM")
    pData->pConfig->eRadioType = ICOM;
  pData->pConfig->strCommPort = strCommPort;
  pData->pConfig->bRadioAddress = (BYTE) bRadioAddress;
  pData->pConfig->lBaud = lBaud;
  pData->pDoc->SetModifiedFlag();

  CloseHandle(hComm);
	CDialog::OnOK();
}
