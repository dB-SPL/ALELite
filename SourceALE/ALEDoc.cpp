// ALEDoc.cpp : implementation of the CALEDoc class
//  24 Jan 2000

#include "stdafx.h"
#include "ALE.h"

#include "ALEDoc.h"
#include "SelectIndivAddr.h"
#include "ALEConstants.h"
#include "ASCII_Set.h"
#include "ALEView.h"
#include "SunTimes.h"
#include "Dlgfloat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT RxThread(LPVOID);
long ASCII_OK(long w, char set);

// CALEDoc

IMPLEMENT_DYNCREATE(CALEDoc, CDocument)

BEGIN_MESSAGE_MAP(CALEDoc, CDocument)
	//{{AFX_MSG_MAP(CALEDoc)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CALEDoc construction/destruction

CALEDoc::CALEDoc() : m_workingEvent(FALSE, TRUE), m_stoppedEvent(FALSE, TRUE)
{
  CStdioFile fCalVal;
  CString strCalVal;
  double dNoiseThreshold;

  arrChannels.SetSize(0, 5);
  arrSelfAddrs.SetSize(0, 5);
  arrOtherAddrs.SetSize(0, 5);
  arrLQAs.SetSize(0, 5);
  arrAMDMessages.SetSize(0, 5);
  Data.pConfig = &stConfig;
  Data.parrChannels = &arrChannels;
  Data.parrSelfAddrs = &arrSelfAddrs;
  Data.parrOtherAddrs = &arrOtherAddrs;
  Data.parrLQAs = &arrLQAs;
  Data.parrAMDMessages = &arrAMDMessages;
  Data.pDoc = this;
  pData = &Data;

  pRadio = NULL;

  bDebugMode = FALSE;
  m_bSounding = FALSE;
  bSoundingInterrupted = FALSE;
  iCurrentPriority = 0;
  m_bRadioInitialized = FALSE;
  hComm = 0;

  sChannelPriorities.iNumChannels = 0;

  // Read in the Noise Threshold from the CalVal.txt file (created by
  // the Calibrate program).
  if (fCalVal.Open("CalVal.txt", CFile::modeRead | CFile::typeText)) {
    fCalVal.ReadString(strCalVal);
    if (!_AfxSimpleFloatParse(strCalVal, dNoiseThreshold)) {
      MessageBox(NULL, "The value in file 'CalVal.txt' (from the Calibrate program) is invalid.\n"
        "This program will continue with no calibration, but the best action is to\n"
        "quit, run the Calibrate program, and then rerun this program.", "Alert", MB_OK);
      m_rxthreadParms.dNoiseThreshold = 1.712307e12;  // Default value
    } else
      m_rxthreadParms.dNoiseThreshold = dNoiseThreshold;
  } else {
    MessageBox(NULL, "The file 'CalVal.txt' (from the Calibrate program) cannot be found.\n"
      "This program will continue with no calibration, but the best action is to\n"
      "quit, run the Calibrate program, and then rerun this program.", "Alert", MB_OK);
    m_rxthreadParms.dNoiseThreshold = 1.712307e12;  // Default value
  }

  // Set any parameters for the RxThread
  m_rxthreadParms.pworkingEvent = &m_workingEvent;
  m_rxthreadParms.pstoppedEvent = &m_stoppedEvent;
  m_rxthreadParms.pCircBufO = &m_CircBufO;
  m_rxthreadParms.pCircBufI = &m_CircBufI;
  m_rxthreadParms.eRxMode = OFF;
  m_rxthreadParms.m_bSignalCheck = FALSE;
  m_rxthreadParms.hrModemError = 0;
  m_workingEvent.ResetEvent();  // Just to be sure
  m_stoppedEvent.SetEvent();

  for (int i = 0; i < 20; i++)
    strcpy(szRXMessage[i], " ");
  RXBufPtr = 0;

  // Start up the thread (in a dormant state)
  pRxThread = AfxBeginThread(RxThread, &m_rxthreadParms);
  
/*
struct SELFADDR sa;
sa.strSelfAddr = "SAM";
sa.strNetAddr = "SEEDY";
sa.m_nSlotWait = 5;
sa.parrValidChannels = new(CArray<BOOLEAN, BOOLEAN>);
sa.parrValidChannels->Add(TRUE);
sa.parrValidChannels->Add(FALSE);
sa.parrValidChannels->Add(FALSE);
Data.parrSelfAddrs->Add(sa);
sa.strSelfAddr = "SANDRA";
sa.strNetAddr = "ALGONQUIN";
sa.m_nSlotWait = 8;
sa.parrValidChannels = new(CArray<BOOLEAN, BOOLEAN>);
sa.parrValidChannels->Add(FALSE);
sa.parrValidChannels->Add(TRUE);
sa.parrValidChannels->Add(TRUE);
Data.parrSelfAddrs->Add(sa);
sa.strSelfAddr = "QUIZNO";
sa.strNetAddr = "BATCH";
sa.m_nSlotWait = 2;
sa.parrValidChannels = new(CArray<BOOLEAN, BOOLEAN>);
sa.parrValidChannels->Add(FALSE);
sa.parrValidChannels->Add(TRUE);
sa.parrValidChannels->Add(FALSE);
Data.parrSelfAddrs->Add(sa);
*/
/*
struct OTHERADDR oa;
oa.strOtherAddr = "BIRK";
oa.dReplyWait = 5.02;
oa.parrValidChannels = new(CArray<BOOLEAN, BOOLEAN>);
// SetSize
oa.parrValidChannels->Add(TRUE);
oa.parrValidChannels->Add(FALSE);
oa.parrValidChannels->Add(FALSE);
oa.parrValidChannels->Add(FALSE);
oa.pSlotStrings = new(CStringArray);
// SetSize
oa.pSlotStrings->Add("AL1");
oa.pSlotStrings->Add("AL2");
oa.pSlotStrings->Add("AL3");
oa.pSlotStrings->Add("AL4");
Data.parrOtherAddrs->Add(oa);
oa.strOtherAddr = "LEN";
oa.dReplyWait = 8.3;
oa.parrValidChannels = new(CArray<BOOLEAN, BOOLEAN>);
oa.parrValidChannels->Add(FALSE);
oa.parrValidChannels->Add(TRUE);
oa.parrValidChannels->Add(FALSE);
oa.parrValidChannels->Add(TRUE);
oa.pSlotStrings = new(CStringArray);
// SetSize
oa.pSlotStrings->Add("ALABAMA1");
oa.pSlotStrings->Add("ALABAMA2");
oa.pSlotStrings->Add("ALABAMA3");
oa.pSlotStrings->Add("ALABAMA4");
oa.pSlotStrings->Add("ALABAMA5");
oa.pSlotStrings->Add("ALABAMA6");
Data.parrOtherAddrs->Add(oa);
oa.strOtherAddr = "ANDERSONVILLE22";
oa.dReplyWait = 2;
oa.parrValidChannels = new(CArray<BOOLEAN, BOOLEAN>);
oa.parrValidChannels->Add(FALSE);
oa.parrValidChannels->Add(FALSE);
oa.parrValidChannels->Add(TRUE);
oa.parrValidChannels->Add(FALSE);
oa.pSlotStrings = new(CStringArray);
// SetSize
oa.pSlotStrings->Add("BRONCO");
oa.pSlotStrings->Add("DOLPHIN");
oa.pSlotStrings->Add("VIKING");
oa.pSlotStrings->Add("CARDINAL");
oa.pSlotStrings->Add("JET");
oa.pSlotStrings->Add("FALCON");
oa.pSlotStrings->Add("JAGUAR");
oa.pSlotStrings->Add("49ER");
Data.parrOtherAddrs->Add(oa);
*/
/*
CTime timeCur = CTime::GetCurrentTime();
struct LQA lqa;
lqa.strOtherAddr = "BRONCO";
lqa.dRxFreq = 7.879;
lqa.RxBER.value = 0;
lqa.RxBER.time = timeCur;
lqa.RxSNR.value = 3;
lqa.RxSNR.time = timeCur;
lqa.TxBER.value = 0;
lqa.TxBER.time = timeCur;
lqa.TxSINAD.value = 3;
lqa.TxSINAD.time = timeCur;
Data.parrLQAs->Add(lqa);
lqa.dRxFreq = 14.330;
lqa.RxBER.value = 4;
lqa.RxSNR.value = 5;
Data.parrLQAs->Add(lqa);
lqa.strOtherAddr = "VIKING";
lqa.dRxFreq = 8.600;
lqa.RxBER.value = 0;
lqa.RxSNR.value = 0;
Data.parrLQAs->Add(lqa);
*/
/*
struct AMDMESSAGE amd;
amd.strTo = "BOB";
amd.strFrom = "SAMUEL";
amd.strFreq = "27.534";
amd.strTime = "2/14/99 12:34";
amd.strMessage = "This is a test of the Emergency Broadcasting System.  Please do not retune your radio.";
Data.parrAMDMessages->Add(amd);
amd.strTo = "BRONCO";
amd.strFrom = "VERYLONGADDRESS";
amd.strFreq = "2.534";
amd.strTime = "7/4/99 15:34";
amd.strMessage = "Testing 1...2...3...";
Data.parrAMDMessages->Add(amd);
*/
}

CALEDoc::~CALEDoc()
{
  if (pRadio != NULL)
    delete pRadio;
}

BOOL CALEDoc::OnNewDocument()
{
  CWnd *pWnd;

	if (!CDocument::OnNewDocument())
		return FALSE;
	strFilePath.Empty();

  // Initialize any variables that need it.
  // These are default values for configuration
  pData->pConfig->eRadioType = ICOM;
  pData->pConfig->strCommPort = "COM1";
  pData->pConfig->lBaud = 9600;
  pData->pConfig->bRadioAddress = 1;

  // Let the MainFrame know that the Document exists
  pWnd = AfxGetMainWnd();
  if (pWnd != NULL)
    pWnd->PostMessage(WM_USER_DOC_STARTED, 0, 1);

	return TRUE;
}

BOOL CALEDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
  CWnd *pWnd;
  int i, scr, size;
  char scrName[16];
  struct CALEDoc::LQA lqa;
  CFile fileLQA;

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	strFilePath = lpszPathName;

  // Make up the new file name from the .ale file name, & read in.
  strFilePath = strFilePath.Left(strFilePath.GetLength() - 3) + "lqa";
  if (fileLQA.Open(strFilePath, CFile::shareExclusive | CFile::modeRead)) {
    fileLQA.Read(&size, sizeof(int));
    for (i = 0; i < size; i++) {
      fileLQA.Read(&scr, sizeof(int));
      fileLQA.Read(scrName, scr);
      scrName[scr] = '\0';
      lqa.strOtherAddr = scrName;
      fileLQA.Read(&lqa.dRxFreq, sizeof(double));
      fileLQA.Read(&lqa.RxBER, sizeof(LQAVALUE));
      fileLQA.Read(&lqa.RxSNR, sizeof(LQAVALUE));
      fileLQA.Read(&lqa.TxBER, sizeof(LQAVALUE));
      fileLQA.Read(&lqa.TxSINAD, sizeof(LQAVALUE));
      arrLQAs.Add(lqa);
    }

    fileLQA.Close();
  }

  // Let the MainFrame know that the Document exists
  pWnd = AfxGetMainWnd();
  if (pWnd != NULL)
    pWnd->PostMessage(WM_USER_DOC_STARTED, 0, 1);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CALEDoc serialization

void CALEDoc::Serialize(CArchive& ar)
{
  struct CHANNEL ch;
  struct SELFADDR sa;
  struct OTHERADDR oa;
  int size, size2, i, j, k;
  BOOLEAN bScr;

  if (ar.IsStoring()) {
    ar << stConfig.eRadioType << stConfig.bRadioAddress << 
      stConfig.strCommPort << stConfig.lBaud;

    size = arrChannels.GetSize();
    ar << size;
    for (i = 0; i < size; i++) {
      ch = arrChannels.GetAt(i);
      ar << ch.dTransFreq << (int) ch.eTransMode << ch.dRecFreq << (int) ch.eRecMode;
      ar << (int) ch.eTRMode << ch.bScanMode << ch.dSoundInterval;
    }

    size = arrSelfAddrs.GetSize();
    ar << size;
    for (i = 0; i < size; i++) {
      sa = arrSelfAddrs.GetAt(i);
      ar << sa.strSelfAddr << sa.strNetAddr << sa.m_nSlotWait << sa.bSoundingOn;
      size2 = sa.parrValidChannels->GetSize();
      ar << size2;
      for (j = 0; j < size2; j++)
        ar << sa.parrValidChannels->GetAt(j);
    }

    size = arrOtherAddrs.GetSize();
    ar << size;
    for (i = 0; i < size; i++) {
      oa = arrOtherAddrs.GetAt(i);
      ar << oa.strOtherAddr << oa.dReplyWait;
      size2 = oa.parrValidChannels->GetSize();
      ar << size2;
      for (j = 0; j < size2; j++)
        ar << oa.parrValidChannels->GetAt(j);
      oa.pSlotStrings->Serialize(ar);
    }

  } else {
    ar >> j >> stConfig.bRadioAddress >> stConfig.strCommPort >> stConfig.lBaud;
    stConfig.eRadioType = (enum RADIOTYPE) j;

    ar >> size;
    for (i = 0; i < size; i++) {
      ar >> ch.dTransFreq >> k >> ch.dRecFreq >> j;
      ch.eTransMode = (enum CRadio::RADIOMODE) k;
      ch.eRecMode = (enum CRadio::RADIOMODE) j;
      ar >> k >> ch.bScanMode >> ch.dSoundInterval;
      ch.eTRMode = (enum TRMODE) k;
      arrChannels.Add(ch);
    }

    ar >> size;
    for (i = 0; i < size; i++) {
      ar >> sa.strSelfAddr >> sa.strNetAddr >> sa.m_nSlotWait >> sa.bSoundingOn;
      sa.parrValidChannels = new(CArray< BOOLEAN, BOOLEAN >);
      ar >> size2;
      for (j = 0; j < size2; j++) {
        ar >> bScr;
        sa.parrValidChannels->Add(bScr);
      }
      arrSelfAddrs.Add(sa);
    }

    ar >> size;
    for (i = 0; i < size; i++) {
      ar >> oa.strOtherAddr >> oa.dReplyWait;
      oa.parrValidChannels = new(CArray< BOOLEAN, BOOLEAN >);
      ar >> size2;
      for (j = 0; j < size2; j++) {
        ar >> bScr;
        oa.parrValidChannels->Add(bScr);
      }
      oa.pSlotStrings = new CStringArray;
      oa.pSlotStrings->Serialize(ar);
      arrOtherAddrs.Add(oa);
    }
	}
}

/////////////////////////////////////////////////////////////////////////////
// CALEDoc diagnostics

#ifdef _DEBUG
void CALEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CALEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CALEDoc commands

void CALEDoc::DeleteContents()
{
  const int MAX_LQA_RECORDS = 500;  // The maximum number of LQA records that
                                    // are saved to file.
  int i, scr, size;
  struct CALEDoc::SELFADDR sa;
  struct CALEDoc::OTHERADDR oa;
  struct CALEDoc::LQA lqa;
  CFile fileLQA;

  // If there is data in the LQA array, and there is a file to write to,
  // write out the LQA data to file
  if (((size = arrLQAs.GetSize()) > 0) && !strFilePath.IsEmpty()) {
    // Make up the new file name from the .ale file name, & write out.
    // But only keep the most recent MAX_LQA_RECORDS entries
    strFilePath = strFilePath.Left(strFilePath.GetLength() - 3) + "lqa";
    if (fileLQA.Open(strFilePath, CFile::modeCreate | CFile::shareExclusive |
        CFile::modeReadWrite)) {
      fileLQA.Write(&size, sizeof(int));
      for (i = (size > MAX_LQA_RECORDS) ? size - MAX_LQA_RECORDS : 0; i < size; i++) {
        lqa = arrLQAs.GetAt(i);
        scr = lqa.strOtherAddr.GetLength();
        fileLQA.Write(&scr, sizeof(int));
        fileLQA.Write(lqa.strOtherAddr, scr);
        fileLQA.Write(&lqa.dRxFreq, sizeof(double));
        fileLQA.Write(&lqa.RxBER, sizeof(LQAVALUE));
        fileLQA.Write(&lqa.RxSNR, sizeof(LQAVALUE));
        fileLQA.Write(&lqa.TxBER, sizeof(LQAVALUE));
        fileLQA.Write(&lqa.TxSINAD, sizeof(LQAVALUE));
      }
      fileLQA.Close();
    }

    strFilePath.Empty();
  }

  if (size > 0)
    arrLQAs.RemoveAll();

  arrChannels.RemoveAll();

  size = arrSelfAddrs.GetSize();
  for (i = size - 1; i >= 0; i--) {
    sa = arrSelfAddrs.GetAt(i);
    delete sa.parrValidChannels;
    arrSelfAddrs.RemoveAt(i);
  }

  size = arrOtherAddrs.GetSize();
  for (i = size - 1; i >= 0; i--) {
    oa = arrOtherAddrs.GetAt(i);
    delete oa.parrValidChannels;
    delete oa.pSlotStrings;
    arrOtherAddrs.RemoveAt(i);
  }

  SetModifiedFlag(FALSE);

	CDocument::DeleteContents();
}

BOOL CALEDoc::InitializeRadio()
{
  // Initialize the serial port on the computer, allocate a radio object, and
  // initialize the actual radio.

  CString str;
  DCB dcb;

  // Do the serial port first
  if (hComm != 0)
    CloseHandle(hComm);
  if ((hComm = CreateFile(stConfig.strCommPort, GENERIC_READ | GENERIC_WRITE,
    0, 0, OPEN_EXISTING, 0, 0)) == INVALID_HANDLE_VALUE) {
    MessageBox(NULL, "The computer Serial Comm Port to the radio is busy.", "Alert", MB_OK);
    hComm = 0;
    return FALSE;
  }

  FillMemory(&dcb, sizeof(dcb), 0);
  dcb.DCBlength = sizeof(dcb);
  str.Format("%ld,n,8,1", stConfig.lBaud);
  if (!BuildCommDCB(str, &dcb)) {
    MessageBox(NULL, "Couldn't build the serial port DCB.", "Alert", MB_OK);
    return FALSE;
  }
  SetCommState(hComm, &dcb);

  // Allocate a radio object
  if (pRadio != NULL)
    delete pRadio;
  if (stConfig.eRadioType == ICOM)
    pRadio = new CIcom(hComm);

  // Initialize the radio to a known state
  pRadio->Initialize(stConfig.bRadioAddress);
  
  m_bRadioInitialized = TRUE;
  return TRUE;
}

CString CALEDoc::ParseToAddr()
  // Parses the 'to' address from a message.  Abbreviated 'to' addresses are ignored.
{
  CString str("");
  long lALE, lWordType;
  TCHAR ch;

  // Skip any abbreviated TO addresses
  for (iParsePtr = 0; iParsePtr < indMessageBuf; iParsePtr++) {
    lALE = lMessageBuf[iParsePtr].lALE;
    lWordType = lALE & 0xe00000;
    if ((lWordType == _TO) && (lALE != lMessageBuf[iParsePtr + 1].lALE) && ASCII_OK(lALE, ASCII_38))
      goto next;
  }
  // If we fall through the loop, there is no complete TO address.
  // Return iParsePtr to 0 because not all messages require a TO address (sounds).
  iParsePtr = 0;
  return str;

next:
  iParsePtr++;
  // First address word
  str = (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Second address word
  if (iParsePtr >= indMessageBuf)
    return str;
  lALE = lMessageBuf[iParsePtr].lALE;
  if (((lALE & 0xe00000) != _DATA) || !ASCII_OK(lALE, ASCII_38)) {
    return str;
  }
  iParsePtr++;
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Third address word
  if (iParsePtr >= indMessageBuf)
    return str;
  lALE = lMessageBuf[iParsePtr].lALE;
  if (((lALE & 0xe00000) != _REP) || !ASCII_OK(lALE, ASCII_38)) {
    return str;
  }
  iParsePtr++;
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Fourth address word
  if (iParsePtr >= indMessageBuf)
    return str;
  lALE = lMessageBuf[iParsePtr].lALE;
  if (((lALE & 0xe00000) != _DATA) || !ASCII_OK(lALE, ASCII_38)) {
    return str;
  }
  iParsePtr++;
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Fifth address word
  if (iParsePtr >= indMessageBuf)
    return str;
  lALE = lMessageBuf[iParsePtr].lALE;
  if (((lALE & 0xe00000) != _REP) || !ASCII_OK(lALE, ASCII_38)) {
    return str;
  }
  iParsePtr++;
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;
  return str;
}

CString CALEDoc::ParseAMD(BOOL *bAMDPresent)
{
  CString str("");
  int i;
  long lALE;

  // Find a COMMAND ALE word with AMD characters
  for (i = iParsePtr; i < indMessageBuf; i++) {
    lALE = lMessageBuf[i].lALE;
    if (((lALE & 0xe00000) == _CMD) && ASCII_OK(lALE, ASCII_64))
      goto next;
  }
  // If we fall through the loop, there is no AMD
  *bAMDPresent = FALSE;
  return str;

next:
  *bAMDPresent = TRUE;
  iParsePtr = ++i;

  // First AMD word
  str = (lALE & 0x1fc000) >> 14;
  str += (lALE & 0x3f80) >> 7;
  str += lALE & 0x7F;

  while (iParsePtr < indMessageBuf) {
    // Next AMD word (DATA)
    lALE = lMessageBuf[iParsePtr].lALE;
    if (((lALE & 0xe00000) != _DATA) || !ASCII_OK(lALE, ASCII_64)) {
      // This isn't a valid AMD word
      break;
    }

    iParsePtr++;
    str += (lALE & 0x1fc000) >> 14;
    str += (lALE & 0x3f80) >> 7;
    str += lALE & 0x7F;

    // Next AMD word (REPEAT)
    if (iParsePtr >= indMessageBuf)
      break;

    lALE = lMessageBuf[iParsePtr].lALE;
    if (((lALE & 0xe00000) != _REP) || !ASCII_OK(lALE, ASCII_64)) {
      // This isn't a valid AMD word
      break;
    }

    iParsePtr++;
    str += (lALE & 0x1fc000) >> 14;
    str += (lALE & 0x3f80) >> 7;
    str += lALE & 0x7F;
  }

  str.TrimRight();
  return str;
}

CString CALEDoc::ParseFromAddr(BOOL *bTis)
  // Parses the from address from a message.  Note that indMessageBuf, the number of ALE
  // words in the message buffer, may be adjusted if the parsing determines that there
  // are bad words at the end of the buffer.
  // bTis is a returned value.  It is TRUE if the from address is a THIS IS word type.
{
  CString str("");
  int i;
  long lALE, lWordType;
  TCHAR ch;

  // Skip to TIS or TWAS
  for (i = iParsePtr; i < indMessageBuf; i++) {
    lALE = lMessageBuf[i].lALE;
    lWordType = lALE & 0xe00000;
    if (((lWordType == _TIS) || (lWordType == _TWAS)) && ASCII_OK(lALE, ASCII_38))
      goto next;
  }
  // If we fall through the loop, the message is incomplete.
  return str;

next:
  if (lWordType == _TIS)
    *bTis = TRUE;
  else
    *bTis = FALSE;

  // First address word
  str = (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Second address word
  if (++i >= indMessageBuf)
    return str;
  lALE = lMessageBuf[i++].lALE;
  if (((lALE & 0xe00000) != _DATA) || !ASCII_OK(lALE, ASCII_38)) {
    // This isn't a valid word, assume junk and truncate the message
    indMessageBuf = i - 1;
    return str;
  }
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Third address word
  if (i >= indMessageBuf)
    return str;
  lALE = lMessageBuf[i++].lALE;
  if (((lALE & 0xe00000) != _REP) || !ASCII_OK(lALE, ASCII_38)) {
    // This isn't a valid word, assume junk and truncate the message
    indMessageBuf = i - 1;
    return str;
  }
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Fourth address word
  if (i >= indMessageBuf)
    return str;
  lALE = lMessageBuf[i++].lALE;
  if (((lALE & 0xe00000) != _DATA) || !ASCII_OK(lALE, ASCII_38)) {
    // This isn't a valid word, assume junk and truncate the message
    indMessageBuf = i - 1;
    return str;
  }
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;

  // Fifth address word
  if (i >= indMessageBuf)
    return str;
  lALE = lMessageBuf[i++].lALE;
  if (((lALE & 0xe00000) != _REP) || !ASCII_OK(lALE, ASCII_38)) {
    // This isn't a valid word, assume junk and truncate the message
    indMessageBuf = i - 1;
    return str;
  }
  str += (lALE & 0x1fc000) >> 14;
  if ((ch = ((lALE & 0x3f80) >> 7)) == '@')
    return str;
  str += ch;
  if ((ch = (lALE & 0x7F)) == '@')
    return str;
  str += ch;
  return str;
}

void CALEDoc::HandleAMD(CString strToAddr, CString strFromAddr, CString strAMD)
{
  int i;
  double dFreq;
  CTime timeCur;
  struct AMDMESSAGE amd;
  POSITION pos;
  CALEView* pView;

  // Add a new entry to the AMDMESSAGE table.
  amd.strFrom = strFromAddr;
  if (strToAddr == "@?")
    amd.strTo = "ALLCALL";
  else
    amd.strTo = strToAddr;
  amd.strMessage = strAMD;
  dFreq = pRadio->GetFreq();
  amd.strFreq.Format("%lf", dFreq);
  timeCur = CTime::GetCurrentTime();
  amd.strTime = timeCur.Format("%m/%d/%y %H:%M");
  i = arrAMDMessages.Add(amd);

  // Check if the AMD dialog box is displayed
  pos = GetFirstViewPosition();
  pView = (CALEView *) GetNextView(pos);
  if (pView->pAMDDlg == NULL) {
    // It's not displayed, so display it
//    pView->OnViewAmd();
    pView->pAMDDlg = new CAMDDisplay;

    pView->pAMDDlg->pData = pData;
    pView->pAMDDlg->view = pView;
    pView->pAMDDlg->Create(IDD_AMD, pView);
  } else
    // It is displayed, so update the AMD view
    UpdateAllViews(NULL, 3L, (CObject *) i);
}

void CALEDoc::LogLQA(CString strAddr)
{
  int i, size;
  long lTotBER = 0;
  CTime timeCur;
  double dFreq, dTotSNR = 0.0;
  struct LQA lqa;

  // Calculate the overall BER and SNR for the message.
  
  for (i = 0; i < indMessageBuf; i++) {
    lTotBER += lMessageBuf[i].Ucount;
    dTotSNR += lMessageBuf[i].dSNR;
  }
  lTotBER /= indMessageBuf;
  dTotSNR /= indMessageBuf;

  // Find the entry in the LQA table (if there is one)
  dFreq = pRadio->GetFreq();
  timeCur = CTime::GetCurrentTime();
  size = arrLQAs.GetSize();
  for (i = 0; i < size; i++) {
    lqa = arrLQAs.GetAt(i);
    if ((lqa.dRxFreq == dFreq) && (lqa.strOtherAddr == strAddr)) {
      lqa.RxSNR.value = (int) (dTotSNR + 0.5);
      lqa.RxSNR.time = timeCur;
      lqa.RxBER.value = lTotBER;
      lqa.RxBER.time = timeCur;
      // Remove the old entry and add the new one at the end.  This
      // will keep the list sorted by time.
      arrLQAs.RemoveAt(i);
      i = arrLQAs.Add(lqa);

      // Update the LQA dialog box
      UpdateAllViews(NULL, 1L, (CObject *) i);

      return;
    }
  }

  // If we fall through the loop, the station/freq can't be found so
  // add a new entry to the LQA table.
  lqa.dRxFreq = dFreq;
  lqa.strOtherAddr = strAddr;
  lqa.RxSNR.value = (int) (dTotSNR + 0.5);
  lqa.RxSNR.time = timeCur;
  lqa.RxBER.value = lTotBER;
  lqa.RxBER.time = timeCur;
  lqa.TxSINAD.value = 31;
  lqa.TxSINAD.time = timeCur;
  lqa.TxBER.value = 31;
  lqa.TxBER.time = timeCur;
  i = arrLQAs.Add(lqa);

  // Update the LQA view
  UpdateAllViews(NULL, 2L, (CObject *) i);
}

BOOL CALEDoc::MessageForUs(CString strToAddr)
// If the address passed is one of the our Self Addresses,
// MessageForUs passes back TRUE.
{
  struct SELFADDR sa;
  int i, len;

  len = pData->parrSelfAddrs->GetSize();
  for (i = 0; i < len; i++) {
    sa = pData->parrSelfAddrs->GetAt(i);
    if (sa.strSelfAddr == strToAddr)
      return TRUE;
  }
  return FALSE;
}

void CALEDoc::SendResponse(CString strFromAddr, CString strToAddr)
// When another station attempts a connection to us, SendResponse will
// send the appropriate response.
{
  struct OTHERADDR oa;
  struct CHANNEL ch;
  enum CRadio::RADIOMODE eCurMode;
  int i, j, len1, len2, iToWords, iAddrWords;
  double dCurFreq;

  // Check if this is an Other Address that is in our data.
  // (We may need to change freq/mode to respond.)
  dCurFreq = pRadio->GetFreq();
  eCurMode = pRadio->GetMode();
  len1 = pData->parrOtherAddrs->GetSize();
  len2 = pData->parrChannels->GetSize();
  for (i = 0; i < len1; i++) {
    oa = pData->parrOtherAddrs->GetAt(i);
    if (oa.strOtherAddr == strToAddr) {
      // Check for a channel that matches the current channel
      for (j = 0; j < len2; j++)
        if (oa.parrValidChannels->GetAt(j)) {
          ch = pData->parrChannels->GetAt(j);
          if (ch.dRecFreq == dCurFreq) {
            pRadio->Tune(ch.dTransFreq);
            pRadio->SetMode(ch.eTransMode);
            sResponseData.iChannelNumber = j;
            goto next;
          }
        }
    }
  }

next:
  // If we fall through, the 'to' address is not in our list, so
  // use the current freq and mode to respond
  // Write to the status bar
  dCurFreq = pRadio->GetFreq();
  strStatus.Format("Responding to connection attempt from %s on %lf", strToAddr, dCurFreq);

  // Make up the TO part of the response
  iToWords = MakeAddrWords(_TO, strToAddr, lTransBuf);
  iToWords = MakeAddrWords(_TO, strToAddr, lTransBuf + iToWords);

  // Add the From part of the response and transmit
  iAddrWords = MakeAddrWords(_TIS, strFromAddr, lTransBuf + 2 * iToWords);
  Transmit(2 * iToWords + iAddrWords, 20);

  // Wait for the transmission to end
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

  // Set up the receiver for the acknowledgment
  pRadio->Tune(dCurFreq);
  pRadio->SetMode(eCurMode);
  OnReceive(376, 20);  // 188 = Wait 1.50 s.
}

void CALEDoc::SendAcknowledgement()
  // When attempting a connection to another station, after the other
  // station responds, SendAcknowledgement will send the
  // acknowledgement message.
{
  int iToWords, iAddrWords;
  double dFreq;
  CString strToAddr, strFromAddr;
  struct SELFADDR sa;
  struct OTHERADDR oa;
  struct CHANNEL ch;

  sa = arrSelfAddrs.GetAt(sChannelPriorities.iSelfAddr);
  oa = arrOtherAddrs.GetAt(sChannelPriorities.iOtherAddr);
  strToAddr = oa.strOtherAddr;
  strFromAddr = sa.strSelfAddr;
  ch = arrChannels.GetAt(sChannelPriorities.arrChannels[sChannelPriorities.iNextChannel - 1]);

  // Write to the status bar
  dFreq = ch.dTransFreq;
  strStatus.Format("Sending acknowledgement for connection attempt to %s on %f", strToAddr, dFreq);

  pRadio->Tune(dFreq);
  pRadio->SetMode(ch.eTransMode);

  // Make up the TO part of the response
  iToWords = MakeAddrWords(_TO, strToAddr, lTransBuf);
  iToWords = MakeAddrWords(_TO, strToAddr, lTransBuf + iToWords);

  // Add the From part of the response and transmit
  iAddrWords = MakeAddrWords(_TIS, strFromAddr, lTransBuf + 2 * iToWords);
  Transmit(2 * iToWords + iAddrWords, 30);

  // Wait for the transmission to end
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

  // Notify the user
  strStatus.Format("Connected to %s on %f", strToAddr, dFreq);
  // Save the TO and FROM addresses in sResponseData so they are in
  // a common location no matter who initiated the connection
  sResponseData.strToAddr = strFromAddr;
  sResponseData.strFromAddr = strToAddr;
  sResponseData.iChannelNumber = sChannelPriorities.arrChannels[sChannelPriorities.iNextChannel - 1];

  // Set up the receiver for any AMD's or break connections
  pRadio->Tune(ch.dRecFreq);
  pRadio->SetMode(ch.eRecMode);
  OnReceive(100000, 30);
}

void CALEDoc::ParseMessage()
{
  CString strFromAddr, strToAddr, strAMD, str;
  BOOL bTis, bAMDPresent;
  struct SELFADDR sa;
  struct OTHERADDR oa;
  struct CHANNEL ch;
  int len, i;
  double dCurFreq;

  // Parse the To Address
  strToAddr = ParseToAddr();
  // Parse any AMD message
  strAMD = ParseAMD(&bAMDPresent);
  // Parse the From Address
  strFromAddr = ParseFromAddr(&bTis);

  // If there isn't a valid address, the message is incomplete,
  // and LQA can't be recorded
  if (strFromAddr != "") {
    LogLQA(strFromAddr);

    // Check for an Allcall.  (ParseToAddr will return @? for an @?@ address.)
    if (strToAddr == "@?") {
      if (strAMD != "")
        HandleAMD(strToAddr, strFromAddr, strAMD);
      // Stop on channel and wait for an audio message
      str.Format("ALERT!! Allcall received on %f from %s.\nPress OK to resume.",
        pRadio->GetFreq(), strFromAddr);
      MessageBox(NULL, str, "Alert", MB_OK);
    } else {
      // See if the message has one of our self addresses
      if (MessageForUs(strToAddr)) {
        // Yup, it's for us.  Any AMD enclosed?
        if (strAMD != "")
          HandleAMD(strToAddr, strFromAddr, strAMD);

        // Depending on what state we are in, take care of the message
        switch (iCurrentPriority) {
          case 30:  // Connected
            // Check for a break connection message.
            if (!bTis && (sResponseData.strFromAddr == strFromAddr) &&
               (sResponseData.strToAddr == strToAddr)) {
              // Notify the user
              strStatus.Format("Connection Ended");
              pStatusBar->SetPaneText(1, strStatus, TRUE);

              iCurrentPriority = 0;
              NextAction();
            }
            break;
          case 25:  // Connection attempt
            // Look for a response
            sa = arrSelfAddrs.GetAt(sChannelPriorities.iSelfAddr);
            oa = arrOtherAddrs.GetAt(sChannelPriorities.iOtherAddr);
            if ((strToAddr == sa.strSelfAddr) && (strFromAddr == oa.strOtherAddr))
              if (bTis) {
                // Send the connection acknowledgment and change status
                AfxGetMainWnd()->PostMessage(WM_USER_ACTION, 27, NULL);
                return;
              }
            break;
          case 20:  // Someone outside is attempting connection to us
            // Look for the connection acknowledgment
            if ((strToAddr == sResponseData.strToAddr) &&
                (strFromAddr == sResponseData.strFromAddr) && bTis) {
              // We are connected, notify the user and set up to receive
              strStatus.Format("Connected to %s on %f", strFromAddr, pRadio->GetFreq());
              OnReceive(100000, 30);
              return;
            }
            break;
          case 15:  // Sound
          case 10:  // Scan
          case 5:   // Single channel
            // Is it a connection attempt?
            if (bTis) {
              // Yes, first check the channel list and make sure that transmissions are allowed.
              dCurFreq = pRadio->GetFreq();
              len = arrChannels.GetSize();
              for (i = 0; i < len; i++) {
                ch = arrChannels.GetAt(i);
                if ((ch.dRecFreq == dCurFreq) && (ch.eTRMode == TR)) {
                  // Transmitting OK. Send back a response.
                  sResponseData.strFromAddr = strFromAddr;
                  sResponseData.strToAddr = strToAddr;
                  AfxGetMainWnd()->PostMessage(WM_USER_ACTION, 20, NULL);
                  return;
                }
              }
            }

            break;
        }
      }
    }
  }

  // If we haven't started a following action yet, call NextAction 
  // to see if one is needed
  NextAction();
}

void CALEDoc::NextAction()
  // Whenever any higher priority action is complete, 
  // NextAction is called to see if there is a continuing action that
  // should be restarted.  Continuing actions include attempting a
  // multichannel connection, sounding, single channel reception or scanning.
{
  int size;
  struct CHANNEL ch;

  // If we are connected, receive on the current channel.
  if (iCurrentPriority == 30) {
    OnReceive(100000, 30);
    return;
  }

  // Check if we are currently trying to make a connection.
  // If so, try on the next channel (if any)
  if (iCurrentPriority == 25) {
    DoConnection();
    return;
  }

  // Is sounding on and has it been interrupted?
  if (bSoundingInterrupted) {
    DoSound();
    return;
  }

  if (m_rxthreadParms.eRxMode == SINGLE_CHANNEL) {
    // Restart the receive under the selected channel
    iCurrentPriority = 0;
    AfxGetMainWnd()->PostMessage(WM_USER_ACTION, 5, iSingleChannel);
    return;
  }

  // The only other case to handle here is scanning
  // Keep track of which channel to scan next and call OnModemAction
  // with the next channel
  if (m_rxthreadParms.eRxMode != SCANNING)
    return;

  iCurrentPriority = 0;
  size = pData->parrChannels->GetSize();
  do {
    if (++m_rxthreadParms.iScanChannel >= size)
      m_rxthreadParms.iScanChannel = 0;
    ch = pData->parrChannels->GetAt(m_rxthreadParms.iScanChannel);
  } while (!ch.bScanMode);

  AfxGetMainWnd()->PostMessage(WM_USER_ACTION, 10, m_rxthreadParms.iScanChannel);
}

void CALEDoc::DataAvail()
// DataAvail gets called (via a message) anytime the RxThread has an
// ALE word to pass back.
{
  // process data
  while (m_CircBufI.Get(&RXBuffer, 1) > 0) {
    if (RXBuffer.ALEWord == -1) {
      // DeGolay decode failure
      if (bDebugMode) {
        sprintf(szRXMessage[RXBufPtr], " %02d DeGolay decode failure", RXBufPtr);
        RXBufPtr = ++RXBufPtr >= 20 ? 0 : RXBufPtr;
      }

      // Reception has been stopped in MakeALEWord
      if (!RXBuffer.bALEDetected) {
        // This should only occur when scanning on the first reception
        NextAction();
        goto update;
      }

      if (indMessageBuf > 0)
        ParseMessage();
      else
        NextAction();
      break;
    } else if (RXBuffer.ALEWord == -3) {
      // Sync failure
      if (bDebugMode) {
        sprintf(szRXMessage[RXBufPtr], " %02d Sync failure", RXBufPtr);
        RXBufPtr = ++RXBufPtr >= 20 ? 0 : RXBufPtr;
      }

      NextAction();
      goto update;
    } else if (RXBuffer.ALEWord == -10) {
      // No ALE detected
      if (bDebugMode) {
        sprintf(szRXMessage[RXBufPtr], " %02d No ALE detected", RXBufPtr);
        RXBufPtr = ++RXBufPtr >= 20 ? 0 : RXBufPtr;
      }

      // Reception has been stopped in MakeALEWord
      // This should only occur when scanning on the first reception
      NextAction();
      goto update;

    } else {
      if (bDebugMode) {
        ASSERT((RXBuffer.dSNR > 0.0) && (RXBuffer.dSNR < 33.0));
        ASSERT((RXBuffer.Ucount > 0) && (RXBuffer.Ucount <= 48));
        sprintf(szRXMessage[RXBufPtr], " %02d  %d%c%c%c    %3.1lf  %d  %d",
          RXBufPtr,
          RXBuffer.ALEWord >> 21 & 7,
          (char) (RXBuffer.ALEWord >> 14 & 0x7F),
          (char) (RXBuffer.ALEWord >> 7 & 0x7F),
          (char) (RXBuffer.ALEWord & 0x7F),
          RXBuffer.dSNR,
          RXBuffer.Ucount,
          RXBuffer.bALEEnded);
        RXBufPtr = ++RXBufPtr >= 20 ? 0 : RXBufPtr;
      }

      lMessageBuf[indMessageBuf].lALE = RXBuffer.ALEWord;
      lMessageBuf[indMessageBuf].Ucount = RXBuffer.Ucount;
      lMessageBuf[indMessageBuf++].dSNR = RXBuffer.dSNR;

      if (RXBuffer.bALEEnded) {
        // An end-of-transmission has been detected and reception stopped.
        if (indMessageBuf > 0)
          ParseMessage();
        else
          NextAction();
        break;
      }
    }

  }  // while

update:
  // If Modem error, write a message to status bar
  if (m_rxthreadParms.hrModemError) {
    strStatus.Format("Modem Error Number %d occurred!", m_rxthreadParms.hrModemError);
    pStatusBar->SetPaneText(1, strStatus, TRUE);
  }

  // update views
  if (bDebugMode)
    UpdateAllViews(NULL, 0L, NULL);
}

void CALEDoc::OnReceive(LONG dwell, int priority)
{
  if (::WaitForSingleObject(m_workingEvent.m_hObject, 0) == WAIT_OBJECT_0)
    if (m_rxthreadParms.m_bReceiving && priority > iCurrentPriority) {
      m_workingEvent.ResetEvent();
    }
  iCurrentPriority = priority;

  // If the thread was in the process of stopping, we need to wait
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

  // Set any parameters for the RxThread
  // Initialize the CWnd class
  m_rxthreadParms.pWnd = AfxGetMainWnd();
  indMessageBuf = 0;
  m_rxthreadParms.hrModemError = 0;
  m_rxthreadParms.dwell = dwell;
  m_rxthreadParms.iPriority = priority;

  // Update the status bar
  pStatusBar->SetPaneText(1, strStatus, TRUE);

  m_rxthreadParms.m_bReceiving = TRUE;
  // Set the buffer to empty
  m_CircBufI.Reset();

  m_workingEvent.SetEvent();
}

BOOL CALEDoc::CheckChannelClear(int priority)
  // Before transmitting, the channel must be checked for any current traffic.
  // The receiver thread returns the m_bChannelClear indication.
{
  if (::WaitForSingleObject(m_workingEvent.m_hObject, 0) == WAIT_OBJECT_0)
    if (m_rxthreadParms.m_bReceiving && priority > iCurrentPriority) {
      m_workingEvent.ResetEvent();
    }
  iCurrentPriority = priority;

  // If the thread was in the process of stopping, we need to wait
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

  // Set any parameters for the RxThread
  // Initialize the CWnd class
  m_rxthreadParms.pWnd = AfxGetMainWnd();
  m_rxthreadParms.hrModemError = 0;
  m_rxthreadParms.m_bSignalCheck = TRUE;

  m_rxthreadParms.m_bChannelClear = TRUE;
  m_rxthreadParms.m_bReceiving = TRUE;
  m_stoppedEvent.ResetEvent();
  m_workingEvent.SetEvent();
  ::Sleep(0);

  // Wait until the thread is done
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);
  m_rxthreadParms.m_bSignalCheck = FALSE;
  return m_rxthreadParms.m_bChannelClear;
}

/*
void CALEDoc::OnTransmitDebug()
// This is for development purposes only.  Delete when Menu option deleted
{
#define _SAM	005160315
#define _JOE	004523705
#define _BOB  004123702

  if (::WaitForSingleObject(m_workingEvent.m_hObject, 0) == WAIT_OBJECT_0)
    if (m_rxthreadParms.m_bReceiving && (15 > iCurrentPriority)) {
      m_workingEvent.ResetEvent();
    }
  iCurrentPriority = 15;

  // If the thread was in the process of stopping, we need to wait
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);
  m_rxthreadParms.m_bReceiving = FALSE;

  // Initialize the CWnd class
  m_rxthreadParms.pWnd = AfxGetMainWnd();
  m_rxthreadParms.hrModemError = 0;

  // Fill the buffer with data to transmit

  int ret;
//  static int len = 4;

//  long iTestBuf[100];
  ::Sleep(30);
  m_CircBufO.Reset();

//  iTestBuf[1] = iTestBuf[2] = iTestBuf[3] = iTestBuf[0] = _TO + _BOB;
//  iTestBuf[4] = iTestBuf[5] = iTestBuf[6] = iTestBuf[7] = _TO + _BOB;
//  iTestBuf[8] = iTestBuf[9] = iTestBuf[10] = iTestBuf[11] = _TO + _BOB;
//  iTestBuf[12] = iTestBuf[13] = iTestBuf[14] = iTestBuf[15] = _TO + _BOB;
//  iTestBuf[16] = iTestBuf[17] = iTestBuf[18] = iTestBuf[19] = _TO + _BOB;

int i, Ts = 3, iToWords, iAddrWords;
CString str = "WILLIAM";
CString strAddr = "BILL";
// Make up the scan call part of the calling cycle
lTransBuf[0] = _TO | ThreeChar(str);
for (i = 1; i < Ts; i++)
  lTransBuf[i] = lTransBuf[0];

// Add the leading call part of the calling cycle
iToWords = MakeAddrWords(_TO, str, lTransBuf + Ts);
iToWords = MakeAddrWords(_TO, str, lTransBuf + Ts + iToWords);

iAddrWords = MakeAddrWords(_TIS, strAddr, lTransBuf + Ts + 2 * iToWords);

ret = m_CircBufO.Put(lTransBuf, iAddrWords + Ts + 2 * iToWords);
//  ret = m_CircBufO.Put(iTestBuf, len);
//  len = (len += 4) > 20 ? 4 : len;

  m_workingEvent.SetEvent();
  ::Sleep(0);

  // Wait for the transmission to end
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

  iCurrentPriority = 0;
  NextAction();
}
*/

void CALEDoc::Transmit(LONG BufSize, int priority)
{
  if (::WaitForSingleObject(m_workingEvent.m_hObject, 0) == WAIT_OBJECT_0)
    if (m_rxthreadParms.m_bReceiving && priority > iCurrentPriority) {
      m_workingEvent.ResetEvent();
    }
  iCurrentPriority = priority;

  // If the thread was in the process of stopping, we need to wait
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);
  m_rxthreadParms.m_bReceiving = FALSE;

  // Initialize the CWnd class
  m_rxthreadParms.pWnd = AfxGetMainWnd();
  m_rxthreadParms.hrModemError = 0;

  // Update the status bar
  pStatusBar->SetPaneText(1, strStatus, TRUE);

  // Fill the buffer with data to transmit
  m_CircBufO.Reset();
  int ret = m_CircBufO.Put(lTransBuf, BufSize);

  m_workingEvent.SetEvent();
  ::Sleep(0);
}

long CALEDoc::ThreeChar(CString str)
// returns an ALE word containing the first three characters in the
// string argument (pads with @ characters if the string is too short)
{
  CString Addr;

  Addr = str + CString("@@@");
  return (Addr[0] << 14) | (Addr[1] << 7) | Addr[2];
}

int CALEDoc::MakeAddrWords(const int iWordType, const CString strAddr, long ALEWords[])
// MakeAddrWords takes the ALE address (1-15 chars) in strAddr and converts
// it to ALE words (1 to 5 words) making an ALE address.  The ALE address
// type (TO, TWAS, TIS) is appended to the first ALE word.
// The returned value is the number of ALE Words used.
//
// MakeAddrWords is also called to make up AMD Messages.  iWordType is
// COMMAND in this case, but the messages are otherwise similar to the
// address ALE words.  The exception is that addresses are filled with
// the '@' character, and messages are filled with the ' ' character.
{
  int len, lenStr, i;
  CString strTemp;

  lenStr = strAddr.GetLength();
  len = (lenStr + 2) / 3;
  ALEWords[0] = iWordType | ThreeChar(strAddr);
  strTemp = strAddr.Right(lenStr -= 3);
  i = 1;
  while (lenStr > 0) {
    ALEWords[i++] = _DATA | ThreeChar(strTemp);
    strTemp = strTemp.Right(lenStr -= 3);
    if (lenStr > 0) {
      ALEWords[i++] = _REP | ThreeChar(strTemp);
      strTemp = strTemp.Right(lenStr -= 3);
    }
  }

  if (iWordType == _CMD) {
    i--;
    // Convert any trailing '@' chars to ' '.
    if (lenStr == -2)
      // There should be two trailing blanks.
      ALEWords[i] = ALEWords[i] & 0xFFC000 | 0x1020;
    else if (lenStr == -1)
      // There should be one trailing blank.
      ALEWords[i] = ALEWords[i] & 0xFFFF80 | 0x20;
  }

  return len;
}

void CALEDoc::DoSound()
{
  CTime timeCur;
  CTimeSpan scrSpan;
  double dSoundInterval, dFreq;
  int Ts;  // The scan period Time in ALE word times
  int min, sec, reps, iChanCnt, iAddrWords, i, j;
  struct SOUNDENTRY scrSound;
  struct CALEDoc::SELFADDR sa;
  POSITION pos;
  CString strAddr;

  sa = pData->parrSelfAddrs->GetAt(NextSound.iSelfAddr);
  strAddr = sa.strSelfAddr;
  // Update the status bar (the transmit routine actually updates status
  dFreq = pData->parrChannels->GetAt(NextSound.iChannel).dTransFreq;
  strStatus.Format("Sounding %s at %f", strAddr, dFreq);

  // Do the sound
  pRadio->Tune(dFreq);
  pRadio->SetMode(pData->parrChannels->GetAt(NextSound.iChannel).eTransMode);
  // The scan period (Ts) is the number of channels * the maximum time for a
  // single channel (784 ms).  Note that 784ms is the time for 2 ALE words.
  iChanCnt = 0;
  j = sa.parrValidChannels->GetSize();
  for (i = 0; i < j; i++)
    if (sa.parrValidChannels->GetAt(i))
      iChanCnt++;
  Ts = iChanCnt * 2;  // In ALE word times

  iAddrWords = MakeAddrWords(_TWAS, strAddr, lTransBuf);

  // Calculate the number of times the address must be repeated
  // and duplicate the address
  reps = 2 + (Ts + iAddrWords - 1) / iAddrWords;
  for (i = iAddrWords; i < iAddrWords * reps; i += iAddrWords)
    for (j = 0; j < iAddrWords; j++)
      lTransBuf[i + j] = lTransBuf[j];

  // Check channel clear
  if (CheckChannelClear(15)) {
    // and transmit the sound
    Transmit(reps * iAddrWords, 15);

    // Set the time for this channel's next sound
    timeCur = CTime::GetCurrentTime();
    dSoundInterval = pData->parrChannels->GetAt(NextSound.iChannel).dSoundInterval;
    min = (int) dSoundInterval;
    sec = (int) ((dSoundInterval - min) * 60.0);
    CTimeSpan timeSoundInt(0, 0, min, sec);
    NextSound.TimeSound = timeCur + timeSoundInt;
  } else {
    // Channel is busy, try again in 10 minutes
    timeCur = CTime::GetCurrentTime();
    CTimeSpan timeSoundInt(0, 0, 10, 0);
    NextSound.TimeSound = timeCur + timeSoundInt;
  }

  // Put the entry back on the linked list (sorted by time)
  if ((pos = listSounds.GetTailPosition()) == NULL) {
    // The list is empty, so just add the single entry
    listSounds.AddHead(NextSound);
  } else {
    do {
      scrSound = listSounds.GetPrev(pos);
      if (NextSound.TimeSound >= scrSound.TimeSound) {
        // It would have been better if the CList routines would have
        // returned the head element when GetNext was called with NULL.
        // This would have allowed back and forth searching like I am
        // doing.  But GetNext will ASSERT, thus the following kluge.
        if (pos == NULL) {
          pos = listSounds.GetHeadPosition();
        } else {
          listSounds.GetNext(pos);
          listSounds.InsertAfter(pos, NextSound);
        }
        break;
      }
      if (pos == NULL) {
        listSounds.AddHead(NextSound);
        break;
      }
    } while (TRUE);
  }

  // Check the head of the list for the time of the next sound
  scrSound = listSounds.GetHead();

  // Set the timer for the next sound
  scrSpan = scrSound.TimeSound - timeCur;
  sec = scrSpan.GetTotalSeconds() * 1000;  // Actually msec. here
  sec = (sec > 50) ? sec : 50;

  pos = GetFirstViewPosition();
  iTimerSounding = SetTimer(GetNextView(pos)->m_hWnd, ID_TIMER_SOUNDING, sec, NULL);
  bSoundingInterrupted = FALSE;
  iCurrentPriority = 0;

  NextAction();
}

void CALEDoc::DoScan(int iChannel)
{
  struct CHANNEL ch;
  double dFreq;

  ch = pData->parrChannels->GetAt(iChannel);
  // Update the status bar (the transmit routine actually updates status
  dFreq = ch.dRecFreq;
  strStatus.Format("Scanning at %f", dFreq);

  // Do the receive
  pRadio->Tune(dFreq);
  pRadio->SetMode(ch.eRecMode);
//  OnReceive(98, 10);
  OnReceive(200, 10);
}

void CALEDoc::DoSingleChannel(int iChannel)
{
  struct CALEDoc::CHANNEL ch;
  double dFreq;

  ch = pData->parrChannels->GetAt(iChannel);
  // Update the status bar (the transmit routine actually updates status
  dFreq = ch.dRecFreq;
  strStatus.Format("Single Channel at %f", dFreq);

  // Do the sound
  pRadio->Tune(dFreq);
  pRadio->SetMode(ch.eRecMode);
  OnReceive(100000, 5);
}

LONG CALEDoc::OnModemAction(UINT wAction, LONG lParam)
{
  // This function takes care of handling the priority of certain actions.
  // The possible actions are to Connect(=25), Sound(=15), Scan(=10),
  // or Single Channel Listen(=5)
  if (((int) wAction > iCurrentPriority) || (iCurrentPriority == 5)) {

    if (iCurrentPriority == 15)
      bSoundingInterrupted = TRUE;

    // Do the action requested
    switch (wAction) {
      case 32:  // Send an AMD message while already connected
        SendAMD(32);
        break;
      case 30:  // Connected
        // I handle connection receives directly and don't go through
        // OnModemAction.  But just in case ...
        OnReceive(100000, 30);
        break;
      case 27:  // Connection made, send acknowledgment
        SendAcknowledgement();
        break;
      case 25:  // Connection attempt
        DoConnection();
        break;
      case 22:  // Send an AMD message
        SendAMD(22);
        break;
      case 20:  // Someone outside is attempting connection to us
        // Note that the FROM and TO sense changes in SendResponse
        SendResponse(sResponseData.strToAddr, sResponseData.strFromAddr);
        break;
      case 15:  // Sound
        DoSound();
        break;
      case 10:  // Scan
        DoScan((int) lParam);
        break;
      case 5:   // Single channel
        DoSingleChannel((int) lParam);
        break;
    }
  } else if ((wAction >= 5) && (wAction <= 10)) {
    // We can't do the requested action yet, so try again
    Sleep(0);
  } else if (wAction == 15) {
    bSoundingInterrupted = TRUE;
  } else if (wAction == 25) {
    DoConnection();
  }
  return 0;
}

void CALEDoc::PrioritizeChannels(int iSelfAddr, int iOtherAddr)
// Make a list of possible channels that can be used for connection and
// put them in priority order with the most likely to succeed at the top.
//
// The algorithm used to prioritize channels is:
//    First, assign every possible channel a score based on the last received LQA.
//    The LQA score is the sum of the pseudo-BER score and the pseudo-SNR scores.
//    If no station/channel LQA score is present assign a score of zero.
//    Second, based on the (very) rough sunset and sunrise times, add 100 to all
//    scores that occur in the same (day or night) period that we are currently in.
// The highest scoring channels are the highest priority.  If ties occur, the 
// higher frequency channels are higher priority.
{
  struct OTHERADDR oa;
  struct SELFADDR sa;
  struct CHANNEL ch;
  struct LQA lqa;
  struct CHANRANK *ChanRanks;
  int i, j, size, size2, count = 0;
  int SunRise, SunSet, iMin;
  double dRxFreq;
  CString strOtherAddr;
  CSunTimes st;
  BOOL bDay, bFound;
  CTime timeCur = CTime::GetCurrentTime();

  // Set up an empty priority list of the channels that the iOtherAddr can use.
  if (sChannelPriorities.iNumChannels != 0)
    delete sChannelPriorities.arrChannels;
  oa = Data.parrOtherAddrs->GetAt(iOtherAddr);
  sa = Data.parrSelfAddrs->GetAt(iSelfAddr);
  strOtherAddr = oa.strOtherAddr;

  // Count the channels used
  size = oa.parrValidChannels->GetSize();
  for (i = 0; i < size; i++) {
    ch = arrChannels.GetAt(i);
    if (oa.parrValidChannels->GetAt(i) && sa.parrValidChannels->GetAt(i) &&
        (ch.eTRMode == CALEDoc::TR))
      count++;
  }
  sChannelPriorities.iNumChannels = count;
  sChannelPriorities.arrChannels = new int[count];
  sChannelPriorities.iNextChannel = 0;

  // Allocate an array to calculate channel scores
  ChanRanks = new CHANRANK[count];

  // Get the approximate sunrise and sunset times (in minutes).
  // This is extremely rough.  The SunRise and SunSet parameters are
  // latitude and longitude.  So the times returned are only good for that
  // position.  Other areas will vary considerably.
#define LATITUDE 45.0
#define LONGITUDE 0.0
  SunRise = (int) st.GetSunRiseGMT(LATITUDE, LONGITUDE);
  SunSet = (int) st.GetSunSetGMT(LATITUDE, LONGITUDE);

  // Is the current time day or not?
  iMin = timeCur.GetHour() * 60 + timeCur.GetMinute();
  if ((iMin > SunRise) && (iMin < SunSet))
    bDay = TRUE;
  else
    bDay = FALSE;

  count = 0;
  size2 = arrLQAs.GetSize();

  for (i = 0; i < size; i++) {
    ch = arrChannels.GetAt(i);
    if (oa.parrValidChannels->GetAt(i) && sa.parrValidChannels->GetAt(i) &&
        (ch.eTRMode == CALEDoc::TR)) {
      ChanRanks[count].iChannelNum = i;
      dRxFreq = ch.dRecFreq;
      ChanRanks[count].dRxFreq = dRxFreq;

      // Find the LQA numbers (if any) for this channel/address
      bFound = FALSE;
      for (j = 0; j < size2; j++) {
        lqa = arrLQAs.GetAt(j);
        if ((lqa.dRxFreq == dRxFreq) && (lqa.strOtherAddr == strOtherAddr)) {
          bFound = TRUE;
          break;
        }
      }

      if (bFound) {
        ChanRanks[count].iScore = lqa.RxBER.value + lqa.RxSNR.value;
        // Check the time of the LQA readings
        iMin = lqa.RxBER.time.GetHour() * 60 + lqa.RxBER.time.GetMinute();
        if (((iMin > SunRise) && (iMin < SunSet)) == bDay)
          ChanRanks[count].iScore += 100;
      } else
        // If we couldn't find an LQA score, give it a zero
        ChanRanks[count].iScore = 0;

      count++;
    }
  }

  // Sort the ChanRanks
  SortChannels(ChanRanks, count);

// Put the results in the output array
  for (i = 0; i < count; i++)
    sChannelPriorities.arrChannels[i] = ChanRanks[i].iChannelNum;

  delete ChanRanks;
}

void CALEDoc::SortChannels(CHANRANK *ChanRanks, int count)
// Sort the CHANRANKs according to the score.  Resolve ties by taking
// the higher frequency.  The size of the array should always be very
// small, so just do a very simple sort.
{
  int i, j;
  CHANRANK temp;
  BOOL bChange;

  for (j = 0; j < count - 1; j++) {
    bChange = FALSE;
    for (i = 0; i < count - 1; i++) {
      if ((ChanRanks[i].iScore < ChanRanks[i + 1].iScore) ||
          ((ChanRanks[i].iScore == ChanRanks[i + 1].iScore) && 
           (ChanRanks[i].dRxFreq < ChanRanks[i + 1].dRxFreq))) {
        temp = ChanRanks[i];
        ChanRanks[i] = ChanRanks[i + 1];
        ChanRanks[i + 1] = temp;
        bChange = TRUE;
      }
    }
    if (!bChange)
      break;
  }
}

void CALEDoc::SingleChannel(int iChannel)
{
  // Set up the priority list with a single channel.
  if (sChannelPriorities.iNumChannels != 1) {
    if (sChannelPriorities.iNumChannels != 0)
      delete sChannelPriorities.arrChannels;
    sChannelPriorities.arrChannels = new int[1];
  }

  sChannelPriorities.iNumChannels = 1;
  sChannelPriorities.iNextChannel = 0;
  sChannelPriorities.arrChannels[0] = iChannel;
}

void CALEDoc::DoConnection()
{
  CString str, strAddr;
  struct SELFADDR sa;
  struct OTHERADDR oa;
  struct CHANNEL ch;
  double dFreq;
  int i, j, iChanCnt, Ts, iAddrWords, iToWords;

  sa = arrSelfAddrs.GetAt(sChannelPriorities.iSelfAddr);
  oa = arrOtherAddrs.GetAt(sChannelPriorities.iOtherAddr);
  if (sChannelPriorities.iNextChannel >= sChannelPriorities.iNumChannels) {
    // We have tried all of the channels unsuccessfully.  Inform the user.
    strStatus.Empty();
    pStatusBar->SetPaneText(1, strStatus, TRUE);
    str.Format("The connection attempt from %s to %s failed.",
      sa.strSelfAddr, oa.strOtherAddr);
    MessageBox(NULL, str, "Alert", MB_OK);

    // Restart any scanning
    iCurrentPriority = 0;
    NextAction();
  } else {
    // Attempt connection
    str = oa.strOtherAddr;
    strAddr = sa.strSelfAddr;
    ch = arrChannels.GetAt(sChannelPriorities.arrChannels[sChannelPriorities.iNextChannel]);

    // Increment iNextChannel so we don't retry this channel
    sChannelPriorities.iNextChannel++;

    // Write to status bar
    dFreq = ch.dTransFreq;
    strStatus.Format("Attempting connection to %s on %f", str, dFreq);

    // Do the initial transmit
    pRadio->Tune(dFreq);
    pRadio->SetMode(ch.eTransMode);

    // The scan period (Ts) is the number of channels * the maximum time for a
    // single channel (784 ms).  Note that 784ms is the time for 2 ALE words.
    iChanCnt = 0;
    j = oa.parrValidChannels->GetSize();
    for (i = 0; i < j; i++)
      if (oa.parrValidChannels->GetAt(i))
        iChanCnt++;
    Ts = iChanCnt * 2;  // In ALE word times

    // Make up the scan call part of the calling cycle
    lTransBuf[0] = _TO | ThreeChar(str);
    for (i = 1; i < Ts; i++)
      lTransBuf[i] = lTransBuf[0];

    // Add the leading call part of the calling cycle
    iToWords = MakeAddrWords(_TO, str, lTransBuf + Ts);
    iToWords = MakeAddrWords(_TO, str, lTransBuf + Ts + iToWords);

    iAddrWords = MakeAddrWords(_TIS, strAddr, lTransBuf + Ts + 2 * iToWords);

    // Check channel clear
    if (CheckChannelClear(25)) {
      // and transmit the call
      Transmit(Ts + 2 * iToWords + iAddrWords, 25);
      // Wait for the transmission to end
      ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

      // Set up to receive the response.  (The Tune and SetMode 
      // functions check to see if a change is really needed.)
      pRadio->Tune(ch.dRecFreq);
      pRadio->SetMode(ch.eRecMode);
      OnReceive(376, 25);  // 188 = Wait 1.50 s.
    } else {
      // If the channel was busy,
      // Let the operator know (on the status bar), and fail on this channel
      strStatus = strStatus + " FAILED: Busy channel";
      pStatusBar->SetPaneText(1, strStatus, TRUE);
      DoConnection();
    }
  }
}

void CALEDoc::SendAMD(int iPriority)
{
  CString str, strAddr;
  struct OTHERADDR oa;
  struct CHANNEL ch;
  double dFreq;
  int i, j, iChanCnt, Ts, iAddrWords, iAMDWords, iToWords, iOldPriority;

  if (iPriority == 22)
    oa = arrOtherAddrs.GetAt(sChannelPriorities.iOtherAddr);

  // Attempt connection
  str = sResponseData.strFromAddr;
  strAddr = sResponseData.strToAddr;
  ch = arrChannels.GetAt(sResponseData.iChannelNumber);

  // Write to status bar
  dFreq = ch.dTransFreq;
  strStatus.Format("Sending AMD to %s on %f", str, dFreq);

  // Set up the transmit
  pRadio->Tune(dFreq);
  pRadio->SetMode(ch.eTransMode);

  if (iPriority == 22) {
    // The scan period (Ts) is the number of channels * the maximum time for a
    // single channel (784 ms).  Note that 784ms is the time for 2 ALE words.
    iChanCnt = 0;
    j = oa.parrValidChannels->GetSize();
    for (i = 0; i < j; i++)
      if (oa.parrValidChannels->GetAt(i))
        iChanCnt++;
    Ts = iChanCnt * 2;  // In ALE word times
  } else
    Ts = 0;

  // Make up the scan call part of the calling cycle
  lTransBuf[0] = _TO | ThreeChar(str);
  for (i = 1; i < Ts; i++)
    lTransBuf[i] = lTransBuf[0];

  // Add the leading call part of the calling cycle
  iToWords = MakeAddrWords(_TO, str, lTransBuf + Ts);
  iToWords = MakeAddrWords(_TO, str, lTransBuf + Ts + iToWords);

  // Add in the AMD message.
  iAMDWords = MakeAddrWords(_CMD, strAMDText, lTransBuf + Ts + 2 * iToWords);

  // Put in the from part
  if (iPriority == 22) 
    iAddrWords = MakeAddrWords(_TWAS, strAddr, lTransBuf + Ts + 2 * iToWords + iAMDWords);
  else
    iAddrWords = MakeAddrWords(_TIS, strAddr, lTransBuf + Ts + 2 * iToWords + iAMDWords);
  iOldPriority = iCurrentPriority;

retry:
  // Check channel clear
  if (CheckChannelClear(iPriority)) {
    // and transmit the call
    Transmit(Ts + 2 * iToWords + iAMDWords + iAddrWords, 25);
    // Wait for the transmission to end
    ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);

    // Here is a mysterious Sleep function call.  The code, in a debug compile, works
    // OK without the sleep.  However in a release compile, the program fails when
    // a connection is made, an AMD message is sent, then another AMD message is sent.
    // The second AMD will fail to occur.  With this sleep inserted all is OK.  Apparently
    // some subtle timing problem is occurring, but it is not known what the problem is.
    // Perhaps allocation or deallocation of DirectSound resources???
    ::Sleep(10);
  } else {
    // If the channel was busy,
    // Let the operator know, and fail or retry
    if (MessageBox(NULL, "The channel was busy.  Retry?", "Alert", MB_YESNO) == IDYES)
      goto retry;
  }

  if (iOldPriority >= 30)
    iCurrentPriority = 30;
  else
    iCurrentPriority = 0;

  NextAction();
}

void CALEDoc::BreakConnection()
{
  int iToWords, iAddrWords;

  // Send a this was.
  iToWords = MakeAddrWords(_TO, sResponseData.strFromAddr, lTransBuf);
  iToWords = MakeAddrWords(_TO, sResponseData.strFromAddr, lTransBuf + iToWords);

  iAddrWords = MakeAddrWords(_TWAS, sResponseData.strToAddr, lTransBuf + 2 * iToWords);

  // Possibly check channel clear

  // Priority 35 is used so that this transmit can interrupt the receive
  Transmit(2 * iToWords + iAddrWords, 35);
  // Wait for the transmission to end
  ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);
  // Notify the user
  strStatus.Format("Connection Ended");
  pStatusBar->SetPaneText(1, strStatus, TRUE);

  iCurrentPriority = 0;
  NextAction();
}

void CALEDoc::DoAllCall(int iSelfAddr, int iChanNum)
// This function does an All Call using the Self Address and Channel Number passed.
{
  CString str("@?@"), strAddr;
  struct SELFADDR sa;
  struct CHANNEL ch;
  double dFreq;
  int i, j, iChanCnt, Ts, iAddrWords;

  sa = arrSelfAddrs.GetAt(iSelfAddr);
  ch = arrChannels.GetAt(iChanNum);
  strAddr = sa.strSelfAddr;

  // Write to status bar
  dFreq = ch.dTransFreq;
  strStatus.Format("Making an Allcall on %f", dFreq);

  // Do the initial transmit
  pRadio->Tune(dFreq);
  pRadio->SetMode(ch.eTransMode);

  // The scan period (Ts) is the number of channels * the maximum time for a
  // single channel (784 ms).  Note that 784ms is the time for 2 ALE words.
  iChanCnt = 0;
  j = sa.parrValidChannels->GetSize();
  for (i = 0; i < j; i++)
    if (sa.parrValidChannels->GetAt(i))
      iChanCnt++;
  Ts = iChanCnt * 2;  // In ALE word times

  // Make up the scan call and leading call parts of the calling cycle
  // Since we know the address is 3 chars, the scan and leading parts
  // use the same address
  lTransBuf[0] = _TO | ThreeChar(str);
  for (i = 1; i < Ts + 2; i++)
    lTransBuf[i] = lTransBuf[0];

  iAddrWords = MakeAddrWords(_TIS, strAddr, lTransBuf + Ts + 2);

retry:
  // Check channel clear
  if (CheckChannelClear(18)) {
    // and transmit the call
    Transmit(Ts + 2 + iAddrWords, 18);
    // Wait for the transmission to end
    ::WaitForSingleObject(m_stoppedEvent.m_hObject, INFINITE);
    iCurrentPriority = 0;
  } else {
    // If the channel was busy,
    // Let the operator know, and fail or retry
    if (MessageBox(NULL, "The channel was busy.  Retry?", "Alert", MB_YESNO) == IDYES)
      goto retry;
  }

  NextAction();
}

//////////////////////////////////////////////////////////////////////////
// Receiver Thread

// Direct Sound Notification Events
extern HANDLE rghEvent[];
// One Block of data from the software Modem
extern struct strSymbolB SymbolIn[];
// One Block of data to be transmitted by the software modem
extern short iSymbolOut[];
extern short iSignalPresent, iALEPresent, iTxEnd;

//        FEC routines for MIL/FED std ALE        eej

/********************  GOLAY DECODER MODULE    *********************/
 
short NextSym()
/* returns next symbol received from modem */
{
  if (ptrSymbolIn < HALFBLKOFSYMBOLS) {
    return SymbolIn[ptrSymbolIn++].iSymbol;
  } else {
    ptrSymbolIn = 0;
    return -2;
  }
}

#define encode(x) (((long) x << 12) | enc[x])

#define ERROR_DETECTED -1

int errwt(long w)
{
    register int i;

    i = (encode(w>>12) ^ w) & 07777;
    return (wt[i]);
}

long decode(long w, long power)
/* decode one 24-bit Golay word */
{
  register int i;

  if (errwt(w) > power)
    return (ERROR_DETECTED);
  i = (encode(w>>12) ^ w) & 07777;
  return (w>>12 ^ e[i]);
}

long DeGolay(long G0, long G1, long power) 
/* decode two 24-bit Golay words to yield one 24-bit ALE word. */
{
  long temp, temp2;
  
  temp = decode(G1, power);
  if (temp > -1) {
    temp2 = decode(G0, power);
    if (temp2 > -1)
      return ((temp2 << 12) | (temp & 07777));
  }

  return (ERROR_DETECTED);
}

BOOL initRxFEC(int dwell, BOOL bFirstInit)
// Initialize the Receiver
{
  int i, index;
  short symbol;

  if (bFirstInit) {
    pingpong = 0;  	/* initialize pointers */
    ptr0 = 0;
    ptr1a = 16;
    ptr1b = 17;
    ptr2a = 32;
    ptr2b= 33;
    uptr = 0;
    ptrSymbolIn = 0;
    g1 = g2 = 0;
    for (i = 0; i < 16; i++)	/* initialize unanimous vote circular buffer */
      ubuffer[i] = 0;     /*   (stores unanimous vote counts for 16    */
                          /*    most recent majority votes.)           */
    Ucount = 0;

    for (i = 0; i < SR1LEN; i++)
      sr1[i] = 0; /* init received-symbol shift rgtr */
  }

  if (initcntr < SR1LEN - 1) { 
    if ((symbol = NextSym()) == -2)
      // The symbol buffer is empty, return FALSE to show this
      return FALSE;  

    /* read first 48 symbols into shift register */
	  index = ((*(sr1+ptr1a) & 3) << 7) | ((*(sr1+ptr1b) & 4) << 4) |
	          ((*(sr1+ptr2a) & 1) << 5) | ((*(sr1+ptr2b) & 6) << 2) |
	           (*(sr1+ptr0) = symbol);  /* concatenate three redundant symbols */
	                                    /* to make an index for table accesses */
	  
	  /* Unanimous Votes */
	  Ucount += utable[index] - ubuffer[uptr]; /* update unanimous vote count  */
	  ubuffer[uptr] = utable[index];           /* shift old value out, new in  */
	  
	  /* Majority Votes and Deinterleaving */
	  maj = mtable[index];
	  if (pingpong) {
	    g1 = ((g1 << 2) | (((maj & 4) >> 1) | (maj & 1)));
	    g2 = ((g2 << 1) | ((maj & 2) >> 1));
	  } else {
	    g2 = ((g2 << 2) | (((maj & 4) >> 1) | (maj & 1)));
	    g1 = ((g1 << 1) | ((maj & 2) >> 1));
	  }
	  
	  /* update pointers to circular buffers */
	  pingpong = 1 - pingpong;
	  ptr0  = (++ptr0  >= SR1LEN) ? 0 : ptr0;
	  ptr1a = (++ptr1a >= SR1LEN) ? 0 : ptr1a;
	  ptr1b = (++ptr1b >= SR1LEN) ? 0 : ptr1b;
	  ptr2a = (++ptr2a >= SR1LEN) ? 0 : ptr2a;
	  ptr2b = (++ptr2b >= SR1LEN) ? 0 : ptr2b;
	  uptr = ++uptr & 0xf;

    initcntr++;
  }
  
  SymbolsToGo = dwell - (SR1LEN - 1);   // number of symbols allowed
                        // for word sync acq. - symbols already seen
  Sync = FALSE;                    /* clear word sync flag */

  return TRUE;  // for completion of initRxFec
}

long RxFEC(int power) 
// Returns a Golay decoded word if successful
// Returns -2 if the SymbolIn buffer is empty
// Returns -1 for no good Golay word from bad DeGolay decode)
// Returns -3 for lack of sync
{
  int index;
  short symbol;
  long gol;   /* Golay decoded word */
  
  if (Sync) {			/* just get next 49 symbols */
	  while ((symbol = NextSym()) != -2) {
	    index = ((*(sr1+ptr1a) & 3) << 7) | ((*(sr1+ptr1b) & 4) << 4)|
	            ((*(sr1+ptr2a) & 1) << 5) | ((*(sr1+ptr2b) & 6) << 2) |
               (*(sr1+ptr0) = symbol);  
	      
	    /* Unanimous Votes */
	    Ucount += utable[index] - ubuffer[uptr];
	    ubuffer[uptr] = utable[index];

	    /* Majority Votes and Deinterleaving */
	    maj = mtable[index];
	    if (pingpong) {
		    g1 = ((g1 << 2) | (((maj & 4) >> 1) | (maj & 1)));
		    g2 = ((g2 << 1) | ((maj & 2) >> 1));
      } else {
		    g2 = ((g2 << 2) | (((maj & 4) >> 1) | (maj & 1)));
		    g1 = ((g1 << 1) | ((maj & 2) >> 1));
	    }
	    
	    /* update pointers to circular buffers */
	    pingpong = 1 - pingpong;
	    ptr0  = (++ptr0  >= SR1LEN) ? 0 : ptr0;
	    ptr1a = (++ptr1a >= SR1LEN) ? 0 : ptr1a;
	    ptr1b = (++ptr1b >= SR1LEN) ? 0 : ptr1b;
	    ptr2a = (++ptr2a >= SR1LEN) ? 0 : ptr2a;
	    ptr2b = (++ptr2b >= SR1LEN) ? 0 : ptr2b;
      sptr  = (++sptr  >= SR1LEN) ? 0 : sptr;
	    uptr = ++uptr & 0xf;

      if (sptr == 0) {
        // We have processed 49 symbols, return the word
	      if (pingpong) /* These are reversed due to pingpong being inc */
	        return(DeGolay(((g2>>1) & 0xffffff), ((g1^0xfff) & 0xffffff), power));
	      else
	        return(DeGolay(((g1>>1) & 0xffffff), ((g2^0xfff) & 0xffffff), power));
      }
	  }

    // We have reached the end of the symbols available now
    return -2;

  } else {  /* looking for word sync */
    while(!Sync && (SymbolsToGo-- > 0)) {
	  
      if ((symbol = NextSym()) == -2) {
        // No more symbols in the buffer for now
        SymbolsToGo++;
        return -2;
      }

	    /* Compute Index into majority vote and unanimous vote tables */
	    index = ((*(sr1+ptr1a) & 3) << 7) | ((*(sr1+ptr1b) & 4) << 4) |
	            ((*(sr1+ptr2a) & 1) << 5) | ((*(sr1+ptr2b) & 6) << 2) |
	             (*(sr1+ptr0) = symbol);  
	    
	    /* Unanimous Votes */
	    Ucount += utable[index] - ubuffer[uptr];
	    ubuffer[uptr] = utable[index];
	    
	    /* Majority Votes and Deinterleaving */
	    maj = mtable[index];
	    if (pingpong) {
	      g1 = ((g1 << 2) | (((maj & 4) >> 1) | (maj & 1)));
	      g2 = ((g2 << 1) | ((maj & 2) >> 1));
	      gol = DeGolay(((g1>>1) & 0xffffff), ((g2^0xfff) & 0xffffff), power);
	    } else {
	      g2 = ((g2 << 2) | (((maj & 4) >> 1) | (maj & 1)));
	      g1 = ((g1 << 1) | ((maj & 2) >> 1));
	      gol = DeGolay(((g2>>1) & 0xffffff), ((g1^0xfff) & 0xffffff), power);
	    }

	    /* update pointers to circular buffers */
	    pingpong = 1 - pingpong;
	    ptr0  = (++ptr0  >= SR1LEN) ? 0 : ptr0;
	    ptr1a = (++ptr1a >= SR1LEN) ? 0 : ptr1a;
	    ptr1b = (++ptr1b >= SR1LEN) ? 0 : ptr1b;
	    ptr2a = (++ptr2a >= SR1LEN) ? 0 : ptr2a;
	    ptr2b = (++ptr2b >= SR1LEN) ? 0 : ptr2b;
	    uptr = ++uptr & 0xf;
	  
	    if ((gol != (long) -1) && (Ucount >= THRESHOLD)) {
	      Sync = TRUE;   /* got good golay; word sync is possible */
        sptr = 0;
	      return (gol);
	    }
    }
  }

  return (-3);      /* Sync failure, no good golay word received */
}

long ASCII_OK(long w, char set)
{
  if (ASCII_Set[w & 0177] >= set)
    if (ASCII_Set[(w >> 7) & 0177] >= set)
      if (ASCII_Set[(w >> 14) & 0177] >= set)
        return w;
  return 0;
}

int PreambleOK(long w)
// OK for TO or THIS IS or THIS WAS
{
  static int PreambleMask[8] = {0, 0, 1, 1, 0, 1, 0, 0};

  if (w)
    return PreambleMask[(w >> 21) & 7];
  return 0;
}

long WordSync()
{
  long ct;          /* (ciphertext) word from RxFEC */

  /* keep trying until RxFEC gives up */
  while ((ct = RxFEC(GOLAY_SCAN)) > 0) {
    if (PreambleOK(ASCII_OK(ct, ASCII_38)))
      return (ct);
    else
      Sync = FALSE;
  }

  // Make all errors less than -3 return -1
ASSERT(ct >= -3);
  if (ct < -3)
    ct = -1;

  return ct;  // (-1 degolay failed, -2 paused, -3 word sync failed
}

void MakeALEWord(BOOL bFirstTime, RXTHREADPARMS* pRxThreadParms)
// MakeALEWord is called whenever the input buffer from DirectX is
// full.  The symbols are read out and put into ALE words.  MakeALEWord is
// independent of the size of the input buffer.
{
  static BOOL TotalSync, initRxDone;
  long ALEWord;
  CIRCBUFINDATA BufferEntry;

  CCircBufI* pCircBufI = pRxThreadParms->pCircBufI;

  // Assume ALE is detected unless shown otherwise for scanning
  BufferEntry.bALEDetected = TRUE;

  if (bFirstTime) {
    // If this is only a signal check, just return that result
    if (pRxThreadParms->m_bSignalCheck) {
      pRxThreadParms->m_bChannelClear = !iSignalPresent;

      // Stop the receive function here
      pRxThreadParms->pworkingEvent->ResetEvent();
      return;
    }

    // If we are scanning, do a quick check for an ALE signal and return if none
    if ((pRxThreadParms->iPriority == 10) && !iALEPresent) {
      BufferEntry.ALEWord = -10;
      BufferEntry.bALEDetected = FALSE;
      pCircBufI->Put(&BufferEntry, 1);
      // Tell the frame window there is data available
      // Also set the error flag to zero because there is no modem error
      pRxThreadParms->hrModemError = 0;
      pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
      // Stop the receive function here
      pRxThreadParms->pworkingEvent->ResetEvent();
      return;
    }

    initRxDone = FALSE;
    initcntr = 0;
    if (initRxFEC(pRxThreadParms->dwell, TRUE))
      initRxDone = TRUE;
    else
      return;
    TotalSync = FALSE;
  }

  // MakeALEWord may be called when initRxFEC has started to initialize but
  // is not yet complete.
  if (!initRxDone) {
    if (initRxFEC(pRxThreadParms->dwell, FALSE))
      initRxDone = TRUE;
    else
      return;
    TotalSync = FALSE;
  }

  if (!TotalSync) {
    ALEWord = WordSync();
    if (ALEWord == -2)
      // Run out of symbols in the buffer
      return;  // wait for next set of symbols to continue
    else if (ALEWord < 0) {
      // Sync failed
ASSERT((ALEWord == -1) || (ALEWord == -3));
      BufferEntry.ALEWord = ALEWord;
      BufferEntry.Ucount = Ucount;
      pCircBufI->Put(&BufferEntry, 1);
      // Tell the frame window there is data available
      // Also set the error flag to zero because there is no modem error
      pRxThreadParms->hrModemError = 0;
      pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
      // Stop the receive function here
      pRxThreadParms->pworkingEvent->ResetEvent();
      return;
    } else {
      // We have a good word, pass it back
      TotalSync = TRUE;

      BufferEntry.ALEWord = ALEWord;
      BufferEntry.Ucount = Ucount;
      BufferEntry.dSNR = SymbolIn[HALFBLKOFSYMBOLS - 1].dSNR;
      BufferEntry.bALEEnded = iTxEnd;
      pCircBufI->Put(&BufferEntry, 1);
      // Tell the frame window there is data available
      // Also set the error flag to zero because all is fine
      pRxThreadParms->hrModemError = 0;
      pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
      if (iTxEnd) {
        // If the transmission has ended, stop the receive function here
        pRxThreadParms->pworkingEvent->ResetEvent();
        return;
      }
    }
  }
  // Get the next word and continue with any symbols left in the buffer
  while (TRUE) {
    ALEWord = RxFEC(GOLAY_RUN);
    if (ALEWord == -2)
      // Run out of symbols in the buffer
      return;  // wait for next set of symbols to continue
    else if (ALEWord < 0) {  
      // We have a bad DeGolay decode
      BufferEntry.ALEWord = ALEWord;
      BufferEntry.Ucount = Ucount;
      pCircBufI->Put(&BufferEntry, 1);
      // Tell the frame window there is data available
      // Also set the error flag to zero because there is no modem error
      pRxThreadParms->hrModemError = 0;
      pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
      // Stop the receive function here
      pRxThreadParms->pworkingEvent->ResetEvent();
      return;
    } else {
      // We have a good word, pass it back
      BufferEntry.ALEWord = ALEWord;
      BufferEntry.Ucount = Ucount;
      BufferEntry.dSNR = SymbolIn[HALFBLKOFSYMBOLS - 1].dSNR;
      BufferEntry.bALEEnded = iTxEnd;
      pCircBufI->Put(&BufferEntry, 1);
      // Tell the frame window there is data available
      // Also set the error flag to zero because all is fine
      pRxThreadParms->hrModemError = 0;
      pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
      if (iTxEnd) {
        // If the transmission has ended, stop the receive function here
        pRxThreadParms->pworkingEvent->ResetEvent();
        return;
      }
    }
  }
}

/********************   GOLAY ENCODER MODULE   *********************/

void Golay(long w, long G[])
/* encode a 24-bit ALE word, producing two 24-bit Golay words */
{
  long temp;

  G[0] = encode(w >> 12);
  temp = w & 07777;
  G[1] = encode(temp) ^ 07777;  /* invert B word check bits */
}
 
short *TxFEC(long ALE_WORD, short *Txw)		/* new 7/14/91 */
{
  long G[2];         	/*	two 24-bit Golay code words	*/
  register int i;
  register long a, b;
  
  Golay(ALE_WORD, G);
  
  a = G[0] << 2;
  b = G[1] << 1;

  /*****  INTERLEAVING AND REDUNDANCY  *****/

  Txw[48] = a&4 | b&2;
  for (i=46; i>33; i -= 2) {
	  Txw[i] = a&040 | b&020;
	  a >>= 1;
    b >>= 1;
	  Txw[i] = (Txw[i] | a&010) >> 3;
	  Txw[i+1] = b&4;
	  a >>= 1;
    b >>= 1;
	  Txw[i+1] |= a&2 | b&1;
	  a >>= 1;
    b >>= 1;
  }
  a |= (G[1] << 5);
  b |= (G[0] << 5)	/*	stuff bit for free	*/;
  for (i=32; i>16; i -= 2) {
	  Txw[i] = a&040 | b&020;
	  a >>= 1;
    b >>= 1;
	  Txw[i] = (Txw[i] | a&010) >> 3;
	  Txw[i+1] = b&4;
	  a >>= 1;
    b >>= 1;
	  Txw[i+1] |= a&2 | b&1;
	  a >>= 1;
    b >>= 1;
  }
  a |= (G[0] << 6);
  b |= (G[1] << 5)	/*	stuff bit for free	*/;
  for (i=16; i>=0; i -= 2) {
	  Txw[i] = a&040 | b&020;
	  a >>= 1;
    b >>= 1;
	  Txw[i] = (Txw[i] | a&010) >> 3;
	  Txw[i+1] = b&4;
	  a >>= 1;
    b >>= 1;
	  Txw[i+1] |= a&2 | b&1;
	  a >>= 1;
    b >>= 1;
  }
  return Txw;
}

// HDAudio system includes
#include <mmsystem.h>
#include <dsound.h>     // dsound.h requires mmsystem.h defined before dsound.h

UINT RxThread(LPVOID pParam)
{
  LPARAM size;
  CString str;
  int j;  // Counter for the Transmit loop

  // The pParam is a pointer to some structure that holds anything you want
  // to pass to the thread.
  RXTHREADPARMS* pRxThreadParms = (RXTHREADPARMS*) pParam;
  CEvent* pWorkingEvent = pRxThreadParms->pworkingEvent;
  CEvent* pStoppedEvent = pRxThreadParms->pstoppedEvent;

  MSG msg;
  CCircBufO* pCircBufO = pRxThreadParms->pCircBufO;
  CCircBufI* pCircBufI = pRxThreadParms->pCircBufI;
  long ALEword;
  short Txw[49];

  BOOL bFirstTime; // bFirstTime is TRUE everytime reception is started
  BOOL bFirstEver = TRUE; // Initialization first time flag
  BOOL bDone;
  HRESULT hr;
  short iSilentBlks; // Counter for the zero transmit buffers
  short iSymTotalOut[MAX_TX_SIZE];
  short iTxIndex;
  short iRestart = 0;
  HDAudio HDA;
  HDA.dNoiseThreshold = pRxThreadParms->dNoiseThreshold;
  DWORD dwEvt; // DSound Notification service Event index 

  // Initialize the Half Duplex Audio HDAudio class

  while (TRUE) {
    if (::WaitForSingleObject(pWorkingEvent->m_hObject, 0) != WAIT_OBJECT_0) {
      pStoppedEvent->SetEvent();

      if ( !bFirstEver && pRxThreadParms->m_bReceiving) {
        // We are done with receiving , stop!
        if ((hr = HDA.Stop(INPUTBUFFER)) != DS_OK) {
          pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
          // Tell the frame window there is data available
          pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
          // Release any of the DirectX services
          HDA.Release();
          // Reset the event and wait for the thread to be shut down
          pWorkingEvent->ResetEvent();
          // Go back to the main while(TRUE) loop
          continue;
        }
      }

      ::WaitForSingleObject(pWorkingEvent->m_hObject, INFINITE);

      bFirstTime = TRUE;
    }

    if (bFirstEver) {
      // One time initialization of the Direct X services
      while (iRestart < 3) {
        if (HDA.InitDSound(pRxThreadParms->pWnd) != DS_OK ) {
          HDA.Release();
          if (iRestart < 2) {
            ::Sleep(1000);
            iRestart++;
          } else if (iRestart == 2) {
            pRxThreadParms->hrModemError = 1;
            // Tell the frame window there is data available
            pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
            // Release any of the DirectX services
            HDA.Release();
            // Reset the event and wait for the thread to be shut down
            pWorkingEvent->ResetEvent();
            // Go back to the main while(TRUE) loop
          }
        } else
          iRestart = 100;
      }
      bFirstEver = FALSE;
    }

    // Check that this is the first time through the loop
    if (bFirstTime && pRxThreadParms->m_bReceiving) {
      HDA.Stop(INPUTBUFFER);
      HDA.Stop(OUTPUTBUFFER);
      // Initialize the DSound notification interface to point to the input capture buffer
      if ((hr = HDA.InitNotify(INPUTBUFFER)) != DS_OK) {
        pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
        // Tell the frame window there is data available
        pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
        // Release any of the DirectX services
        HDA.Release();
        // Reset the event and wait for the thread to be shut down
        pWorkingEvent->ResetEvent();
        // Go back to the main while(TRUE) loop
        continue;
      }

      // Now start the receiver
      if ((hr = HDA.Start(INPUTBUFFER)) != DS_OK) {
        pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
        // Tell the frame window there is data available
        pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
        // Release any of the DirectX services
        HDA.Release();
        // Reset the event and wait for the thread to be shut down
        pWorkingEvent->ResetEvent();
        // Go back to the main while(TRUE) loop
        continue;
      }
    }

    pStoppedEvent->ResetEvent();

    if (pRxThreadParms->m_bReceiving) {
      // Receiving
      // Read data from radio
      // move to buffer,

      dwEvt = MsgWaitForMultipleObjects(
                    NUMBEVENTS+1, // Number of events for the DirectSoundCaptureBuffer
                    rghEvent,     // Location of the handles
                    FALSE,        // Wait for all?
                    INFINITE,     // How long to wait?
                    QS_ALLINPUT );// Any message is an event

      // WAIT_OBJECT_0 == zero but is treated as an arbitrary
      // index value asssigned to the first event, therefore we subtract 
      // it from the dwEvt to get the zero-based index of the event

      dwEvt -= WAIT_OBJECT_0;

      // If the event was set by the capture buffer, you have input
      // to process.
      if (dwEvt < NUMBEVENTS && dwEvt >= 0) {
        // Process the next input buffer of data from DirectX
        if ((hr = HDA.ProcessInputBuffer(dwEvt)) != DS_OK) {
          pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
          // Tell the frame window there is data available
          pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
          // Release any of the DirectX services
          HDA.Release();
          // Reset the event and wait for the thread to be shut down
          pWorkingEvent->ResetEvent();
          // Quit receiving
          goto RxError;
        }

        // There is a half second of symbols in an input buffer
        MakeALEWord(bFirstTime, pRxThreadParms);
        bFirstTime = FALSE;
      } else if (dwEvt == NUMBEVENTS)
        HDA.StopOutputBuffer();

      // If it's the last event, it's a message.
      else if (dwEvt == NUMBEVENTS + 1) {
        while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
			      DispatchMessage(&msg);
        }
      }

RxError:
      ::Sleep(0);
    } else {
      /////////////////////////////////////////////////////////////////////////////////////
      // Transmitting
      // Initialize the DSound notification interface to point to the output buffer 
      if ((hr = HDA.InitNotify(OUTPUTBUFFER)) != DS_OK) {
          pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
          // Tell the frame window there is data available
          pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
          // Release any of the DirectX services
          HDA.Release();
          goto TxError;
      }
      // Now start the transmitter
      if ((hr = HDA.Start(OUTPUTBUFFER)) != DS_OK) {
          pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
          // Tell the frame window there is data available
          pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
          // Release any of the DirectX services
          HDA.Release();
          goto TxError;
      }
      // write data to the radio
      // Get the data to be transmitted
      size = 0;
      while (pCircBufO->Get(&ALEword, 1)) {
        TxFEC(ALEword, Txw);
        for (j = 0; j < 49; j++)
          iSymTotalOut[size + j] = Txw[j];
        size += 49;
      }
       
      iSilentBlks = iTxIndex = 0;
      bDone = FALSE;
      while ( !bDone) {

        dwEvt = MsgWaitForMultipleObjects(
                  NUMBEVENTS+1, // Number of events for the DirectSoundCaptureBuffer
                  rghEvent,     // Location of the handles
                  FALSE,        // Wait for all?
                  INFINITE,     // How long to wait?
                  QS_ALLINPUT );// Any message is an event

        // WAIT_OBJECT_0 == zero but is treated as an arbitrary
        // index value asssigned to the first event, therefore we subtract 
        // it from the dwEvt to get the zero-based index of the event

        dwEvt -= WAIT_OBJECT_0;

        // If the event was set by the capture buffer, you have input
        // to process.
        if (dwEvt < NUMBEVENTS) {
          if ((size - iTxIndex) >= HALFBLKOFSYMBOLS) {
            // There are more symbols than will fit in the buffer
            for (j = 0; j < HALFBLKOFSYMBOLS; j++) {
              iSymbolOut[j] = iSymTotalOut[iTxIndex + j];
            }
            iTxIndex += HALFBLKOFSYMBOLS;
          } else if ((size - iTxIndex) < HALFBLKOFSYMBOLS &&
             (size - iTxIndex) != 0) {
            // There are symbols for a partial buffer, fill the rest
            // of the buffer with silence
            for (j = 0; j < (size - iTxIndex); j++) {
              iSymbolOut[j] = iSymTotalOut[iTxIndex + j];
            }
            for (j = (size - iTxIndex); j < HALFBLKOFSYMBOLS; j++) {
              iSymbolOut[j] = 8;
            }
            iTxIndex = (short) size;
          } else if ((size - iTxIndex) == 0 && iSilentBlks <= 3) {
            // There are no more symbols, so fill the buffer with silence
            for (j = 0; j < HALFBLKOFSYMBOLS; j++) {
              iSymbolOut[j] = 8;
            }
            iSilentBlks++;
            if (iSilentBlks == 4)
              bDone = TRUE;
          }

          // Finally squirt out the next transmit block        
          if ((hr = HDA.ProcessOutputBuffer(dwEvt, 5000)) != DS_OK) {
            pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
            // Tell the frame window there is data available
            pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
          // Release any of the DirectX services
          HDA.Release();
          goto TxError;
          }          
        }
        else if (dwEvt == NUMBEVENTS)  
          HDA.StopOutputBuffer(); 
        // If it's the last event, it's a message.
        else if (dwEvt == NUMBEVENTS + 1) {
          while (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
          }
        }
      }

      // We are done with transmitting , stop!
      if ((hr = HDA.Stop(OUTPUTBUFFER)) != DS_OK) {
        pRxThreadParms->hrModemError = (HRESULT) 100 + hr;
        // Tell the frame window there is data available
        pRxThreadParms->pWnd->PostMessage(WM_USER_DATA_AVAIL, 0, 0);
        // Release any of the DirectX services
        HDA.Release();
        goto TxError;
      }
      // empty buffer
      pCircBufO->Reset();
TxError:
      pWorkingEvent->ResetEvent();
    }
  }
  return 0;
}
