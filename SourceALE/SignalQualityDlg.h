// SignalQualityDlg.h : header file
//  12 May 1999

#if !defined(AFX_SIGNALQUALITYDLG_H__8AB1E944_06E1_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SIGNALQUALITYDLG_H__8AB1E944_06E1_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CALEView;

/////////////////////////////////////////////////////////////////////////////
// CSignalQualityDlg dialog

class CSignalQualityDlg : public CDialog
{
private:
  CListCtrl& ctlList() { return *(CListCtrl*) GetDlgItem(IDC_LIST1); }

// Construction
public:
	CSignalQualityDlg(CWnd* pParent = NULL);   // standard constructor
  void LqaModified(UINT wEditType, LONG index);

// Dialog Data
  struct CALEDoc::DATA *pData;
  CALEView* view;

	//{{AFX_DATA(CSignalQualityDlg)
	enum { IDD = IDD_SIGNALQUALITY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSignalQualityDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:
  virtual void OnCancel();
  virtual void PostNcDestroy();
	// Generated message map functions
	//{{AFX_MSG(CSignalQualityDlg)
	afx_msg void OnDeleteCh();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIGNALQUALITYDLG_H__8AB1E944_06E1_11D3_8F3A_0060089F9D1C__INCLUDED_)
