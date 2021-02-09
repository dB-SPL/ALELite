// Icom.h: interface for the CIcom class, a type of CRadio.
//  1 Jul 1999
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICOM_H__0A0160A1_0209_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_ICOM_H__0A0160A1_0209_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Icom.h : header file
//

class CIcom : public CRadio
{
// Construction
public:
	CIcom(HANDLE hComm);

// Attributes
public:

// Operations
public:

// Implementation
public:
	virtual ~CIcom();
  virtual void Initialize(BYTE bRadioAddress);
  virtual void Tune(double dFreq);
  virtual void SetMode(enum RADIOMODE Mode);

private:
  BYTE BCD(int twodig);
};

#endif // !defined(AFX_ICOM_H__0A0160A1_0209_11D3_8F3A_0060089F9D1C__INCLUDED_)
