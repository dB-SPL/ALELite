// CalDlg.cpp : implementation file
//

#include "stdafx.h"

#include <mmsystem.h>
#include <dsound.h>  
#include "hdaudio.h"
#include <stdio.h>
#include <process.h>

#include "Calibrate.h"
#include "CalibrateDoc.h"
#include "CalibrateView.h"
#include "CalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////
extern struct strReturnVal ReturnVal;   //
//////////////////////////////////////////

static int iBtnState = 0;


UINT RxThread(LPVOID);

/////////////////////////////////////////////////////////////////////////////
// CCalDlg dialog


CCalDlg::CCalDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalDlg::IDD, pParent)
{

	//{{AFX_DATA_INIT(CCalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalDlg, CDialog)
	//{{AFX_MSG_MAP(CCalDlg)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalDlg message handlers

void CCalDlg::OnButton1() 
{
	int x = 0;
	char str[10];

	switch (iBtnState) {
		case 0:
			ctlText3().SetWindowText("Average Power:");
			ctlText5().SetWindowText("DC Offset (V):");
			ctlText0().SetWindowText("Now adjust the PC sound card mixer Line-In level " 
        "during any tone portion of the WWV signal. Adjust the mixer until the "
        "Average Power value, shown below, during the tone portion, is between "
        "1.00e+007 and 2.00e+007. Then click Lock to accept this value.");
			ctlButton1().SetWindowText("Lock");
			// Set any parameters for the the thread here
			rxthreadParms.pWnd = AfxGetMainWnd();
			rxthreadParms.bContinue = TRUE;

			pRxThread = AfxBeginThread(RxThread, &rxthreadParms);
			
			break;
		case 1:
			dSigInPwr = ReturnVal.dPwr;

      rxthreadParms.bContinue = FALSE;
			::Sleep(500);
			ctlText3().SetWindowText("Locked Value:");
		  ctlText4().SetWindowText("0.0");
			ctlText5().SetWindowText("");
			sprintf(str, "%5.2e", dSigInPwr);
			ctlText1().SetWindowText(str);


			ctlText0().SetWindowText("Examine the Locked Value below. If it is not at least "
        "1.00e+007, press Cancel and begin again. Otherwise, close the mixer program "
        "and tune the radio to an unoccupied frequency near WWV and press Next.");
			ctlButton1().SetWindowText("Next");
			break;
		case 2:			
			ctlText0().SetWindowText("This portion of the calibration process will determine"
        "your system's Noise Power level. Allow the Noise Power, shown below, to "
        "stabilize (i.e., the value doesn't vary by more than ~30%) and then press "
        "Lock. If it does not stabilize, try to press Lock near it's highest value.");
			ctlText3().SetWindowText("Noise Power:");
			ctlText5().SetWindowText("DC Offset (V):");
			ctlText1().SetWindowText("0.0");
			ctlText4().SetWindowText("0.0");
			ctlButton1().SetWindowText("Lock");

			// Set any parameters for the the thread here
			rxthreadParms.pWnd = AfxGetMainWnd();
			rxthreadParms.bContinue = TRUE;

			pRxThread = AfxBeginThread(RxThread, &rxthreadParms);

			break;
		case 3:
			dNoise = ReturnVal.dPwr;

      rxthreadParms.bContinue = FALSE;
			::Sleep(500);

			ctlText3().SetWindowText("Locked Value:");
		    ctlText4().SetWindowText("0.0");
			ctlText5().SetWindowText("");
			sprintf(str, "%5.2e", dNoise);
			ctlText1().SetWindowText(str);


			ctlText0().SetWindowText("Examine the Locked Value. If it is not near the "
        "target value you selected, press Cancel and begin again. Otherwise, press "
        "Exit to complete the calibration process, then close this program.");
			ctlButton1().SetWindowText("Exit");
			break;
		case 4:			
			
      // Write out the file
			FILE *fpOut;

			if( NULL == (fpOut = fopen("calval.txt", "w"))) {
				MessageBox("Can't open 'calval.txt' for output", "HF ALE Calibration Error", MB_OK | MB_ICONERROR);
			} else {
				// There are 88 Samples in a G Block. dNoise is the Noise power per sample
				// multiply these two together. Since there are 60 blocks in a buffer in Demod
				// multiply the power/block * # of blocks. The result will be the total power in 
				// a Demod buffer. To match the power calculation in the Goertz blk in Demod we have to 
				// multiply by 44. Then multiply by 2 (arbitarily) to give 3dB clearance from the calculated noise floor
				dNoise = dNoise * 88 * 60 * 44 * 2;	
				fprintf(fpOut, "%e\n", dNoise);
				fclose(fpOut);
			}

			DestroyWindow();

			break;
		default:
			break;
	}
	iBtnState = (++iBtnState)%5;
}


BOOL CCalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ctlText0().SetWindowText("This program uses WWV to calibrate the ALE software modem. "
    "First, tune your radio to the WWV signal (2.5, 5.0, 10.0, 15.0 and 20.0 MHz) "
    "that is strongest at the moment. Next, start the sound card mixer program (that "
    "is supplied with Windows or your sound card) and select the Line-In recording "
    "adjustment. Then click Begin below to start the calibration");
	ctlText1().SetWindowText("0.0");
	ctlText4().SetWindowText("0.0");
	ctlButton1().SetWindowText("Begin");
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CCalDlg::UpdateValue()
{
  CString str;

  str.Format("%5.2e", ReturnVal.dPwr);
  ctlText1().SetWindowText(str);
  str.Format("%5.2e", ReturnVal.dDC);
  ctlText4().SetWindowText(str);

}

void CCalDlg::OnCancel() 
{

  iBtnState = 0;

  // In case the user stopped the dialog box when the thread was running
	rxthreadParms.bContinue = FALSE;
  ::Sleep(500);

	DestroyWindow();
}

void CCalDlg::PostNcDestroy()
{
  view->pCalDlg = NULL;
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// Receiver Thread

UINT RxThread(LPVOID pParam)
{
  RXTHREADPARMS* pRxThreadParms = (RXTHREADPARMS*) pParam;

  HDAudio HDA;                            
  // External for HDAudio				          //
  extern int iInOrOut;                    
  extern HANDLE     rghEvent[];           
  
  MSG msg;

  pRxThreadParms->value = 0.0; // Just for test

	if (FALSE == HDA.InitDSound(pRxThreadParms->pWnd)) {// If you can't initialize DirectSound quit
		HDA.Release( );
    return 0;
	}
	iInOrOut = INPUTBUFFER;
	HDA.InitNotify( );
	HDA.Start( );			

  // Loop here and take measurements
  while (pRxThreadParms->bContinue) {
    DWORD dwEvt = MsgWaitForMultipleObjects(
                        NUMBEVENTS+1, // Number of Events
                        rghEvent,           // Location of handles.
                        FALSE,              // Wait for all?
                        INFINITE,           // How long to wait?
                        QS_ALLINPUT );      // Any message is an event

    // WAIT_OBJECT_0 == zero but is properly treated as an arbitrary
    // index value assigned to the first event, therefore we subtract
    // it from dwEvt to get the zero-based index of the event

    dwEvt -= WAIT_OBJECT_0;

    // If the event was set by the capture buffer, you have input
    // to process.
    if ( dwEvt < NUMBEVENTS )
      {
        iInOrOut = INPUTBUFFER;
        HDA.ProcessInputBuffer( dwEvt );
        // Periodically send back a value to be displayed
        pRxThreadParms->value += 1.0; // Just for test
        pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
      }
    // The next event in the array is the one set by the output buffer
    // when is has been told to play remaining data and stop.
    else if ( dwEvt == NUMBEVENTS )
      {
      HDA.StopOutputBuffer( );
      }
    // If it's the last event, it's a message.
    else if ( dwEvt == NUMBEVENTS + 1 )
      {
      while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
        {
        // If it's a quit message, were out of here.
        if ( msg.message == WM_QUIT )
          {
          pRxThreadParms->bContinue = TRUE;
          }
        else 
          {
          TranslateMessage(&msg);
		  DispatchMessage(&msg);
		  }
      }
    } // End message Processing
  } // while ( pRxThreadParms->bContinue )

  // This is where the thread terminates
  // Give up any resources allocated here

 	HDA.Release( );

  return 0;
}

