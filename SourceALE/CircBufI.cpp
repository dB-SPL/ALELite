// CircBuf.cpp: implementation of the CCircBufI class.
//  10 Mar 99


#include "stdafx.h"
#include <afxmt.h>
#include "CircBuf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCircBufI::CCircBufI()
{
  in = 0;
  out = 0;
}

CCircBufI::~CCircBufI()
{

}

//////////////////////////////////////////////////////////////////////
// Member Functions

void CCircBufI::Reset()
{
  g_CriticalSection.Lock();
  in = 0;
  out = 0;
  g_CriticalSection.Unlock();
}

int CCircBufI::Put(CIRCBUFINDATA *buffer, int count)
{
  g_CriticalSection.Lock();

  // Check if there is sufficient room for the Put
  if (count > (CIRCBUFISIZE - in + out - 1) % CIRCBUFISIZE) { // There is a problem
    g_CriticalSection.Unlock();
    if (count >= CIRCBUFISIZE)
      return 2;
    else
      return 1;
  }
  
  // Move the data into the circular buffer
  for (int i = 0; i < count; i++) {
    strCBuffer[in++] = buffer[i];
    in %= CIRCBUFISIZE;
  }

  g_CriticalSection.Unlock();
  return 0;
}

int CCircBufI::Get(CIRCBUFINDATA *buffer, int count)
{
  int i;

  g_CriticalSection.Lock();

  // Move the data from the circular buffer
  for (i = 0; i < count; i++) {
    if (in == out)
      break;
    buffer[i] = strCBuffer[out++];
    out %= CIRCBUFISIZE;
  }

  g_CriticalSection.Unlock();
  return i;
}
