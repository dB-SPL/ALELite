//////////////////////////////////////////////////////////////////////////////////////////////////
//
// HDAudio.cpp: This class implements the operation of a half duplex ALE 
//              modem. 
//
//         Designed and programmed at The Institute of Telecommunication Sciences
//           Ver 0.1 - R. Stafford & C. Behm 10 Jan 2000              
//
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

// HDAudio system includes
#include <mmsystem.h>
#include <dsound.h>           // dsound.h requires mmsystem.h defined before dsound.h
#include <math.h>

// HDAudio local includes
#include "HDAudio.h"

#ifdef  _DEBUG
#undef   THIS_FILE
static  char  THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Define the DirectX service pointers
//////////////////////////////////////////////////////////////////////

//DSCBCAPS                            dscbc; 
HANDLE											      	rghEvent[NUMBEVENTS+1];
LPDIRECTSOUND										    lpds;
LPDIRECTSOUNDBUFFER									lpdsb;
LPDIRECTSOUNDCAPTURE								lpdsc;
LPDIRECTSOUNDCAPTUREBUFFER					lpdscb;
LPDIRECTSOUNDNOTIFY                 lpdsnInput, lpdsnOutput, lpDSNotify;
DSBUFFERDESC										    dsbDesc;
DSCBUFFERDESC										    dscbDesc;
DSBPOSITIONNOTIFY                   rgdsbpn[NUMBEVENTS];
WAVEFORMATEX										    wfx;

// These are defined in modem.cpp
struct strSymbolB                   SymbolIn[NUMBGBLK];
short                               iSymbolOut[NUMBGBLK/2];
short                               iOutputData[(NUMBGBLK * GOERTZBLK/2)+25];

// I only want to calculate this one time. This is for the demodulator
#define   TWOPIE                    8.0*atan(1.0)
double    cos_omega[8]      = {     2.0 * cos(TWOPIE*FSK_1/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_2/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_3/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_4/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_5/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_6/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_7/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_8/SAMPLEFREQ) };

// Calculate the initial conditions for the modulator
double    dY1[9]            = {     sin(TWOPIE*FSK_1/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_2/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_3/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_4/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_5/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_6/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_7/SAMPLEFREQ),
                                    sin(TWOPIE*FSK_8/SAMPLEFREQ),
                                    0.0 }; // This last one is for silence
double    dModCoeff[9]      = {     2.0 * cos(TWOPIE*FSK_1/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_2/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_3/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_4/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_5/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_6/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_7/SAMPLEFREQ),
                                    2.0 * cos(TWOPIE*FSK_8/SAMPLEFREQ),
                                    0.0 }; // This last one is for silence

// Storage for the intermediate results from the last GOERTZEL Block
double    dGVR1[NUMBFSK]    = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
double    dGVR2[NUMBFSK]    = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
 
short     iFirstBLK = FALSE;
short     iSignalPresent = FALSE;
short     iALEPresent = FALSE;
short     iTxEnd = FALSE;
 
//////////////////////////////////////////////////////////////////////
// Constructor and Distructor
//////////////////////////////////////////////////////////////////////

HDAudio::HDAudio()
{
  dwMyWriteCursor = 0xFFFFFFFF;
  dNoiseThreshold = 1.712307e12;  // Default value overwritten with value from CalVal.txt
}

HDAudio::~HDAudio()
{
}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

HRESULT HDAudio::InitDSound(CWnd *pWnd)
{
	int					        i = 0;
  HRESULT             hr = 0;

  // If the DirectSoundCaptureBuffer is set up first, before DirectSound then nSamples 
  // can be set to 44.1 KHz, 22.05KHz, 11.025 KHz or 8 KHz. Note that if you are running 
  // full duplex only 22.05K and 8000 Hz seem to work  

  ZeroMemory(&wfx, sizeof(wfx)); // write zeros to wfx structure
	//	Create initialize the WAVEFORMATEX structure
	wfx.cbSize = 0;
	wfx.nChannels =	NUMBCHANNELS;
	wfx.nSamplesPerSec = SAMPLEFREQ;
	wfx.wBitsPerSample = BITSPERSAMPLE;
 	wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8; // nChannels * wBitsPerSample / 8; 
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; // nSamplesPerSec * nBlockAlign; 
 	wfx.wFormatTag = WAVE_FORMAT_PCM;

	// Create a capture buffer
	dscbDesc.dwSize = sizeof(DSCBUFFERDESC);
	dscbDesc.dwFlags = DSCBCAPS_WAVEMAPPED;
	dscbDesc.dwBufferBytes = (DWORD)(GOERTZBLK * NUMBGBLK * wfx.nBlockAlign + 48);
	dscbDesc.dwReserved = 0;
	dscbDesc.lpwfxFormat = &wfx;

  // Create DirectSoundCapture.
  if (FAILED(hr = DirectSoundCaptureCreate(NULL, &lpdsc, NULL))) {
    return hr;
  }

  if (FAILED(hr = lpdsc->CreateCaptureBuffer(&dscbDesc, &lpdscb, NULL))) {
		return hr;
  }

  // Create DirectSound.
  if (FAILED(hr = DirectSoundCreate(NULL, &lpds, NULL))) {
		return hr;
  }

  // Set cooperative level.
  if (FAILED(hr = lpds->SetCooperativeLevel(pWnd->m_hWnd, DSSCL_PRIORITY))) {
		return hr;
  }

  // Now set up the output buffer
	// Create output buffer in same format as input buffer.
	ZeroMemory(&dsbDesc, sizeof(dsbDesc));
	dsbDesc.dwSize = sizeof(dsbDesc);

	// Set flags for whatever properties of the buffer you want to 
	// be able to change. We also set DSBCAPS_GLOBALFOCUS to keep
	// the buffer running when the app is in the background.
	dsbDesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS
											| DSBCAPS_GETCURRENTPOSITION2;

  dsbDesc.dwBufferBytes = dscbDesc.dwBufferBytes;
  dsbDesc.lpwfxFormat = &wfx;

  if (FAILED(hr = lpds->CreateSoundBuffer(&dsbDesc, &lpdsb, NULL))) {
		return hr;
  }

  return (HRESULT) DS_OK;

} // InitDSound 
//////////////////////////////////////////////////////////////////////////////////////////////

HRESULT HDAudio::InitNotify(short iInOrOut)
{
  short i;
  HRESULT             hr = 0;


  // Notifications - Setting up the Circular Buffer
  // Create the events. We make an extra one for later use
	for (i = 0; i <= NUMBEVENTS; i++) {
    rghEvent[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (NULL == rghEvent[i])
      return hr;
  }


  // Get the notification interface pointer to the input or output buffer
  if (iInOrOut == INPUTBUFFER) {

    // Set up notification positions n capture buffer. These occur halfway 
    // through the buffer, at the end of the buffer, and when capture stops.
    // The 1/2 way point in the buffer
    rgdsbpn[0].dwOffset = (dscbDesc.dwBufferBytes / 2) - 1;
    rgdsbpn[0].hEventNotify = rghEvent[0];
    // Trigger at the end
    rgdsbpn[1].dwOffset = dscbDesc.dwBufferBytes - 1;   
    rgdsbpn[1].hEventNotify = rghEvent[1];
    rgdsbpn[2].dwOffset = DSBPN_OFFSETSTOP; // If the application stops trigger event
    rgdsbpn[2].hEventNotify = rghEvent[2];

    // Get interface for capture buffer notification 
    if (lpdsnInput == NULL) {
      if (FAILED(hr = lpdscb->QueryInterface(IID_IDirectSoundNotify,
                            (VOID **) &lpdsnInput))) {
	      return hr;
      }
    }
    lpDSNotify = lpdsnInput;
  } else if (iInOrOut == OUTPUTBUFFER) {
    // Set up notification positions n capture buffer. These occur halfway 
    // through the buffer, at the end of the buffer, and when capture stops.
    // The 1/2 way point in the buffer
    rgdsbpn[0].dwOffset = (dsbDesc.dwBufferBytes / 2) - 1;  
    rgdsbpn[0].hEventNotify = rghEvent[0];

    // Trigger at the end
    rgdsbpn[1].dwOffset = dsbDesc.dwBufferBytes - 1;          
    rgdsbpn[1].hEventNotify = rghEvent[1];
    rgdsbpn[2].dwOffset = DSBPN_OFFSETSTOP; // If the application stops trigger event
    rgdsbpn[2].hEventNotify = rghEvent[2];
    
    if (lpdsnOutput == NULL) {
      if (FAILED(hr = lpdsb->QueryInterface(IID_IDirectSoundNotify,
                  (VOID **) &lpdsnOutput))) {
	      return hr;
      }
    }
    lpDSNotify = lpdsnOutput;
  }

  if (FAILED(hr = lpDSNotify->SetNotificationPositions(
                    NUMBEVENTS, rgdsbpn))) {  
    return hr;
  }

  return DS_OK;
} // InitNotify
///////////////////////////////////////////////////////////////////////////////////////////////


HRESULT HDAudio::Start(short iInOrOut)
{
  HRESULT         hr;

  // Stop output and clear notification just in case it's still
  // running from last time
  HDAudio::StopOutputBuffer();   

  // Clean up output buffer since it will run before getting new data.
  HDAudio::InitializeBuffer(0, dsbDesc.dwBufferBytes);

  // Start Output buffer
  switch (iInOrOut) {
    case OUTPUTBUFFER:
    int		j;
    LPBYTE	pbOutput1;
    LPBYTE	pbOutput2;
	  DWORD		cbOutput1;
	  DWORD		cbOutput2;

	  // Reset the write cursor at the beginning of each transmission
	  dwMyWriteCursor = 0xFFFFFFFF;

	  // Load the output data array with zeros
    for ( j = 0; j < (signed int) dsbDesc.dwBufferBytes / 4; j++)
			iOutputData[j] = 0;

	  if (SUCCEEDED(hr = lpdsb->Lock(0, dsbDesc.dwBufferBytes/2,
					(LPVOID *) &pbOutput1, &cbOutput1,
					(LPVOID *) &pbOutput2, &cbOutput2, 0))) {

			// Do the copy.
			CopyMemory(pbOutput1, (LPBYTE) &iOutputData, cbOutput1);
   
			if (cbOutput2) 
				memcpy(pbOutput2, (LPBYTE)&iOutputData + cbOutput1, cbOutput2);
		
			lpdsb->Unlock(pbOutput1, cbOutput1, pbOutput2, cbOutput2);  

			if (SUCCEEDED(hr = lpdsb->Lock(dsbDesc.dwBufferBytes/2, dsbDesc.dwBufferBytes/2,
				(LPVOID *) &pbOutput1, &cbOutput1,
				(LPVOID *) &pbOutput2, &cbOutput2, 0))) {

			  // Do the copy.
			  CopyMemory(pbOutput1, (LPBYTE) &iOutputData, cbOutput1);

			  if (cbOutput2) 
				  memcpy(pbOutput2, (LPBYTE)&iOutputData + cbOutput1, cbOutput2);
		  
			  lpdsb->Unlock(pbOutput1, cbOutput1, pbOutput2, cbOutput2);  
      } else {
				return hr;
			}
	  } else {
			return hr;
	  }

	  if (FAILED(hr = lpdsb->Play(0, 0, DSBPLAY_LOOPING))) {
      return hr;
    }
    break;

    // Start capture buffer
    case INPUTBUFFER:   
      if (FAILED(hr = lpdscb->Start(DSCBSTART_LOOPING))) {
        return hr;
      }
      break;
  }

  iFirstBLK = TRUE;

  return DS_OK;
} // Start
//////////////////////////////////////////////////////////////////////////////////////////////

HRESULT HDAudio::ProcessOutputBuffer(DWORD iEvent, short iAmp)
{
  HRESULT             hr;
  DWORD               dwNumBytes;
  DWORD               dwStatus;
  LPBYTE              pbOutput1;
  DWORD               cbOutput1;
  LPBYTE              pbOutput2;
  DWORD               cbOutput2;
       
  // Make sure we haven't lost the output buffer.
  hr = lpdsb->GetStatus(&dwStatus);
  if (dwStatus & DSBSTATUS_BUFFERLOST) {
    if (FAILED(hr = lpdsb->Restore())) {
      hr = OUTBUFFER_LOST;
      return hr;
    }
  }
   
  // If just starting, find the point at which it is safe to write
  // to the output buffer and initialize our internal cursor.
  if (dwMyWriteCursor == 0xFFFFFFFF) {
    hr = lpdsb->GetCurrentPosition(NULL, &dwMyWriteCursor);

    // We should get some distance between us and the write position 
    // to make sure it never overtakes us; it is necessarily going 
    // to stay a constant distance in front of the play position.
    dwMyWriteCursor += (dsbDesc.dwBufferBytes / 8);

    // Wrap around the dwMyWriteCursor if it is near the end of the buffer
    if (dwMyWriteCursor >= dsbDesc.dwBufferBytes)
      dwMyWriteCursor -= dsbDesc.dwBufferBytes;
  }
  
  // Now determine the size of the output buffer in bytes
  dwNumBytes = dsbDesc.dwBufferBytes / 2; 

  // DirectSoundBuffer::Lock() returns the addresses and sizes of the locked buffer in
  // pbOutput and cbOutput
  if (SUCCEEDED(hr = lpdsb->Lock(dwMyWriteCursor, dwNumBytes,
                (LPVOID *) &pbOutput1, &cbOutput1,
                (LPVOID *) &pbOutput2, &cbOutput2, 0))) {

    //********************************************************************************************
    // Covert the symbol array into one of 8 8 mS tones 
    HDAudio::Modulate(iAmp);
    //********************************************************************************************

    // Do the copy.
    CopyMemory(pbOutput1, (LPBYTE) &iOutputData, cbOutput1);
    
    if (cbOutput2) 
      memcpy(pbOutput2, (LPBYTE) &iOutputData + cbOutput1, cbOutput2);

    lpdsb->Unlock(pbOutput1, cbOutput1, pbOutput2, cbOutput2);  

    // Advance our internal write cursor, compensating for the wrap around
    dwMyWriteCursor += dwNumBytes;
    if (dwMyWriteCursor >= dsbDesc.dwBufferBytes)
      dwMyWriteCursor -= dsbDesc.dwBufferBytes;
  } else {
    hr = OUTPUT_BUFFER_LOCK_FAILED; 
    return hr;
  }

  return DS_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////
HRESULT HDAudio::ProcessInputBuffer(DWORD iEvent)
{

  HRESULT             hr;
  static DWORD        dwMyReadCursor = 0;
  DWORD               dwCBReadPos;
  DWORD               dwNumBytes;
  LPBYTE              pbInput1;
  DWORD               cbInput1;
  LPBYTE              pbInput2;
  DWORD               cbInput2;


  // Get the position of the read cursor when the event was set.
  // Remember, the index of the event is the index into the array
  // of notifications
  dwCBReadPos = rgdsbpn[iEvent].dwOffset;

  if (dwCBReadPos == DSBPN_OFFSETSTOP) {
    lpdscb->GetCurrentPosition(NULL, &dwCBReadPos);
    // The current position is one ahead of the last byte of data
    // available. It's important to sync up for 16-bit data.
    dwCBReadPos--;
    if (dwCBReadPos < 0)
      dwCBReadPos += dsbDesc.dwBufferBytes;
  }

  // Calculate the number of bytes to read.
  // We don't have to worry about wraparound of dwNumBytes, since we know we did
  // a read when the cursor last hit the end of the buffer.
  // + 49 is actually = 1 (to compensate for the byte count starting at zero)
  // + 48 (to compensate for the loss of a sample every 3rd & 5th input symbol)
  dwNumBytes = (dwCBReadPos - dwMyReadCursor) + 1;
  if (dwNumBytes == 0) 
    dwNumBytes = dsbDesc.dwBufferBytes / 2;

  // Lock the part of the capture buffer that has the data we want.
  // Note the cbInput2 will always be zero, since our reads never
  // wrap around.
  if (SUCCEEDED(hr = lpdscb->Lock(dwMyReadCursor, dwNumBytes,
                (LPVOID *) &pbInput1, &cbInput1,
                (LPVOID *) &pbInput2, &cbInput2, 0))) {
    //***************************************************************
    // Do the demodulation & symbol evaluation 
    HDAudio::Demodulate(pbInput1, cbInput1);
    //***************************************************************

    // Finished with capture buffer.
    lpdscb->Unlock(pbInput1, cbInput1, pbInput2, cbInput2);

    // Advance our internal read cursor by the number of bytes in
    // the capture lock, compensating for the wraparound.
    dwMyReadCursor += dwNumBytes;
    if (dwMyReadCursor >= dscbDesc.dwBufferBytes)
      dwMyReadCursor -= dscbDesc.dwBufferBytes;
  } else {
    hr = INPUT_BUFFER_LOCK_FAILED;
    return hr;
  }

  return DS_OK;

} // ProcessInputBuffer
/////////////////////////////////////////////////////////////////////

HRESULT HDAudio::StopOutputBuffer()
{ 
  lpdsb->Stop();

  return DS_OK;
} // StopOutputBuffer

//////////////////////////////////////////////////////////////////////////////////////////////

HRESULT HDAudio::Stop(short iInOrOut)
{
  HRESULT hr;

  if (iInOrOut == INPUTBUFFER) {
    hr = lpdscb->Stop(); // Stop the Capture Buffer
    ASSERT(hr != DSERR_NODRIVER);   
  	ASSERT(hr != DSERR_OUTOFMEMORY);   
  	ASSERT(hr == DS_OK);
  } else if (iInOrOut == OUTPUTBUFFER) {
    hr = lpdsb->Stop();  // Stop Output Buffer
    ASSERT(hr != DSERR_INVALIDPARAM);
  	ASSERT(hr != DSERR_PRIOLEVELNEEDED);
  	ASSERT(hr == DS_OK);
  }

  // Set the Notifications to NULL to turn them off
  if (lpDSNotify != NULL) {
    if (FAILED(hr = lpDSNotify->SetNotificationPositions(0, NULL))) {
      return hr;
    }
  }  

  return DS_OK;
} // Stop
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::InitializeBuffer(DWORD dwStart, DWORD cbLength)
{
  // Empty the output buffer

  PBYTE     pb1, pb2;
  DWORD     cb1, cb2;

  if ((!dwStart && !cbLength) || (NULL == lpdsb)
        || (NULL == dsbDesc.lpwfxFormat))
    return FALSE;

  if (SUCCEEDED(lpdsb->Lock(dwStart, cbLength, (LPVOID *) &pb1, &cb1, 
                              (LPVOID *) &pb2, &cb2, 0))) {
    FillMemory(pb1, cb1, (dsbDesc.lpwfxFormat->wBitsPerSample == 8) ? 128 : 0);
    
    if (NULL != pb2 && cb2)
      FillMemory(pb2, cb2, (dsbDesc.lpwfxFormat->wBitsPerSample == 8) ? 128 : 0);

    lpdsb->Unlock(pb1, cb1, pb2, cb2);
    return TRUE;
  }

  return FALSE;

} // InitializeBuffer
//////////////////////////////////////////////////////////////////////////////////////////////

HDAudio::Release()
{
  if (lpdsnOutput)
    lpdsnOutput->Release();

  if (lpds)
    lpds->Release(); // Release all the buffers

  if (lpdsnInput) 
      IDirectSoundNotify_Release(lpdsnInput);    
  if (lpdscb) 
      IDirectSoundCaptureBuffer_Release(lpdscb);
  if (lpdsc) 
      IDirectSoundCapture_Release(lpdsc);
} // Release
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::Demodulate(LPBYTE pbSource, DWORD cbLength)
{
  short                   j, k, n, m, t; 
  short                   *piData[3], iBiggest;
  short                   iAdjust[2] = {4, 0};
  short                   iTemp[NUMBFSK];
  short                   iNumbTriples;
  static short            iSkip = 0, iCount = 0;
  static short            iVeryLastSymbol = FALSE;
  static short            iForcedSymbol = FALSE;
  static strSymbolB       FedForwardSymbol;
  static strSymbolL       LastSymbol[3] = { {0 , {.0,.0,.0,.0,.0,.0,.0,.0}, 0.0},
                                            {0 , {.0,.0,.0,.0,.0,.0,.0,.0}, 0.0},
                                            {0 , {.0,.0,.0,.0,.0,.0,.0,.0}, 0.0} };
  double                  vr1[2][NUMBFSK], vr2[2][NUMBFSK];
  double                  dTemp[2], dBiggest;
  double                  dSNRAve, dTotalPower;
  struct strSymbolL       SymbolInL[NUMBGBLK+2];

  //FILE                    *fp;

  // Initialize any of the static vars that must be at the begining of a group of blocks
  if (iFirstBLK == TRUE) {
    iFirstBLK = FALSE;
    iVeryLastSymbol = FALSE;
    iForcedSymbol = FALSE;
    iTxEnd = FALSE;
    iALEPresent = FALSE;
    iCount = 0;
    iSkip = 0;
    for (n=0; n<NUMBFSK; n++) {
      dGVR1[n] = 0.0;
      dGVR2[n] = 0.0;
    }
  }

  // Load a local pointer
  piData[2] = piData[0] = piData[1] = (short *)pbSource; 
  piData[1]++;

  dTotalPower = 0;

  // n = FSK frequency index 
  // m = Overlap stream index
  // t = Which stream is done next index
  // j = Block index
  // i = Counter to determine if you have three Goertz blocks in a row of 
  // k = Current freqency number

  // Initialize the arrays
  for (n=0; n<NUMBFSK; n++) {
    // Load the array that you have already calculated 1/2 GOERTZBLK for 
    vr1[0][n] = dGVR1[n];
    vr2[0][n] = dGVR2[n];
    // Load the array that is aligned with this block
    vr1[1][n] = 0.0; 
    vr2[1][n] = 0.0;
  }

  ///////////////////////////////////////////////////////////////////////////
  // Start the first pass. This pass determines the magnitude of the 
  // frequency in all 8 bins for the 2 overlapping 4 mS 
  // Goertzel blocks. The var t keeps track of the which overlap tier is 
  // next to complete
  ///////////////////////////////////////////////////////////////////////////
  // Set t to 0 because the 0th overlap stream will complete first
  t = 0;

  // Determine the symbols 
  for (j=0; j<NUMBGBLK; j++) {
    // There are going to twice the number of outputs to the number of G blocks
    // Every half Goertzel block one or the other of the overlapped blocks is done
    for (k=0; k<GOERTZBLK/2; k++) {
      // There are two overlaping Goertzel calculations
      for (m=0; m<2; m++) {
        for (n=0; n<NUMBFSK; n++) {
           dTemp[m] = vr1[m][n];
           vr1[m][n] = cos_omega[n]*dTemp[m]-vr2[m][n] + (double)*piData[m];
           vr2[m][n] = dTemp[m];
        } // n
        piData[m]++;
      }  // m   
    } // k

    // Because the sample rate is not in time sync with the 8 ms symbols we will have to
    // skip a sample every 5th Goertzel block to stay in symbol sync.
    iAdjust[t]++;
    if ((iAdjust[t] == 5) && (j!= NUMBGBLK - 1)) {
      // Reset adjustment and swallow a sample
      iAdjust[t] = 0;
      piData[t]++;
    }

    // find the magnitude and the Da Biggest one
    dBiggest = 0.0;
    iBiggest = 0;

    // Initialize the spectral average
    SymbolInL[j].dSpecAve = 0.0;

    for (n=0; n<NUMBFSK; n++) {

      // Store the magnitude of the 8 frequency bins for this block in the "little" symbol structure
      SymbolInL[j].dFSK[n] = 
                  vr1[t][n]*vr1[t][n]+vr2[t][n]*vr2[t][n]-cos_omega[n]*vr1[t][n]*vr2[t][n];
      SymbolInL[j].dSpecAve += SymbolInL[j].dFSK[n];
      
      if (SymbolInL[j].dFSK[n] > dBiggest) {
        dBiggest = SymbolInL[j].dFSK[n]; 
        iBiggest = n;
      }

      // Start the stream over again by clearing the first input
      vr1[t][n] = 0.0; 
      vr2[t][n] = 0.0;
    }
    
    // Store the frequency choice for this block
    SymbolInL[j].iSymbol = iBiggest;

    // Now fix up the spectral average by dividing by 8
    dTotalPower += SymbolInL[j].dSpecAve;
    SymbolInL[j].dSpecAve /= 8.0;

    // Toggle the block indicator for the next overlap stream to finish
    t = 1 - t;    
  } // j

  // At this point you have exited the loop above with only half of the last block finished 
  // Load the array so that the next time you enter this routine you have the initializations
  // for the last half of the overlap
  for (n=0; n<NUMBFSK; n++) {
    dGVR1[n] = vr1[t][n]; 
    dGVR2[n] = vr2[t][n];    
    
    // Clear the counter array
    iTemp[n] = 0;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Begin the second pass of the 2 mS output array.
  // This pass determines which of the 8 frequency symbols will represent each
  // the 8 mS of input data
  /////////////////////////////////////////////////////////////////////////////  
  
  iNumbTriples = 0;
  dSNRAve = 0;

  // k will be the count of the 8 mS symbols
  k = 0;

  //Check to see if there was a fed forward symbol
  if (FedForwardSymbol.iSymbol != 9) {
    SymbolIn[k++] = FedForwardSymbol;
    FedForwardSymbol.iSymbol = 9;
  }

  // If the last block left you with a symbol check to see if the first symbol of this block
  // is also a symbol then skip symbols as appropiate
  if (iVeryLastSymbol == TRUE) {
    if ((LastSymbol[0].iSymbol == LastSymbol[1].iSymbol) 
                               && 
        (LastSymbol[1].iSymbol == LastSymbol[2].iSymbol)) {
      iSkip = 3;
      iNumbTriples++;
    } else {
	    if ((SymbolInL[0].iSymbol == SymbolInL[1].iSymbol) 
		                    && 
	        (SymbolInL[1].iSymbol == SymbolInL[2].iSymbol)
			  			          &&
		      (LastSymbol[1].iSymbol == SymbolInL[0].iSymbol)) {
			  iSkip = 3;
	    } else {
			  iSkip = 2;
	    }
    }
    // Either way reset the count
    iCount = 1;
    iVeryLastSymbol = FALSE;
  }

  // If you came up short in the prev block and thus forced a symbol
  // skip the first symbol that would have been forced
  if (iForcedSymbol == TRUE) {
    iForcedSymbol = FALSE;
    iSkip = 3;
  }

  if (iSkip == 1) {
    LastSymbol[2] = LastSymbol[1];
    LastSymbol[1] = SymbolInL[0];
  } else if (iSkip == 2) {
    LastSymbol[2] = SymbolInL[0];
    LastSymbol[1] = SymbolInL[1];
  } else if (iSkip == 3) {
    LastSymbol[2] = SymbolInL[1];
    LastSymbol[1] = SymbolInL[2];
  }

  /////////////////////////////////////////////////////////////////////////////////////
  // Begin the main symbol hunting routine.
  /////////////////////////////////////////////////////////////////////////////////////
  
  for (j = iSkip; j < NUMBGBLK-1; j++) {
    
    LastSymbol[0] = SymbolInL[j];
    
    // Check for a triple symbol
    if ((LastSymbol[0].iSymbol == LastSymbol[1].iSymbol) 
                               && 
        (LastSymbol[1].iSymbol == LastSymbol[2].iSymbol)) {
      SymbolIn[k].iSymbol = LastSymbol[0].iSymbol;
      SymbolIn[k].dSNR = (LastSymbol[0].dFSK[LastSymbol[0].iSymbol]+
                             LastSymbol[1].dFSK[LastSymbol[1].iSymbol]+
                             LastSymbol[2].dFSK[LastSymbol[2].iSymbol])
                                          /
        (LastSymbol[0].dSpecAve + LastSymbol[1].dSpecAve + LastSymbol[2].dSpecAve);
      SymbolIn[k].dSNR = ((SymbolIn[k].dSNR - 1) * 4.1428) + 1;
      dSNRAve += SymbolIn[k].dSNR;
      k++;
      iNumbTriples++;
      // Check to see if the next symbol is also triplicated
      if ((LastSymbol[0].iSymbol == SymbolInL[j+1].iSymbol)) { 
        // Two triples contiguously means that you can jump 2 past the second triple
        j += 3;
      } else {
        // There were not two triples in a row it is only safe to move two
        j += 2;
      }
      // Either way reset the count 2  
      iCount = 2;

      LastSymbol[2] = SymbolInL[j-1];
      LastSymbol[1] = SymbolInL[j];
    } else if (iCount == 3) {
      // Force a symbol you have looked long enough
      SymbolIn[k].iSymbol = LastSymbol[0].iSymbol;

      // Check to see if the next symbol is triplicated to see if you can resync
      if ((LastSymbol[0].iSymbol == SymbolInL[j + 1].iSymbol)
                                 &&
          (LastSymbol[1].iSymbol == LastSymbol[0].iSymbol)) {
        // A force and a triple is as good as contiguous triples
        // Treat this case like contiguous triples above
        SymbolIn[k].dSNR = (LastSymbol[0].dFSK[LastSymbol[0].iSymbol]+
                             LastSymbol[1].dFSK[LastSymbol[1].iSymbol]+
                             SymbolInL[j + 1].dFSK[SymbolInL[j + 1].iSymbol])
                                            /
          (LastSymbol[0].dSpecAve + LastSymbol[1].dSpecAve + SymbolInL[j + 1].dSpecAve);
        SymbolIn[k].dSNR = ((SymbolIn[k].dSNR - 1) * 4.1428) + 1;
        dSNRAve += SymbolIn[k].dSNR;
        // Increment the symbol counter
        k++;
        // Skip 3 - 2 plus one for the triple
        j += 3;
        iNumbTriples++;
      } else {
        // There were not two triples in a row, it is only safe to move two
        j += 2;
        // Set the SNR to something arbitrarily low
        SymbolIn[k++].dSNR = 0.1;
      }

      // Either way reset the count to 2
      iCount = 2;
      
      LastSymbol[2] = SymbolInL[j-1];
      LastSymbol[1] = SymbolInL[j];
    } else { 
      iCount++;
      LastSymbol[2] = LastSymbol[1];
      LastSymbol[1] = LastSymbol[0];
    }    
  }
  
  ////////////////////////////////////////////////////////////////////////////////////
  // Now determine the end conditions if/when the next input block arrives
  ////////////////////////////////////////////////////////////////////////////////////
  iSkip = 0;
  if (j > NUMBGBLK)
    iSkip = j - (NUMBGBLK);

  if (k > 30) {
    FedForwardSymbol = SymbolIn[k];
  } else if ((j == NUMBGBLK - 1) || ((j >= NUMBGBLK) && (k == (NUMBGBLK/4) - 1))) {
    LastSymbol[0] = SymbolInL[NUMBGBLK-1];    
    if ((LastSymbol[0].iSymbol == LastSymbol[1].iSymbol) 
                               && 
        (LastSymbol[1].iSymbol == LastSymbol[2].iSymbol)) {
      SymbolIn[k].iSymbol = LastSymbol[0].iSymbol;
      SymbolIn[k].dSNR = (LastSymbol[0].dFSK[LastSymbol[0].iSymbol]+
                             LastSymbol[1].dFSK[LastSymbol[1].iSymbol]+
                             LastSymbol[2].dFSK[LastSymbol[2].iSymbol])
                                          /
      (LastSymbol[0].dSpecAve + LastSymbol[1].dSpecAve + LastSymbol[2].dSpecAve);
      SymbolIn[k].dSNR = ((SymbolIn[k].dSNR - 1) * 4.1428) + 1;
      dSNRAve += SymbolIn[k].dSNR;
      if (k == 30)
        FedForwardSymbol = SymbolIn[k];
      k++;
      iVeryLastSymbol = TRUE;
      iNumbTriples++;
    } else if ((iCount == 3) || (k == (NUMBGBLK / 4) - 1)) {
      // Belch out a symbol
      SymbolIn[k].iSymbol = LastSymbol[0].iSymbol;
      SymbolIn[k].dSNR = 0.2;
      // You have forced a symbol skip ahead by two
      iCount = 2;
      // By skipping two you will next look at the 3rd data point away from the 
      // present point
      iSkip = 2;
      if (k == 30)
        FedForwardSymbol = SymbolIn[k];
      k++;
    } else 
      iCount++;
  }

  // Fix up the end condition where the end of the symbol sequence is erred or the
  // the n*4-1-n*4 case that the hunter was not built for by forcing a symbol on this 
  // side of the block boundry
  if ((k == ((NUMBGBLK / 4) - 1)) && (iCount == 3)) {
    SymbolIn[k].iSymbol = LastSymbol[0].iSymbol;
    SymbolIn[k++].dSNR = 0.3;
    iForcedSymbol = TRUE;
  }

  // Place the average SNR in the last symbol
  SymbolIn[NUMBGBLK / 4 - 1].dSNR = dSNRAve / 30.0;

  // Store the last two symbols, if necessary
  LastSymbol[2] = SymbolInL[NUMBGBLK-2];
  LastSymbol[1] = SymbolInL[NUMBGBLK-1];

  // Determine the block indicators
  iSignalPresent = FALSE;

  // Is there any signal present
  if (dTotalPower >= dNoiseThreshold)
    iSignalPresent = TRUE;

  // Are at least 25 out of 30 symbols valid ALE symbols 
  if (iNumbTriples >= 25)
    iALEPresent = TRUE;
  else
    iALEPresent = FALSE;

  // If the power at the end is 1/10th or less the power at the beginning 
  // then the transmission must have ended
  if (iTxEnd == FALSE) {
    if ((SymbolInL[1].dSpecAve / SymbolInL[NUMBGBLK - 2].dSpecAve >= 10)
                                ||
        ((iALEPresent == TRUE) && (iSignalPresent == FALSE)))
      iTxEnd = TRUE;
  }
       
  return TRUE;
} // Demodulate
/////////////////////////////////////////////////////////////////////////////////////////////////


BOOL HDAudio::Modulate(short iAmp)
{
  short       iAdjust, iWhen, j, k;
  short       iFSK;
  double      dY[3];
  short       *iOut;

  iAdjust = 0;
  iWhen = 2;
  iOut = &iOutputData[0];

  // Determine the symbols 
  for (j=0; j<NUMBGBLK/4; j++) {
    // Load the FSK array index
    iFSK = iSymbolOut[j];
    
    // Initial conditions
    dY[2] = 0.0;
    dY[1] = (double)iAmp * dY1[iFSK];

    // Every Goertzel block output a frequency
    for (k=0; k<GOERTZBLK*2; k++) {                 
        dY[0] = dModCoeff[iFSK]*dY[1] - dY[2];
        *iOut++ = (short)dY[0];
        dY[2] = dY[1];
        dY[1] = dY[0];              
    }

    // Every 5th Symbol two samples are needed to keep in 8 mS sync
    // One samples is added in the following way, the second of five symbols gets 
    // an extra sample then the 5th of five symbols then the count is reset and
    // is done again every 5th sample
    if (iAdjust == 1 || iAdjust == 4) {
        dY[0] = dModCoeff[iFSK]*dY[1] - dY[2];
        *iOut++ = (short)dY[0];
        dY[2] = dY[1];
        dY[1] = dY[0]; 
    }
    
    // If this is the 5th symbol reset the count
    iAdjust == 4 ? iAdjust = 0 : ++iAdjust;
  }  

  return TRUE;
}
