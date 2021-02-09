// CalDlg.h : header file
//

#if !defined(AFX_CALDLG_H__DF634695_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
#define AFX_CALDLG_H__DF634695_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCalibrateView;

typedef struct tagRXTHREADPARMS {
  // Add any parameters to be passed to the thread
  CWnd *pWnd;
  BOOL bContinue;
  double value;  // This is the value passed from the thread back
} RXTHREADPARMS;

/////////////////////////////////////////////////////////////////////////////
// CCalDlg dialog

class CCalDlg : public CDialog
{
private:
	CStatic& ctlText0() {return *(CStatic *) GetDlgItem(IDC_STATIC0); }
	CStatic& ctlText3() {return *(CStatic *) GetDlgItem(IDC_STATIC3); }
	CStatic& ctlText5() {return *(CStatic *) GetDlgItem(IDC_STATIC5); }
	CStatic& ctlText1() {return *(CStatic *) GetDlgItem(IDC_STATIC1); }
	CStatic& ctlText4() {return *(CStatic *) GetDlgItem(IDC_STATIC4); }
	CButton& ctlButton1() {return *(CButton *) GetDlgItem(IDC_BUTTON1); }
	CButton& ctlButton2() {return *(CButton *) GetDlgItem(IDC_BUTTON2); }
	double dSigInPwr, dNoise;
// Construction
public:
  CCalDlg(CWnd* pParent = NULL);   // standard constructor
  void UpdateValue();

// Dialog Data
private:
  CWinThread* pRxThread;

public:
  CCalibrateView* view;
  RXTHREADPARMS rxthreadParms;

	//{{AFX_DATA(CCalDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALDLG_H__DF634695_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
