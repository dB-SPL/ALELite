// ALEDoc.h : interface of the CALEDoc class
//  24 Jan 2000
/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>
#include <afxmt.h>
#include "CircBuf.h"
#include "Radio.h"
#include "Icom.h"

#if !defined(AFX_ALEDOC_H__1A7D6914_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
#define AFX_ALEDOC_H__1A7D6914_335C_11D2_8F37_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_TX_SIZE CIRCBUFOSIZE*49  // The maximum size, in symbols of an ALE transmission

enum RECEIVEMODE {OFF, SINGLE_CHANNEL, SCANNING};
enum RADIOTYPE {ICOM};
enum CHANNELMODE {AUTO, MANUAL};

typedef struct tagRXTHREADPARMS {
  // Add any data to pass to thread
  CWnd *pWnd;
  CEvent *pworkingEvent, *pstoppedEvent;
  BOOL m_bReceiving;  // True if receiving; False if transmitting
  int dwell;  // Number of symbols to look at before giving up syncing
  CCircBufO *pCircBufO;
  CCircBufI *pCircBufI;
  RECEIVEMODE eRxMode;  // Which receive mode we are in
  int iPriority;        // Current priority
  double dNoiseThreshold;  // Set by the Calibrate program, then read in and passed to the thread
  int iScanChannel;  // Used to keep track of which channel is being scanned
  BOOL m_bSignalCheck;  // Set true when receiving before transmission
                        // to check for signal presence
  BOOL m_bChannelClear;  // The result when checking channel before transmitting

  HRESULT hrModemError;
  // If hrModemError
  //             = 0 The data from the receiver is fine No Error
  //             = 1 The initialization of DirectX did occur 
  //             > 100 One of the DirectX services has failed 
} RXTHREADPARMS;

class CALEDoc : public CDocument
{
protected: // create from serialization only
	CALEDoc();
	DECLARE_DYNCREATE(CALEDoc)

// Attributes
private:
  int iParsePtr;       // Used during message parsing to keep current location

  struct {
    long lALE;
    long Ucount;
    double dSNR;
  } lMessageBuf[100];  // An array of ALE words received, one complete transmission
  int indMessageBuf;  // current number of ALE words in the buffer
  CWinThread* pRxThread;

#define GOLAY_SCAN	1
#define GOLAY_RUN	3

#define _TO   020000000  /* use these to build or parse ALE words */
#define _TIS	050000000
#define _TWAS	030000000
#define _DATA	000000000
#define _REP	070000000
#define _CMD	060000000
#define _THRU	010000000
#define _FROM	040000000

public:
  struct SOUNDENTRY {
    int iChannel;  // index of channel in arrChannels
    int iSelfAddr; // index of self address in arrSelfAddrs
    CTime TimeSound;  // Time to sound
  };

  RXTHREADPARMS m_rxthreadParms;
  CEvent m_workingEvent, m_stoppedEvent;
  CCircBufO m_CircBufO;
  CCircBufI m_CircBufI;
  BOOL bDebugMode;
  // For display of the symbols
  short iInOrOut;
  CIRCBUFINDATA RXBuffer;
  TCHAR szRXMessage[20][135];
  int RXBufPtr;
  // The priority of the current modem/thread operation, used to determine
  // whether the current operation is interrupted
  //  35 = Break Connection message being sent
  //  32 = Sending AMD while already connected
  //  30 = Connected
  //  25 = Calling out
  //  22 = Sending AMD
  //  20 = Calling in
  //  18 = Allcall
  //  15 = Sounding
  //  10 = Scanning (applies to a signal being received)
  //   5 = Single frequency receiving
  int iCurrentPriority;
  BOOL m_bRadioInitialized;  // True when the radio has been initialized
  HANDLE hComm;  // Handle to the computer serial port for radio control
  BOOL m_bSounding;  // True when Sounding is turned on
  BOOL bSoundingInterrupted;  // True when Sounding is turned on but interrupted by higher priority
  UINT iTimerSounding;  // The identifier for the Sounding timer
  struct SOUNDENTRY NextSound;
  int iSingleChannel;  // When in single channel mode, the number of the channel
  long lTransBuf[CIRCBUFOSIZE];  // Array of ALE words to be transmitted
  CStatusBar *pStatusBar;
  CString strStatus;
  CString strFilePath; // The path for the open document/file
  CString strAMDText;  // The text sent in an AMD message

public:
  enum TRMODE {TR, RECONLY};

  struct CONFIG {
    RADIOTYPE eRadioType;
    BYTE bRadioAddress;
    CString strCommPort;
    long lBaud;
  } stConfig;

  struct CHANNEL {
    double dTransFreq;
    CRadio::RADIOMODE eTransMode;
    double dRecFreq;
    CRadio::RADIOMODE eRecMode;
    TRMODE eTRMode;
    BOOLEAN bScanMode;
    double dSoundInterval;
  };

  CArray< struct CHANNEL, struct CHANNEL& > arrChannels;

  struct SELFADDR {
    CString strSelfAddr;
    CString strNetAddr;
    int m_nSlotWait;
    CArray< BOOLEAN, BOOLEAN > *parrValidChannels;
    BOOLEAN bSoundingOn;
  };

  CArray< struct SELFADDR, struct SELFADDR& > arrSelfAddrs;

  struct OTHERADDR {
    CString strOtherAddr;
    double dReplyWait;
    CArray< BOOLEAN, BOOLEAN > *parrValidChannels;
    CStringArray *pSlotStrings;
  };

  CArray< struct OTHERADDR, struct OTHERADDR& > arrOtherAddrs;

  struct LQAVALUE {
    int value;
    CTime time;
  };

  struct LQA {
    CString strOtherAddr;
    double dRxFreq;
    LQAVALUE RxBER, RxSNR, TxBER, TxSINAD;
  };

  CArray< struct LQA, struct LQA& > arrLQAs;  // Table of signal qualities

  struct CHANRANK {  // Used for determining channel priorities when connecting
    int iChannelNum;
    double dRxFreq;
    int iScore;
  };

  struct AMDMESSAGE {
    CString strFrom;
    CString strTo;
    CString strFreq;
    CString strTime;
    CString strMessage;
  };

  CArray< struct AMDMESSAGE, struct AMDMESSAGE& > arrAMDMessages;

  struct CHANNELPRIORITY {
    int iNumChannels,   // The number of channels in the array
        iNextChannel,   // Index into the array for the channel to be tried next
        *arrChannels,   // An array of channel numbers, ordered so the
                        // first is the most likely to connect
        iOtherAddr,     // Index into arrOtherAddrs for the "to" address
        iSelfAddr;      // Index into arrSelfAddrs for the "from" address
  } sChannelPriorities;

  CList<struct SOUNDENTRY, struct SOUNDENTRY&> listSounds;

  struct RESPONSEDATA {
    CString strToAddr;
    CString strFromAddr;
    int iChannelNumber;
  } sResponseData;

  struct DATA {
    CALEDoc *pDoc;
    struct CONFIG *pConfig;
    CArray< struct CHANNEL, struct CHANNEL& > *parrChannels;
    CArray< struct SELFADDR, struct SELFADDR& > *parrSelfAddrs;
    CArray< struct OTHERADDR, struct OTHERADDR& > *parrOtherAddrs;
    CArray< struct LQA, struct LQA& > *parrLQAs;
    CArray< struct AMDMESSAGE, struct AMDMESSAGE& > *parrAMDMessages;
  } Data, *pData;

  CRadio *pRadio;

//  CIcom Icom;  // For now, this is the only radio possible

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CALEDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
private:
  long ThreeChar(CString str);
  CString ParseToAddr();
  CString ParseAMD(BOOL *bAMDPresent);
  CString ParseFromAddr(BOOL *bTis);
  void HandleAMD(CString strToAddr, CString strFromAddr, CString strAMD);
  void LogLQA(CString strAddr);
  BOOL MessageForUs(CString strToAddr);
  void SendResponse(CString strToAddr, CString strFromAddr);
  void SendAcknowledgement();
  void ParseMessage();
  void DoSound();
  void DoScan(int iChannel);
  void DoSingleChannel(int iChannel);
  void DoConnection();
  void SendAMD(int iPriority);

public:
	virtual ~CALEDoc();
  BOOL InitializeRadio();
  void OnReceive(LONG dwell, int priority);
  BOOL CheckChannelClear(int priority);
  void Transmit(LONG, int priority);
  int MakeAddrWords(const int iWordType, const CString strAddr, long ALEWords[]);
  void NextAction();
  void DataAvail();
  LONG OnModemAction(UINT, LONG);
  void PrioritizeChannels(int iSelfAddr, int iOtherAddr);
  void SortChannels(CHANRANK *ChanRanks, int count);
  void SingleChannel(int iChannel);
  void BreakConnection();
  void DoAllCall(int iSelfAddr, int iChanNum);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CALEDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALEDOC_H__1A7D6914_335C_11D2_8F37_0060089F9D1C__INCLUDED_)
