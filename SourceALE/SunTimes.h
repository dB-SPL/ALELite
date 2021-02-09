// SunTimes.h: interface for the CSunTimes class.
//   19 Oct 1999
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUNTIMES_H__1D8715E4_860F_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SUNTIMES_H__1D8715E4_860F_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSunTimes  
{
private:
  double PI;

public:
	CSunTimes();
	virtual ~CSunTimes();
  double GetSunRiseGMT(double latitude, double longitude);
  double GetSunSetGMT(double latitude, double longitude);

private:
  int JulianDay(int mn, int dy, int lpyr);
  int IsLeapYear(int yr);
  double CalcGamma(int julianDay);
  double CalcGamma2(int julianDay, double hour);
  double CalcEqofTime(double gamma);
  double CalcSolarDec(double gamma);
  double CalcHourAngle(double lat, double solarDec, BOOL bSunRise);
  double DegToRad(double angleDeg);
  double RadToDeg(double angleRad);
};

#endif // !defined(AFX_SUNTIMES_H__1D8715E4_860F_11D3_8F3A_0060089F9D1C__INCLUDED_)
