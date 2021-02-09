// Icom.cpp : implementation file
//  20 Jan 2000
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radio.h"
#include "Icom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIcom

CIcom::CIcom(HANDLE hCommPort)
{
  baMessage[0] = baMessage[1] = 0xfe;
  baMessage[3] = 0xe0;
  hComm = hCommPort;
}

CIcom::~CIcom()
{
}

void CIcom::Initialize(BYTE bRadioAddress)
{
  // Initialize the radio to a known state
  baMessage[2] = bRadioAddress;
}

BYTE CIcom::BCD(int twodig)
{
  // twodig is assumed to be an int between 0 and 99.  The BCD
  // representation is returned.
  return (BYTE) ((twodig / 10) * 16 + twodig % 10);
}

void CIcom::Tune(double dFreq)
{
  int i;
  DWORD dwWritten;

  // Check to see if we are already on the desired frequency
  if (dCurFreq == dFreq)
    return;

  SetFreq(dFreq);

  // Tune the radio to the frequency
  baMessage[4] = 0;

  // Convert the double freq to the ICOM BCD freq
  dFreq += 0.0000005;
  i = (int) (dFreq / 100.0);
  baMessage[9] = BCD(i);
  dFreq = dFreq - i * 100;
  i = (int) dFreq;
  baMessage[8] = BCD(i);
  dFreq = (dFreq - i) * 100.0;
  i = (int) dFreq;
  baMessage[7] = BCD(i);
  dFreq = (dFreq - i) * 100.0;
  i = (int) dFreq;
  baMessage[6] = BCD(i);
  dFreq = (dFreq - i) * 100.0;
  i = (int) dFreq;
  baMessage[5] = BCD(i);

  baMessage[10] = 0xfd;

  // Write it to the comm port
  WriteFile(hComm, baMessage, 11, &dwWritten, NULL);
}

void CIcom::SetMode(enum RADIOMODE Mode)
{
  DWORD dwWritten;

  // Check to see if we are already in the desired mode
  if (eCurMode == Mode)
    return;

  // Some ICOM models require a delay here.  They will not correctly execute
  // an immediate change mode command.
  ::Sleep(15);

  // Set the mode on the radio
  if (Mode != eCurMode) {
    baMessage[4] = 1;

    if (Mode == LSB)
      baMessage[5] = 0;
    else if (Mode == USB)
      baMessage[5] = 1;
    else if (Mode == AM)
      baMessage[5] = 2;
    else if (Mode == CW)
      baMessage[5] = 3;

    baMessage[6] = 0xfd;

    // Write it to the comm port
    WriteFile(hComm, baMessage, 7, &dwWritten, NULL);

    eCurMode = Mode;
  }
}
