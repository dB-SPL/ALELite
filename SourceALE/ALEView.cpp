// ALEView.cpp : implementation of the CALEView class
//  20 Jan 2000

#include "stdafx.h"
#include "ALE.h"

//#include "SignalQualityDlg.h"
#include "ALEDoc.h"
#include "Config.h"
#include "ChannelList.h"
#include "SelfAddrList.h"
#include "OtherAddrList.h"
#include "SelectSoundAddrs.h"
#include "SelectRxChannel.h"
#include "SelectIndivAddr.h"
#include "SelectSelfAddr.h"
#include "AMDText.h"
#include "ALEView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CALEView

IMPLEMENT_DYNCREATE(CALEView, CView)

BEGIN_MESSAGE_MAP(CALEView, CView)
	//{{AFX_MSG_MAP(CALEView)
	ON_COMMAND(ID_EDIT_CHANNEL, OnEditChannel)
	ON_COMMAND(ID_EDIT_SELFADDR, OnEditSelfaddr)
	ON_COMMAND(ID_EDIT_OTHERADDR, OnEditOtheraddr)
	ON_COMMAND(ID_SOUNDING_START, OnSoundingStart)
	ON_UPDATE_COMMAND_UI(ID_SOUNDING_START, OnUpdateSoundingStart)
	ON_WM_TIMER()
	ON_COMMAND(ID_VIEW_SIGNALQUALITY, OnViewSignalquality)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIGNALQUALITY, OnUpdateViewSignalquality)
	ON_COMMAND(ID_RX_SCANNING, OnRxScanning)
	ON_UPDATE_COMMAND_UI(ID_RX_SCANNING, OnUpdateRxScanning)
	ON_COMMAND(ID_RX_OFF, OnRxOff)
	ON_UPDATE_COMMAND_UI(ID_RX_OFF, OnUpdateRxOff)
	ON_COMMAND(ID_RX_SINGCHAN, OnRxSingchan)
	ON_UPDATE_COMMAND_UI(ID_RX_SINGCHAN, OnUpdateRxSingchan)
	ON_COMMAND(ID_EDIT_CONFIG, OnEditConfig)
	ON_COMMAND(ID_INDIVIDUALCALL, OnIndividualCall)
	ON_UPDATE_COMMAND_UI(ID_INDIVIDUALCALL, OnUpdateIndividualcall)
	ON_COMMAND(ID_ALLCALL, OnAllcall)
	ON_COMMAND(ID_VIEW_AMD, OnViewAmd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_AMD, OnUpdateViewAmd)
	ON_COMMAND(ID_AMDMESSAGE, OnSendAMDMessage)
	ON_COMMAND(ID_DEBUG_MODE, OnDebugMode)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALEView construction/destruction

CALEView::CALEView()
{
  pSQDlg = NULL;
  pAMDDlg = NULL;
}

CALEView::~CALEView()
{
}

BOOL CALEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CALEView drawing

void CALEView::OnDraw(CDC* pDC)
{
  int i;

  CRect rect;
  TCHAR szMessage2[128];

	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  if (pDoc->bDebugMode) {
    GetClientRect(&rect);

    for (i = 0; i < 20; i++) {
      if (i == pDoc->RXBufPtr)
        pDoc->szRXMessage[i][0] = '>';
      else
        pDoc->szRXMessage[i][0] = ' ';
      pDC->DrawText(pDoc->szRXMessage[i], strlen(pDoc->szRXMessage[i]), &rect, DT_LEFT);
      rect.top += 15;
    }

    // Check for a returned error from the DirectX Services
    rect.top += 15;
    sprintf(szMessage2, "%s", "                                                             ");
    pDC->DrawText(szMessage2, strlen(szMessage2), &rect, DT_LEFT);
    sprintf(szMessage2, "Error Status from DirectX Services -> %d", 
      (short)pDoc->m_rxthreadParms.hrModemError);
    pDC->DrawText(szMessage2, strlen(szMessage2), &rect, DT_LEFT);
  }
}

/////////////////////////////////////////////////////////////////////////////
// CALEView printing

BOOL CALEView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CALEView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CALEView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CALEView diagnostics

#ifdef _DEBUG
void CALEView::AssertValid() const
{
	CView::AssertValid();
}

void CALEView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CALEDoc* CALEView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CALEDoc)));
	return (CALEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CALEView message handlers


void CALEView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();

}

void CALEView::OnEditConfig()
{
	CConfig dlg(this);

	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  dlg.pData = pDoc->pData;
  dlg.hComm = pDoc->hComm;
  if (dlg.DoModal() == IDOK) {
    pDoc->m_bRadioInitialized = FALSE;
  }
}

void CALEView::OnEditChannel() 
{
	CChannelList dlg(this);

  dlg.pData = GetDocument()->pData;
  dlg.DoModal();
}

void CALEView::OnEditSelfaddr() 
{
	CSelfAddrList dlg(this);

  dlg.pData = GetDocument()->pData;
  dlg.DoModal();
}

void CALEView::OnEditOtheraddr() 
{
	COtherAddrList dlg(this);

  dlg.pData = GetDocument()->pData;
  dlg.DoModal();
}

void CALEView::OnSoundingStart() 
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // If not already done, initialize the serial port and radio
  if (!pDoc->m_bRadioInitialized)
    if (!pDoc->InitializeRadio())
      return;

  if (!pDoc->m_bSounding) {
    pDoc->m_bSounding = SoundingOn();
  } else {
    SoundingOff();
    pDoc->m_bSounding = FALSE;
    pDoc->bSoundingInterrupted = FALSE;
  }
}

void CALEView::OnUpdateSoundingStart(CCmdUI* pCmdUI)
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  if (pDoc->m_bSounding) {
    pCmdUI->SetText("&Stop Sounding");
    pCmdUI->Enable();
  } else {
    pCmdUI->SetText("&Start Sounding");
    pCmdUI->Enable();
  }
}

BOOL CALEView::SoundingOn()
// Set up a linked list of channels to be sounded.
// The operator selects one or more self addresses to be sounded.
// All channels that have a defined Sounding Interval for each
// self address are set up in the linked list.
// A timer is started that will signal each sound.
{
  int i, j, numSelfs, numChans;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::CHANNEL ch;
  struct CALEDoc::SOUNDENTRY NextSound;
	CSelectSoundAddrs dlg(NULL);
	
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  dlg.pData = pDoc->pData;
  if (dlg.DoModal() == IDOK) {
    // Get the time
    CTime time = CTime::GetCurrentTime();

    // Set up a sorted linked list for the time schedule of channels to
    // be sounded.  Set each to sound now.  (Initially all have the same
    // start time, so sorting is not needed.)
    numSelfs = pDoc->pData->parrSelfAddrs->GetSize();
    numChans = pDoc->pData->parrChannels->GetSize();
    for (i = 0; i < numSelfs; i++) {
      sa = pDoc->pData->parrSelfAddrs->GetAt(i);
      if (sa.bSoundingOn) {
        for (j = 0; j < numChans; j++) {
          if (sa.parrValidChannels->GetAt(j)) {
            ch = pDoc->pData->parrChannels->GetAt(j);
            if (ch.dSoundInterval > 0.0) {
              NextSound.iChannel = j;
              NextSound.iSelfAddr = i;
              NextSound.TimeSound = time;
              pDoc->listSounds.AddTail(NextSound);
            }
          }
        }
      }
    }

    pDoc->bSoundingInterrupted = FALSE;

    // Set a timer for the first sound
    pDoc->iTimerSounding = SetTimer(ID_TIMER_SOUNDING, 50, NULL);
    return TRUE;  // For sounding started
  }
  return FALSE;  // For sounding NOT started
}

void CALEView::SoundingOff()
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // Kill the timer
  KillTimer(pDoc->iTimerSounding);
  // Delete the linked list
  pDoc->listSounds.RemoveAll();
}


void CALEView::OnTimer(UINT nIDEvent) 
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  if (pDoc->m_bSounding) {
    KillTimer(pDoc->iTimerSounding);
    // Get the first entry on the Sounding linked list
    pDoc->NextSound = pDoc->listSounds.RemoveHead();
    AfxGetMainWnd()->PostMessage(WM_USER_ACTION, 15, 0L);
  }

	CView::OnTimer(nIDEvent);
}

void CALEView::OnViewSignalquality() 
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  if (pSQDlg != NULL) {
    pSQDlg->DestroyWindow();  // This sends a WM_NCDESTROY message to the window. 
  } else {
    pSQDlg = new CSignalQualityDlg;

    pSQDlg->pData = pDoc->pData;
    pSQDlg->view = this;
    pSQDlg->Create(IDD_SIGNALQUALITY, this);
  }
}

void CALEView::OnUpdateViewSignalquality(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pSQDlg != NULL);
}

void CALEView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
  if ((pSQDlg != NULL) && ((lHint == 1L) || (lHint == 2))) {
    // An entry in the lqa array has changed or there is a new lqa entry
    pSQDlg->LqaModified(lHint, (LPARAM) pHint);
  } else if ((pAMDDlg != NULL) && (lHint == 3L)) {
    // There is a new AMD entry
    pAMDDlg->AMDModified((LPARAM) pHint);
  } else
    CView::OnUpdate(pSender, lHint, pHint);
}

void CALEView::OnRxScanning() 
{
  int i, size;
  struct CALEDoc::CHANNEL ch;

	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // Make sure there is at least one channel to scan
  size = pDoc->pData->parrChannels->GetSize();
  for (i = 0; i < size; i++) {
    ch = pDoc->pData->parrChannels->GetAt(i);
    if (ch.bScanMode)
      goto StartScanningOK;
  }
  // If here, couldn't find a channel to scan
  MessageBox("There aren't any Channels to scan!", "Alert", MB_OK);
  return;

StartScanningOK:
  // If not already done, initialize the serial port and radio
  if (!pDoc->m_bRadioInitialized)
    if (!pDoc->InitializeRadio())
      return;

  pDoc->iCurrentPriority = 0;
  pDoc->m_rxthreadParms.eRxMode = SCANNING;
  // Set the scan channel so scanning starts at the first channel
  pDoc->m_rxthreadParms.iScanChannel = -1;
  pDoc->NextAction();
}

void CALEView::OnUpdateRxScanning(CCmdUI* pCmdUI) 
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  pCmdUI->SetCheck(pDoc->m_rxthreadParms.eRxMode == SCANNING ? 1 : 0);
}

void CALEView::OnRxOff() 
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  pDoc->m_rxthreadParms.eRxMode = OFF;
  if (pDoc->iCurrentPriority <= 10) {
    pDoc->iCurrentPriority = 0;
    pDoc->m_workingEvent.ResetEvent();

    // Update the status bar
    pDoc->pStatusBar->SetPaneText(1, "Receive mode is OFF", TRUE);
  }
}

void CALEView::OnUpdateRxOff(CCmdUI* pCmdUI)
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  pCmdUI->SetCheck(pDoc->m_rxthreadParms.eRxMode == OFF ? 1 : 0);
}

void CALEView::OnRxSingchan() 
{
	CSelectRxChannel dlg(NULL);

  CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // If not already done, initialize the serial port and radio
  if (!pDoc->m_bRadioInitialized)
    if (!pDoc->InitializeRadio())
      return;

  // Select the channel to receive
  dlg.pData = pDoc->pData;
  dlg.iSelfAddrMode = 0;
  if (dlg.DoModal() == IDOK) {
    pDoc->iSingleChannel = dlg.iChanNum;
    pDoc->m_rxthreadParms.eRxMode = SINGLE_CHANNEL;
    pDoc->NextAction();
    return;
  }

  // No channel selected, leave receive mode the way it was
  return;
}

void CALEView::OnUpdateRxSingchan(CCmdUI* pCmdUI)
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  pCmdUI->SetCheck(pDoc->m_rxthreadParms.eRxMode == SINGLE_CHANNEL ? 1 : 0);
}

void CALEView::OnIndividualCall()
{
	CSelectIndivAddr dlg(NULL);

  CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // If not already done, initialize the serial port and radio
  if (!pDoc->m_bRadioInitialized)
    if (!pDoc->InitializeRadio())
      return;

  // Check if currently connected.  If so, ask if this connection should be broken.
  if (pDoc->iCurrentPriority == 30) {
    if (MessageBox("Break the current connection?", "Alert", MB_YESNO) == IDYES) {
      pDoc->BreakConnection();
    }
  } else {
    // Select the other address to contact
    dlg.pData = pDoc->pData;
    if (dlg.DoModal() == IDOK) {
      if (dlg.eChannelMode == AUTO)
        pDoc->PrioritizeChannels(dlg.iSelfAddr, dlg.iOtherAddr);
      else
        pDoc->SingleChannel(dlg.iChannel);
      pDoc->sChannelPriorities.iOtherAddr = dlg.iOtherAddr;
      pDoc->sChannelPriorities.iSelfAddr = dlg.iSelfAddr;
      AfxGetMainWnd()->PostMessage(WM_USER_ACTION, 25, NULL);
      return;
    }

    // No channel selected, quit
    return;
  }
}

void CALEView::OnUpdateIndividualcall(CCmdUI* pCmdUI)
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  if (pDoc->iCurrentPriority == 30) {
    pCmdUI->SetText("&Break Connection");
    pCmdUI->Enable();
  } else {
    pCmdUI->SetText("&Individual Call");
    pCmdUI->Enable();
  }
}

void CALEView::OnAllcall() 
{
	CSelectSelfAddr dlg(NULL);

  CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // If not already done, initialize the serial port and radio
  if (!pDoc->m_bRadioInitialized)
    if (!pDoc->InitializeRadio())
      return;

  // Check if currently connected.  If so, ask if this connection should be broken.
  if (pDoc->iCurrentPriority == 30) {
    if (MessageBox("Break the current connection?", "Alert", MB_YESNO) == IDYES) {
      pDoc->BreakConnection();
    }
  } else {
    // Select the channel to transmit on
    dlg.pData = pDoc->pData;
    if (dlg.DoModal() == IDOK) {
      pDoc->DoAllCall(dlg.iSelfAddr, dlg.iChannel);
      return;
    }

    // No channel selected, quit
    return;
  }
}

void CALEView::OnViewAmd() 
{
	CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  if (pAMDDlg != NULL) {
    pAMDDlg->DestroyWindow();  // This sends a WM_NCDESTROY message to the window. 
  } else {
    pAMDDlg = new CAMDDisplay;

    pAMDDlg->pData = pDoc->pData;
    pAMDDlg->view = this;
    pAMDDlg->Create(IDD_AMD, this);
  }
}

void CALEView::OnUpdateViewAmd(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(pAMDDlg != NULL);
}

void CALEView::OnSendAMDMessage() 
{
  CSelectIndivAddr dlg(NULL);
  CAMDText dlg2;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;
  WPARAM priority = 22;

  CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  // If not already done, initialize the serial port and radio
  if (!pDoc->m_bRadioInitialized)
    if (!pDoc->InitializeRadio())
      return;

  // If not currently connected, ask for TO and FROM address and channel
  if (pDoc->iCurrentPriority != 30) {
    // Select the other address to contact
    dlg.pData = pDoc->pData;
    if (dlg.DoModal() == IDOK) {
      if (dlg.eChannelMode == AUTO) {
        pDoc->PrioritizeChannels(dlg.iSelfAddr, dlg.iOtherAddr);
        pDoc->sResponseData.iChannelNumber = pDoc->sChannelPriorities.arrChannels[0];
      } else {
        pDoc->SingleChannel(dlg.iChannel);
        pDoc->sResponseData.iChannelNumber = dlg.iChannel;
      }
      pDoc->sChannelPriorities.iOtherAddr = dlg.iOtherAddr;
      oa = pDoc->pData->parrOtherAddrs->GetAt(dlg.iOtherAddr);
      pDoc->sResponseData.strFromAddr = oa.strOtherAddr;
      sa = pDoc->pData->parrSelfAddrs->GetAt(dlg.iSelfAddr);
      pDoc->sResponseData.strToAddr = sa.strSelfAddr;
//      pDoc->sChannelPriorities.iSelfAddr = dlg.iSelfAddr;
    } else
      // No channel selected, quit
      return;
  } else
    priority += 10;  // Make the priority 32 if already connected

  // Get the text for the AMD message
  dlg2.pData = pDoc->pData;
  if (dlg2.DoModal() == IDOK) {
    pDoc->strAMDText = dlg2.strAMDText;
  } else
    // No text entered, quit
    return;

  AfxGetMainWnd()->PostMessage(WM_USER_ACTION, priority, NULL);
  return;
}

void CALEView::OnDebugMode() 
{
  CALEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  pDoc->bDebugMode = !pDoc->bDebugMode;
  OnUpdate(NULL, 0L, NULL);
}
