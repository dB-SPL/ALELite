// DlgFloat.cpp
//  30 Jun 1999

// This is derived from the Microsoft Foundation Classes C++ library.

#include "stdafx.h"
#include <float.h>              // floating point precision


/////////////////////////////////////////////////////////////////////////////
// Extra data validation procs for float/double support
//  see "dlgdata.cpp" for non-floating point support
/////////////////////////////////////////////////////////////////////////////

AFX_STATIC BOOL AFXAPI _AfxSimpleFloatParse(LPCTSTR lpszText, double& d)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	d = _tcstod(lpszText, (LPTSTR*)&lpszText);  
	if (d == 0.0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}

// Here is a similar version for longs
AFX_STATIC BOOL AFXAPI _AfxSimpleLongParse(LPCTSTR lpszText, long& lo)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	lo = _tcstol(lpszText, (LPTSTR*)&lpszText, 10);
	if (lo == 0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

	return TRUE;
}

// This is for hexadecimal numbers
BOOL SimpleHexParse(LPCTSTR lpszText, long& lo)
{
	ASSERT(lpszText != NULL);
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	TCHAR chFirst = lpszText[0];
	lo = _tcstol(lpszText, (LPTSTR*)&lpszText, 16);
	if (lo == 0 && chFirst != '0')
		return FALSE;   // could not convert
	while (*lpszText == ' ' || *lpszText == '\t')
		lpszText++;

	if (*lpszText != '\0')
		return FALSE;   // not terminated properly

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
