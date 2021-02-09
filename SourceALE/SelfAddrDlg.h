// SelfAddrDlg.h : header file
//  11 Dec 1998

#if !defined(AFX_SELFADDRDLG_H__E7D694C1_807E_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SELFADDRDLG_H__E7D694C1_807E_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelfAddrDlg dialog

class CSelfAddrDlg : public CDialog
{
private:
  CEdit& ctlSelfAddr() {return *(CEdit *) GetDlgItem(IDC_EDIT1); }
  CEdit& ctlNetAddr() {return *(CEdit *) GetDlgItem(IDC_EDIT2); }
  CEdit& ctlSlotWait() {return *(CEdit *) GetDlgItem(IDC_EDIT3); }
  CListBox& ctlChannelList() {return *(CListBox *) GetDlgItem(IDC_LIST1); }

// Construction
public:
	CSelfAddrDlg(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  BOOL bNew;  // TRUE if this is a new record, FALSE to edit existing record
  int pos;  // The position of the record to edit or where to add a new record

// Dialog Data
	//{{AFX_DATA(CSelfAddrDlg)
	enum { IDD = IDD_SELFADDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfAddrDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelfAddrDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFADDRDLG_H__E7D694C1_807E_11D2_8F3A_0060089F9D1C__INCLUDED_)
