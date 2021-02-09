// Radio.h: interface for the CRadio class.
//  31 Aug 1999
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADIO_H__23CB3481_3120_11D2_8F37_0060089F9D1C__INCLUDED_)
#define AFX_RADIO_H__23CB3481_3120_11D2_8F37_0060089F9D1C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRadio
{
public:
  enum RADIOMODE {NULL_MODE, USB, LSB, CW, AM};

protected:
  HANDLE hComm;
  enum RADIOMODE eCurMode;
  double dCurFreq;
  BYTE baMessage[20];

public:
	CRadio();
	virtual ~CRadio();
  virtual void Initialize(BYTE bRadioAddress);
  virtual void Tune(double dFreq);
  void SetFreq(double dFreq);
  double GetFreq();
  virtual void SetMode(enum RADIOMODE Mode);
  enum RADIOMODE GetMode();
};

#endif // !defined(AFX_RADIO_H__23CB3481_3120_11D2_8F37_0060089F9D1C__INCLUDED_)
