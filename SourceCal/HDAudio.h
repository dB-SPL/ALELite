// HDAudio.h: interface for the HDAudio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDAUDIO_H__D85F9E8C_788E_11D2_90C8_00609710F7FB__INCLUDED_)
#define AFX_HDAUDIO_H__D85F9E8C_788E_11D2_90C8_00609710F7FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Defines for HDAudio::InitNotify
#define     INPUTBUFFER       1
#define     OUTPUTBUFFER      2

// Number of Event Notifiations
#define     NUMBEVENTS    3      

// Setup Globals for HDAudio demod 
#define     SAMPLEFREQ        22050L
#define     BITSPERSAMPLE     16
#define     NUMBCHANNELS      1
                                        // Index
#define     FSK_1             750       //   0
#define     FSK_2             1000      //   1
#define     FSK_3             1500      //   2
#define     FSK_4             1250      //   3
#define     FSK_5             2500      //   4
#define     FSK_6             2250      //   5
#define     FSK_7             1750      //   6
#define     FSK_8             2000      //   7

#define     NUMBFSK           8

// Samples per 4 mS Block
#define     GOERTZBLK         (short)(0.004*SAMPLEFREQ)   
// NUMBGBLK * GOERTZBLK/22050 = second data at f_s = 22050
#define     NUMBGBLK          (short)(SAMPLEFREQ/GOERTZBLK) 

struct strReturnVal {
	double			dDC;
	double			dPwr;
};

class HDAudio
{
public:
  // Constructor and Distructor
    HDAudio();
	virtual   ~HDAudio();
  // Member Functions
  BOOL      InitDSound(CWnd *pWnd);
  BOOL      StopOutputBuffer(  );
  BOOL      InitNotify(  );
  BOOL      ProcessOutputBuffer( DWORD iEvent );
  BOOL      ProcessInputBuffer( DWORD iEvent );
  BOOL      InitializeBuffer( DWORD dwStart, DWORD cblength );
  BOOL      Start(  );
	        Stop(  );
	        Release( );
private:
  Error( HRESULT hr );
  // Demodulate is used only in ProcessOutputBuffer
  BOOL      Calibration( LPBYTE pbSource, DWORD cbLength );
};



#endif // !defined(AFX_HDAUDIO_H__D85F9E8C_788E_11D2_90C8_00609710F7FB__INCLUDED_)
