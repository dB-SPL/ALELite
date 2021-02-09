// Radio.cpp: implementation of the CRadio class.
//  31 Aug 1999
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Radio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// CRadio

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRadio::CRadio()
{
  dCurFreq = 0.0;
  eCurMode = NULL_MODE;
}

CRadio::~CRadio()
{
}

void CRadio::Initialize(BYTE bRadioAddress)
{
}

void CRadio::Tune(double dFreq)
{
  dCurFreq = dFreq;
}

void CRadio::SetFreq(double dFreq)
{
  // This is used by derived classes to save the current frequency
  dCurFreq = dFreq;
}

double CRadio::GetFreq()
{
  return dCurFreq;
}

void CRadio::SetMode(enum RADIOMODE Mode)
{
}

enum CRadio::RADIOMODE CRadio::GetMode()
{
  return eCurMode;
}
