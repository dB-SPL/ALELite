// SunTimes.cpp: implementation of the CSunTimes class.
//  19 Oct 1999
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ALE.h"
#include "SunTimes.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSunTimes::CSunTimes()
{
  PI = 4.0 * atan(1.0);
}


CSunTimes::~CSunTimes()
{

}


double CSunTimes::GetSunRiseGMT(double latitude, double longitude)
// Calculates the time of the sunrise for the current day (as read from the system clock)
// latitude and longitude are in degrees.
// Returned value is Sun Rise time in minutes (from midnight)
{
  int julDay;
  double dGamma, dEqTime, dSolarDec, dHourAngle, dGammaSunrise, dDelta;
  double dTimeDiff, dTimeGMT;
  CTime timeCur = CTime::GetCurrentTime();

  julDay = JulianDay(timeCur.GetMonth(), timeCur.GetDay(),
    IsLeapYear(timeCur.GetYear()));

  // *** First pass to approximate sunrise
  dGamma = CalcGamma(julDay);
  dEqTime = CalcEqofTime(dGamma);
  dSolarDec = CalcSolarDec(dGamma);
  dHourAngle = CalcHourAngle(latitude, dSolarDec, TRUE);
  dDelta = longitude - RadToDeg(dHourAngle);
  dTimeDiff = 4.0 * dDelta;
  dTimeGMT = 720.0 + dTimeDiff - dEqTime;

  // *** Second pass includes fractional jday in gamma calc

  dGammaSunrise = CalcGamma2(julDay, dTimeGMT / 60.0);
  dEqTime = CalcEqofTime(dGammaSunrise);
  dSolarDec = CalcSolarDec(dGammaSunrise);
  dHourAngle = CalcHourAngle(latitude, dSolarDec, TRUE);
  dDelta = longitude - RadToDeg(dHourAngle);
  dTimeDiff = 4.0 * dDelta;
  dTimeGMT = 720.0 + dTimeDiff - dEqTime; // in minutes

  return dTimeGMT;
}


double CSunTimes::GetSunSetGMT(double latitude, double longitude)
// Calculates the time of the Sunset for the current day (as read from the system clock)
// latitude and longitude are in degrees.
// Returned value is Sun Set time in minutes (from midnight)
{
  int julDay;
  double dGamma, dEqTime, dSolarDec, dHourAngle, dGammaSunset, dDelta;
  double dTimeDiff, dSetTimeGMT;
  CTime timeCur = CTime::GetCurrentTime();

  julDay = JulianDay(timeCur.GetMonth(), timeCur.GetDay(),
    IsLeapYear(timeCur.GetYear()));

  // First calculates sunrise and approx length of day
  dGamma = CalcGamma(julDay);
  dEqTime = CalcEqofTime(dGamma);
  dSolarDec = CalcSolarDec(dGamma);
  dHourAngle = CalcHourAngle(latitude, dSolarDec, FALSE);
  dDelta = longitude - RadToDeg(dHourAngle);
  dTimeDiff = 4.0 * dDelta;
  dSetTimeGMT = 720.0 + dTimeDiff - dEqTime;

  // first pass used to include fractional day in gamma calc

  dGammaSunset = CalcGamma2(julDay, dSetTimeGMT / 60.0);
  dEqTime = CalcEqofTime(dGammaSunset);
  dSolarDec = CalcSolarDec(dGammaSunset);
  dHourAngle = CalcHourAngle(latitude, dSolarDec, FALSE);
  dDelta = longitude - RadToDeg(dHourAngle);
  dTimeDiff = 4.0 * dDelta;
  dSetTimeGMT = 720.0 + dTimeDiff - dEqTime; // in minutes

  return dSetTimeGMT;
}


int CSunTimes::JulianDay(int mn, int dy, int lpyr)
// Calculates the Julian day.  mn is the month (Jan = 1),
// dy is the day of the month, lpyr = 1 for leap years or 0 otherwise.
{
  int i, julDay = 0;
  int monthList[11] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30};

	for (i = 0; i < mn - 1; i++)
		julDay += monthList[i];

  if (mn >= 2)
    julDay += lpyr;

  julDay += dy;

  return julDay;
}


int CSunTimes::IsLeapYear(int yr) 
// IsLeapYear returns 1 if the yr is a leap year, 0 if it is not.
{
  return ((yr % 4 == 0 && yr % 100 != 0) || yr % 400 == 0);
}


double CSunTimes::CalcGamma(int julianDay)
//  Returns the gamma value that is used in the calculation for the
//  equation of time and the solar declination.
{
	return (2.0 * PI / 365.0) * (julianDay - 1.0);
}



double CSunTimes::CalcGamma2(int julianDay, double hour)
//	Returns the gamma value used to calculate eq of time
//	and solar declination.
{
  return (2.0 * PI / 365) * (julianDay - 1 + (hour / 24.0));
}


double CSunTimes::CalcEqofTime(double gamma)
//	Return the equation of time value for the given date.
{
  return (229.18 * (0.000075 + 0.001868 * cos(gamma) - 0.032077 * sin(gamma)
	  - 0.014615 * cos(2 * gamma) - 0.040849 * sin(2 * gamma)));
}


double CSunTimes::CalcSolarDec(double gamma)
//	Return the solar declination angle (in radians) for the given date.
{
  return (0.006918 - 0.399912 * cos(gamma) + 0.070257 * sin(gamma)
	  - 0.006758 * cos(2 * gamma) + 0.000907 * sin(2 * gamma));
}


double CSunTimes::CalcHourAngle(double lat, double solarDec, BOOL bSunRise)
//	The hour angle returned below is only for sunrise/sunset, i.e. when the solar
//	zenith angle is 90.8 degrees.
//  The reason why it's not 90 degrees is because we need to account for atmoshperic
//  refraction.
{
  double dLatRad;

  dLatRad = DegToRad(lat);

  if (bSunRise) 		//	if true, then calculationg for sunrise
	  return (acos(cos(DegToRad(90.833)) / (cos(dLatRad) * cos(solarDec))
      - tan(dLatRad) * tan(solarDec)));
  else
	  return -(acos(cos(DegToRad(90.833)) / (cos(dLatRad) * cos(solarDec))
      - tan(dLatRad) * tan(solarDec)));
}


double CSunTimes::DegToRad(double angleDeg) 
//	Convert degree angle to radians
{
  return (PI * angleDeg / 180);
}


double CSunTimes::RadToDeg(double angleRad) 
//	Convert radian angle to degrees
{
  return (180.0 * angleRad / PI);
}

