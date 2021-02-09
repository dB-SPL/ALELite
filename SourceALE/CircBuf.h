// CircBuf.h: interface for the CCircBuf class.
//  12 Mar 1999
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CIRCBUF_H__BE28EFE1_731F_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_CIRCBUF_H__BE28EFE1_731F_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HDAudio.h" // You need the defines from here for the CCircBufx sizes

// The longest ALE message is 36 seconds long set the output buffer to that size
#define CIRCBUFOSIZE 100   // ~40 Seconds of output data
// The input buffer can be small since in all likelyhood we'll be checking it quite often
// So to start with lets make it...
#define CIRCBUFISIZE 20    // ~4 Seconds of input data

typedef struct tagCIRCBUFINDATA {
  long ALEWord;  // a 24 bit ALE word
  double dSNR;   // a "signal/noise" associated with the word
  int Ucount;    // count of unanimous votes (for "BER")
  BOOL bALEDetected, // Used during scanning as a fast check if ALE signal present
       bALEEnded;  // TRUE when the end of an ALE transmission is detected
} CIRCBUFINDATA;

//////////////////////////////////////////////////////////////////////////////////////////
// Circular Buffer classes
//
// This classes are thread-safe.  All accesses through member functions are
//    locked by CriticalSections so synchronicity problems should not occur.
//
// If in == out, then the buffer is empty
// If in == out - 1 or (in == CIRCBUFSIZE - 1 and out == 0), then
//    the buffer is full.  This always leaves one place in the buffer
//    unused, so that the full buffer can be differentiated from the 
//    empty buffer.
///////////////////////////////////////////////////////////////////////////////////////////

// Circular Buffer Output
class CCircBufO
{
public:
	CCircBufO();
	virtual ~CCircBufO();

  // Reset the circular buffer to empty
  void Reset();

  // Put copies 'count' bytes from 'buffer' to the circular buffer.
  // Value returned is 0 if successful,
  //                   1 if failure because the circular buffer is too full
  //                   2 if failure because the circular buffer is too small
  //                     (that is the data would not fit even when the buffer
  //                      is empty.)
  // If the Put fails, no data is put in the circular buffer.
  int Put(long *buffer, int count);

  // Get copies up to 'count' words from the circular buffer to 'buffer'.
  //   'count' words will be returned if that many are available.
  // Value returned is the number of bytes returned (possibly zero).
  // If the Get fails, no data is put in the 'buffer'.
  int Get(long *buffer, int count);

private:
  int in, out;
  long iCBuffer[CIRCBUFOSIZE];
  CCriticalSection g_CriticalSection;
};

// Circular Buffer Input
class CCircBufI  
{
public:
	CCircBufI();
	virtual ~CCircBufI();

  // Reset the circular buffer to empty
  void Reset();

  // Put copies 'count' items from 'buffer' to the circular buffer.
  // Value returned is 0 if successful,
  //                   1 if failure because the circular buffer is too full
  //                   2 if failure because the circular buffer is too small
  //                     (that is the data would not fit even when the buffer
  //                      is empty.)
  // If the Put fails, no data is put in the circular buffer.
  int Put(CIRCBUFINDATA *buffer, int count);

  // Get copies up to 'count' items from the circular buffer to 'buffer'.
  //   'count' items will be returned if that many are available.
  // Value returned is the number of items returned (possibly zero).
  // If the Get fails, no data is put in the 'buffer'.
  int Get(CIRCBUFINDATA *buffer, int count);

private:
  int in, out;
  CIRCBUFINDATA strCBuffer[CIRCBUFISIZE];
  CCriticalSection g_CriticalSection;
};

#endif // !defined(AFX_CIRCBUF_H__BE28EFE1_731F_11D2_8F3A_0060089F9D1C__INCLUDED_)
