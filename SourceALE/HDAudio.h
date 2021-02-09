/////////////////////////////////////////////////////////////////////////////////////////////////
//
// HDAudio.h: Interface for the HDAudio class.
//
//            Designed and programmed at The Institute of Telecommunication Sciences
//
//            Ver 0.1 - R. Stafford & C. Behm 26 Mar 1999              
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDAUDIO_H__D85F9E8C_788E_11D2_90C8_00609710F7FB__INCLUDED_)
#define AFX_HDAUDIO_H__D85F9E8C_788E_11D2_90C8_00609710F7FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////////////////////
// Error defines for HDAudio
#define     INPUT_BUFFER_LOCK_FAILED      900
#define     OUTBUFFER_LOST                901
#define     OUTPUT_BUFFER_LOCK_FAILED     902

// Defines for HDAudio::InitNotify
#define     INPUTBUFFER       1
#define     OUTPUTBUFFER      2

// Number of Event Notifications
#define     NUMBEVENTS    3      

// Setup Globals for HDAudio::Demodule 
#define     SAMPLEFREQ        22050L
#define     BITSPERSAMPLE     16
#define     NUMBCHANNELS      1
#define     OUTPUTAMPLITUDE   5000
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
// NUMBGBLK is the number of Goertzel blocks in the total circular buffer
// It must be divisable by 5 to keep in time sync with the 8 ms ALE symbols
#define     NUMBGBLK          120 

// Define the number of 8 ms symbols in half of the DirectX circular buffer
#define     HALFBLKOFSYMBOLS  NUMBGBLK/4

// Define the Input symbols structures
// First define the half size Goertzel blocks
struct strSymbolL { // little blocks are 1/2 Goertzel length 
  short   iSymbol;
  double  dFSK[8];
  double  dSpecAve;
};
// The input symbols array will have an SNR attached to each symbol
struct strSymbolB { // big blocks are Goertzel length
  short   iSymbol;
  double  dSNR;
};

class HDAudio
{
// Attributes
private:
  DWORD     dwMyWriteCursor;

public:
  double    dNoiseThreshold;

public:
  // Constructor and Destructor
            HDAudio();
	virtual   ~HDAudio();

  // Member Functions
  HRESULT   InitDSound( CWnd *pWnd );
  HRESULT   InitNotify( short iInOrOut );
  HRESULT   ProcessOutputBuffer( DWORD iEvent, short iAmp );
  HRESULT   ProcessInputBuffer( DWORD iEvent );
  BOOL      InitializeBuffer( DWORD dwStart, DWORD cblength );
  HRESULT   Start( short iInOrOut );
  HRESULT   StopOutputBuffer(  );
  HRESULT   Stop( short iInOrOut );
	          Release( );
private:
  // Demodulate is used only in ProcessOutputBuffer
  BOOL      Demodulate( LPBYTE pbSource, DWORD cbLength );
  BOOL      Modulate( short iAmp );
};

#endif // !defined(AFX_HDAUDIO_H__D85F9E8C_788E_11D2_90C8_00609710F7FB__INCLUDED_)
