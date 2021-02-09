//////////////////////////////////////////////////////////////////////
//
// HDAudio.cpp: implementation of the HDAudio class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include <mmsystem.h>
#include <dsound.h>  
#include <math.h>
#include "hdaudio.h"

#ifdef  _DEBUG
#undef   THIS_FILE
static  char  THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Define the DirectX service pointers
//////////////////////////////////////////////////////////////////////

DSCBCAPS                            dscbc; 

HANDLE								rghEvent[NUMBEVENTS+1];
LPDIRECTSOUND						lpds;
LPDIRECTSOUNDBUFFER					lpdsb;
LPDIRECTSOUNDCAPTURE				lpdsc;
LPDIRECTSOUNDCAPTUREBUFFER			lpdscb;
LPDIRECTSOUNDNOTIFY                 lpdsnInput, lpdsnOutput, lpDSNotify, lpdsn;
DSBUFFERDESC						dsbDesc;
DSCBUFFERDESC						dscbDesc;
DSBPOSITIONNOTIFY                   rgdsbpn[NUMBEVENTS];
WAVEFORMATEX						wfx;

int    iInOrOut;
struct strReturnVal ReturnVal;

short            iOutputData[NUMBGBLK*SAMPLEFREQ];
FILE			 *fpDataOut;

HDAudio::HDAudio()
{

}

HDAudio::~HDAudio()
{

}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

BOOL HDAudio::InitDSound(CWnd *pWnd)
{

  int				i = 0;
  HRESULT           hr = 0;

  // If the DirectSoundCaptureBuffer is set up first, before DirectSound then nSamples 
  // can be set to 44.1 KHz, 22.05KHz, 11.025 KHz or 8 KHz. Note that if youa re running 
  // full duplex only 22.05K and 8000 Hz seem to work  

    ZeroMemory( &wfx, sizeof( wfx ) ); // write zeros to wfx structure
		//	Create initailize the WAVEFORMATEX structure
		wfx.cbSize = 0;
		wfx.nChannels =	NUMBCHANNELS;
		wfx.nSamplesPerSec = SAMPLEFREQ;
		wfx.wBitsPerSample = BITSPERSAMPLE;
 		wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8; // nChannels * wBitsPerSample / 8; 
		wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign; // nSamplesPerSec * nBlockAlign; 
 		wfx.wFormatTag = WAVE_FORMAT_PCM;

		// Create a capture buffer
		dscbDesc.dwSize = sizeof( DSCBUFFERDESC );
		dscbDesc.dwFlags = DSCBCAPS_WAVEMAPPED;
		dscbDesc.dwBufferBytes = (DWORD)(GOERTZBLK * NUMBGBLK * wfx.nBlockAlign);
		dscbDesc.dwReserved = 0;
		dscbDesc.lpwfxFormat = &wfx;

    // Create DirectSoundCapture.
    if ( FAILED( hr = DirectSoundCaptureCreate( NULL, &lpdsc, NULL ) ) )
		  {
			HDAudio::Error(hr);
      return FALSE;
		  }
    
    if ( FAILED( hr = lpdsc->CreateCaptureBuffer( &dscbDesc, &lpdscb, NULL ) ) )
      {
			HDAudio::Error(hr);
			return FALSE;
		  }
 
    // Create DirectSound.
    if ( FAILED( hr = DirectSoundCreate( NULL, &lpds, NULL ) ) )
    {
			HDAudio::Error(hr);
      return FALSE;
    }

    // Set cooperative level.
    if ( FAILED( hr = lpds->SetCooperativeLevel( pWnd->m_hWnd, DSSCL_PRIORITY  ) ) )
		{
			HDAudio::Error(hr);
			return FALSE;
		}
   
    // Now set up the output buffer
		// Create output buffer in same format as input buffer.
		ZeroMemory( &dsbDesc, sizeof( dsbDesc ) );
		dsbDesc.dwSize = sizeof( dsbDesc );

		// Set flags for whatever properties of the buffer you want to 
		// be able to change. We also set DSBCAPS_GLOBALFOCUS to keep
		// the buffer running when the app is in the background.
		dsbDesc.dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GLOBALFOCUS
												| DSBCAPS_GETCURRENTPOSITION2; 


		 // The output buffer should be bigger than the capture buffer to
		 // avoid any overlap problems.
		 dsbDesc.dwBufferBytes = dscbDesc.dwBufferBytes;
		 dsbDesc.lpwfxFormat = &wfx;

		 if ( FAILED( hr = lpds->CreateSoundBuffer( &dsbDesc, &lpdsb, NULL ) ) )
			{
				HDAudio::Error(hr);
				return FALSE;
			}

		return TRUE;

} // InitDSound 
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::InitNotify(  )
{
  short i;
  HRESULT             hr = 0;


  // Notifications - Setting up the Circular Buffer
  // Create the events. We make an extra one for later use
	for ( i = 0; i <= NUMBEVENTS; i++)
    {
      rghEvent[i] = CreateEvent( NULL, FALSE, FALSE, NULL );
      if ( NULL == rghEvent[i] ) return FALSE;
    }


  // Get the notification interface pointer to the input or output buffer
  if( iInOrOut == INPUTBUFFER )
    {

    // Set up notification positions n capture buffer. These occur halfway 
    // through the buffer, at the end of the buffer, and when capture stops.
    // The 1/2 way point in the buffer
    rgdsbpn[0].dwOffset = ( dscbDesc.dwBufferBytes / 2) - 1;  
    rgdsbpn[0].hEventNotify = rghEvent[0];
    // Trigger at the end
    rgdsbpn[1].dwOffset = dscbDesc.dwBufferBytes - 1;          
    rgdsbpn[1].hEventNotify = rghEvent[1];
    rgdsbpn[2].dwOffset = DSBPN_OFFSETSTOP; // If the application stops trigger event
    rgdsbpn[2].hEventNotify = rghEvent[2];

    // Get interface for capture buffer notification 
    if( lpdsnInput == NULL )
      {
      if ( FAILED( hr = lpdscb->QueryInterface( IID_IDirectSoundNotify,
                            ( VOID **)&lpdsnInput ) ) ) 
		    {
	      HDAudio::Error(hr);
        return FALSE;
	      }
      }
    lpDSNotify = lpdsnInput;
    }
  else if( iInOrOut == OUTPUTBUFFER )
    {
	  
    // Set up notification positions n capture buffer. These occur halfway 
    // through the buffer, at the end of the buffer, and when capture stops.
    // The 1/2 way point in the buffer
    rgdsbpn[0].dwOffset = ( dsbDesc.dwBufferBytes / 2 ) - 1;  
    rgdsbpn[0].hEventNotify = rghEvent[0];
    // Trigger at the end
    rgdsbpn[1].dwOffset = dsbDesc.dwBufferBytes - 1;          
    rgdsbpn[1].hEventNotify = rghEvent[1];
    rgdsbpn[2].dwOffset = DSBPN_OFFSETSTOP; // If the application stops trigger event
    rgdsbpn[2].hEventNotify = rghEvent[2];
    
    if( lpdsnOutput == NULL )
      {
      if ( FAILED( hr = lpdsb->QueryInterface( IID_IDirectSoundNotify,
                            ( VOID **)&lpdsnOutput ) ) ) 
		    {
	      HDAudio::Error(hr);
        return FALSE;
	      }
      }
    lpDSNotify = lpdsnOutput;
    }

   if ( FAILED(hr = lpDSNotify->SetNotificationPositions(
                    NUMBEVENTS, rgdsbpn ) ) )
      {  
      HDAudio::Error(hr); 
      lpDSNotify->Release();
      return FALSE;
      }

  return TRUE;

} // InitNotify
///////////////////////////////////////////////////////////////////////////////////////////////

HDAudio::Error( HRESULT hr )
{
	char szMsg[256]="";

	switch (hr)
		{		
		case DS_OK: // The request completed successfully.
			sprintf(szMsg, "DS_OK");
			break;
		case DSERR_ALLOCATED: // The request failed because resources, such as a priority level, were already in use by another caller.  
			sprintf(szMsg, "DSERR_ALLOCATED");
			break;
		case DSERR_ALREADYINITIALIZED: // The object is already initialized.  
			sprintf(szMsg, "DSERR_ALREADYINITIALIZE");
			break;
		case DSERR_BADFORMAT: // The specified wave format is not supported.  
			sprintf(szMsg, "DSERR_BADFORMAT");
			break;
		case DSERR_BUFFERLOST:  // The buffer memory has been lost and must be restored.  
			sprintf(szMsg, "DSERR_BUFFERLOST");
			break;
		case DSERR_CONTROLUNAVAIL:  // The control (volume, pan, and so forth) requested by the caller is not available.  
			sprintf(szMsg, "DSERR_CONTROLUNAVAIL");
			break;
		case DSERR_GENERIC:  // An undetermined error occurred inside the DirectSound subsystem.  
			sprintf(szMsg, "DSERR_GENERIC");
			break;
		case DSERR_INVALIDCALL:  // This function is not valid for the current state of this object.  
			sprintf(szMsg, "DSERR_INVALIDCALL");
			break;
		case DSERR_INVALIDPARAM:  // An invalid parameter was passed to the returning function.  
			sprintf(szMsg, "DSERR_INVALIDPARAM");
			break;
		case DSERR_NOAGGREGATION:  // The object does not support aggregation.  
			sprintf(szMsg, "DSERR_NOAGGREGATION");
			break;
		case DSERR_NODRIVER:  // No sound driver is available for use.  
			sprintf(szMsg, "DSERR_NODRIVER");
			break;
		case DSERR_NOINTERFACE:  // The requested COM interface is not available.  
			sprintf(szMsg, "DSERR_NOINTERFACE");
			break;
		case DSERR_OTHERAPPHASPRIO:  // Another application has a higher priority level, preventing this call from succeeding  
			sprintf(szMsg, "DSERR_OTHERAPPHASPRIO");
			break;
		case DSERR_OUTOFMEMORY:  // The DirectSound subsystem could not allocate sufficient memory to complete the caller's request.  
			sprintf(szMsg, "DSERR_OUTOFMEMORY");
			break;
		case DSERR_PRIOLEVELNEEDED:  // The caller does not have the priority level required for the function to succeed.  
			sprintf(szMsg, "DSERR_PRIOLEVELNEEDED");
			break;
		case DSERR_UNINITIALIZED:  // The IDirectSound::Initialize method has not been called or has not been called successfully before other methods were called.  
			sprintf(szMsg, "DSERR_UNINITIALIZED");
			break;
		case DSERR_UNSUPPORTED:  // The function called is not supported at this time.  
			sprintf(szMsg, "DSERR_UNSUPPORTED");
			break;
		default:
			sprintf(szMsg, " I have no idea.");
			break;
		}

	MessageBox(NULL, (LPCTSTR)szMsg, "HDAudio Error", MB_OK | MB_ICONEXCLAMATION );
 

} // Error
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::Start( )
{
  HRESULT         hr;

  // Stop output and clear notification just in case it's still
  // running from last time
  HDAudio::StopOutputBuffer(  );   

  // Clean up output buffer since it will run before getting new data.
  HDAudio::InitializeBuffer( 0, dsbDesc.dwBufferBytes );

  // Start Output buffer
  switch(iInOrOut)
    {
    case OUTPUTBUFFER:
      if ( FAILED( hr = lpdsb->Play( 0, 0, DSBPLAY_LOOPING ) ) )
        {
        HDAudio::Error(hr); 
        return FALSE;
        }
    break;
    // Start capture buffer
    case INPUTBUFFER:   
      if ( FAILED( hr = lpdscb->Start( DSCBSTART_LOOPING ) ) )
        {
        HDAudio::Error(hr); 
        return FALSE;
        }
    break;
    }

  return TRUE;

} // Start
//////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////
BOOL HDAudio::ProcessInputBuffer( DWORD iEvent )
{

  HRESULT             hr;
  static DWORD        dwMyReadCursor;
  static DWORD        dwMyWriteCursor = 0xFFFFFFFF;
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

  if ( dwCBReadPos == DSBPN_OFFSETSTOP )
    {
    lpdscb->GetCurrentPosition( NULL, &dwCBReadPos );
    // The current position is one ahead of the last byte of data
    // available. It's important to sync up for 16-bit data.
    dwCBReadPos--;
    if ( dwCBReadPos < 0 )
      dwCBReadPos += dsbDesc.dwBufferBytes;
    }

  // Calculate the number of bytes to read.
  // We don't have to worry about wraparound, since we know we did
  // a read when the cursor last hit the end if the buffer.
  dwNumBytes = ( dwCBReadPos - dwMyReadCursor ) + 1;
  if (dwNumBytes == 0) 
    dwNumBytes = dsbDesc.dwBufferBytes / 2;

  // Lock the part of the capture buffer that has the data we want.
  // Note the cbInput2 will always be zero, since our reads never
  // wrap around.
  if ( FAILED( hr = lpdscb->Lock( dwMyReadCursor, dwNumBytes,
                ( LPVOID * )&pbInput1, &cbInput1,
                ( LPVOID * )&pbInput2, &cbInput2, 0 ) ) ) 
    OutputDebugString( "Capture lock string\n" );
  else {
  //***************************************************************************
  // Do the Calibration 
  HDAudio::Calibration(pbInput1, cbInput1);
  //***************************************************************************

  // Finished with capture buffer.
  lpdscb->Unlock( pbInput1, cbInput1, pbInput2, cbInput2 );

  // Advance our internal read cursor by the number of bytes in 
  // the capture lock, compensating for the wraparound.
  dwMyReadCursor += dwNumBytes;
  if ( dwMyReadCursor >= dscbDesc.dwBufferBytes )
    dwMyReadCursor -= dscbDesc.dwBufferBytes;
  }

  return TRUE;

} // ProcessInputBuffer
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::StopOutputBuffer(  )
{
  HRESULT       hr;
  
  lpdsb->Stop();

  if ( lpdsn != NULL )
    {
      if ( FAILED( hr = lpdsn->SetNotificationPositions( 0, NULL ) ) )
        {
        HDAudio::Error(hr);
        return FALSE;
        }
    }
  return TRUE;
} // StopOutputBuffer
//////////////////////////////////////////////////////////////////////////////////////////////

HDAudio::Stop( )
{
  HRESULT       hr;



  // Stop the Capture Buffer
  lpdscb->Stop();
  // Stop Output Buffer 
  lpdsb->Stop();

  if ( lpDSNotify != NULL )
  {
    if ( FAILED( hr = lpDSNotify->SetNotificationPositions( 0, NULL ) ) )
      {
      HDAudio::Error(hr);
      return FALSE;
      }
  }  

  return TRUE;

} // Stop
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::InitializeBuffer( DWORD dwStart, DWORD cbLength )
{
  // Empty the output buffer

  PBYTE     pb1, pb2;
  DWORD     cb1, cb2;

  if ( ( !dwStart && !cbLength ) || ( NULL == lpdsb )
        || ( NULL == dsbDesc.lpwfxFormat ) )
    return FALSE;

  if ( SUCCEEDED( lpdsb->Lock( dwStart, cbLength, ( LPVOID * )&pb1, &cb1, 
                              ( LPVOID * )&pb2, &cb2, 0 ) ) ) 
    {
      FillMemory( pb1, cb1, ( dsbDesc.lpwfxFormat->wBitsPerSample == 8 ) ? 128 : 0 );
      
      if ( NULL != pb2 && cb2 )
        FillMemory( pb2, cb2, ( dsbDesc.lpwfxFormat->wBitsPerSample == 8 ) ? 128 : 0 );

      lpdsb->Unlock( pb1, cb1, pb2, cb2 );
      return TRUE;

    }

  return FALSE;

} // InitializeBuffer
//////////////////////////////////////////////////////////////////////////////////////////////

HDAudio::Release( )
{
if ( lpDSNotify ) lpDSNotify->Release();
if ( lpdsnOutput ) lpdsnOutput->Release();
if ( lpdsn ) lpdsn->Release();
if ( lpds ) lpds ->Release(); // Release all the buffers
if ( lpdsc ) lpdsc->Release();
} // Release
//////////////////////////////////////////////////////////////////////////////////////////////

BOOL HDAudio::Calibration(LPBYTE pbSource, DWORD cbLength)
{
	short *piData[2], j;
	double dDC, dX;

	piData[0] =  (short *)pbSource; 
	
	dDC = 0.0;
	dX = 0.0;
	for (j=0; j<NUMBGBLK*GOERTZBLK/2; j++) {
		dDC += (double)*piData[0];
		dX += ((double)*piData[0] * (double)*piData[0]);
		piData[0]++;
	}
	dDC = dDC/(NUMBGBLK*GOERTZBLK);

	dX = dX/(NUMBGBLK*GOERTZBLK/2);


	// Calculate DC offset

	ReturnVal.dDC = (dDC/32768)*2;
	ReturnVal.dPwr = dX;

	return TRUE;
}


