// SignalQualityDlg.cpp : implementation file
//  24 Jan 2000

#include "stdafx.h"
#include "ALE.h"
#include "ALEDoc.h"
#include "ALEView.h"
#include "SignalQualityDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSignalQualityDlg dialog

CSignalQualityDlg::CSignalQualityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignalQualityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSignalQualityDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(CSignalQualityDlg, CDialog)
	//{{AFX_MSG_MAP(CSignalQualityDlg)
	ON_BN_CLICKED(IDC_DELETE_CH, OnDeleteCh)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSignalQualityDlg message handlers

BOOL CSignalQualityDlg::OnInitDialog() 
{
  struct CALEDoc::LQA lqa;
  int size, i, index;
  CString str;

  CDialog::OnInitDialog();

	size = pData->parrLQAs->GetSize();

  ctlList().InsertColumn(0, "Address", LVCFMT_RIGHT, 120);
  ctlList().InsertColumn(1, "Frequency", LVCFMT_RIGHT, 64);
  ctlList().InsertColumn(2, "Rx BER", LVCFMT_RIGHT, 50);
  ctlList().InsertColumn(3, "Rx SNR", LVCFMT_RIGHT, 53);
  ctlList().InsertColumn(4, "Rx Time", LVCFMT_CENTER, 100);
  ctlList().InsertColumn(5, "Tx BER", LVCFMT_RIGHT, 50);
  ctlList().InsertColumn(6, "Tx SINAD", LVCFMT_RIGHT, 61);
  ctlList().InsertColumn(7, "Tx Time", LVCFMT_CENTER, 100);

  for (i = 0; i < size; i++) {
    lqa = pData->parrLQAs->GetAt(i);
    index = ctlList().InsertItem(i, lqa.strOtherAddr);
    str.Format("%lf", lqa.dRxFreq);
    ctlList().SetItemText(index, 1, str);
    str.Format("%d", lqa.RxBER.value);
    ctlList().SetItemText(index, 2, str);
    str.Format("%d", lqa.RxSNR.value);
    ctlList().SetItemText(index, 3, str);
    str = lqa.RxBER.time.Format("%m/%d/%y %H:%M");
    ctlList().SetItemText(index, 4, str);
    str.Format("%d", lqa.TxBER.value);
    ctlList().SetItemText(index, 5, str);
    str.Format("%d", lqa.TxSINAD.value);
    ctlList().SetItemText(index, 6, str);
    str = lqa.TxBER.time.Format("%m/%d/%y %H:%M");
    ctlList().SetItemText(index, 7, str);
  }

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSignalQualityDlg::OnDeleteCh()
{
  int item, size, i;
  double dRxFreq;
  CString strOtherAddr, str;
  struct CALEDoc::LQA lqa;

	POSITION pos = ctlList().GetFirstSelectedItemPosition();
  if (pos == NULL) {
    MessageBox("There isn't anything selected to delete!", "Alert", MB_OK);
  } else {
    while (pos) {
      item = ctlList().GetNextSelectedItem(pos);
      strOtherAddr = ctlList().GetItemText(item, 0);
      str = ctlList().GetItemText(item, 1);
      sscanf((LPCTSTR) str, "%lf", &dRxFreq);

    	size = pData->parrLQAs->GetSize();
      for (i = 0; i < size; i++) {
        lqa = pData->parrLQAs->GetAt(i);
        if ((lqa.strOtherAddr == strOtherAddr) && (lqa.dRxFreq == dRxFreq)) {
          pData->parrLQAs->RemoveAt(i);
          break;
        }
      }

      ctlList().DeleteItem(item);
	    pos = ctlList().GetFirstSelectedItemPosition();
    }
  }
}

void CSignalQualityDlg::LqaModified(UINT wEditType, LONG index)
{
  struct CALEDoc::LQA lqa;
  int size, index2;
  double dRxFreq;
  CString strOtherAddr, str;

  lqa = pData->parrLQAs->GetAt(index);
  if (wEditType == 1) {
    // An lqa record has changed
    // Find the right entry in the list
    size = ctlList().GetItemCount();
    for (index2 = 0; index2 < size; index2++) {
      strOtherAddr = ctlList().GetItemText(index2, 0);
      if (lqa.strOtherAddr == strOtherAddr) {
        str = ctlList().GetItemText(index2, 1);
        sscanf((LPCTSTR) str, "%lf", &dRxFreq);
        if (lqa.dRxFreq == dRxFreq)
          // We have the right entry
          break;
      }
    }
  } else {
    // A new lqa record has been added
    index2 = ctlList().InsertItem(index, lqa.strOtherAddr);
    str.Format("%lf", lqa.dRxFreq);
    ctlList().SetItemText(index2, 1, str);
  }

  str.Format("%d", lqa.RxBER.value);
  ctlList().SetItemText(index2, 2, str);
  str.Format("%d", lqa.RxSNR.value);
  ctlList().SetItemText(index2, 3, str);
  str = lqa.RxBER.time.Format("%m/%d/%y %H:%M");
  ctlList().SetItemText(index2, 4, str);
  str.Format("%d", lqa.TxBER.value);
  ctlList().SetItemText(index2, 5, str);
  str.Format("%d", lqa.TxSINAD.value);
  ctlList().SetItemText(index2, 6, str);
  str = lqa.TxBER.time.Format("%m/%d/%y %H:%M");
  ctlList().SetItemText(index2, 7, str);
}

void CSignalQualityDlg::OnCancel()
{
  DestroyWindow();
}

void CSignalQualityDlg::PostNcDestroy()
{
  view->pSQDlg = NULL;
  delete this;
}
