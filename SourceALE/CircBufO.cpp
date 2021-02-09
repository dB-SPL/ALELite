// CircBufO.cpp: implementation of the CCircBufO class.
//  10 Mar 1999


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

CCircBufO::CCircBufO()
{
  in = 0;
  out = 0;
}

CCircBufO::~CCircBufO()
{

}

//////////////////////////////////////////////////////////////////////
// Member Functions

void CCircBufO::Reset()
{
  g_CriticalSection.Lock();
  in = 0;
  out = 0;
  g_CriticalSection.Unlock();
}

int CCircBufO::Put(long *buffer, int count)
{
  g_CriticalSection.Lock();

  // Check if there is sufficient room for the Put
  if (count > (CIRCBUFOSIZE - in + out - 1) % CIRCBUFOSIZE) { // There is a problem
    g_CriticalSection.Unlock();
    if (count >= CIRCBUFOSIZE)
      return 2;
    else
      return 1;
  }
  
  // Move the data into the circular buffer
  for (int i = 0; i < count; i++) {
    iCBuffer[in++] = buffer[i];
    in %= CIRCBUFOSIZE;
  }

  g_CriticalSection.Unlock();
  return 0;
}

int CCircBufO::Get(long *buffer, int count)
{
  int i;

  g_CriticalSection.Lock();

  // Move the data from the circular buffer
  for (i = 0; i < count; i++) {
    if (in == out)
      break;
    buffer[i] = iCBuffer[out++];
    out %= CIRCBUFOSIZE;
  }

  g_CriticalSection.Unlock();
  return i;
}
