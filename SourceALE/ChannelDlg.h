// ChannelDlg.h : header file
//  17 Nov 1998
#if !defined(AFX_CHANNELDLG_H__22A4BD01_7AFA_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_CHANNELDLG_H__22A4BD01_7AFA_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChannelDlg dialog

class CChannelDlg : public CDialog
{
private:
  CEdit& ctlTxFreq() {return *(CEdit *) GetDlgItem(IDC_EDIT1); }
  CComboBox& ctlComboTxMode() {return *(CComboBox *) GetDlgItem(IDC_COMBO1); }
  CEdit& ctlRxFreq() {return *(CEdit *) GetDlgItem(IDC_EDIT2); }
  CComboBox& ctlComboRxMode() {return *(CComboBox *) GetDlgItem(IDC_COMBO2); }
  CComboBox& ctlComboTRMode() {return *(CComboBox *) GetDlgItem(IDC_COMBO3); }
  CComboBox& ctlComboScanMode() {return *(CComboBox *) GetDlgItem(IDC_COMBO4); }
  CEdit& ctlSoundInt() {return *(CEdit *) GetDlgItem(IDC_EDIT3); }

// Construction
public:
	CChannelDlg(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  BOOL bNew;  // TRUE if this is a new record, FALSE to edit existing record
  int pos;  // The position of the record to edit or where to add a new record

// Dialog Data
	//{{AFX_DATA(CChannelDlg)
	enum { IDD = IDD_CHANNEL };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChannelDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELDLG_H__22A4BD01_7AFA_11D2_8F3A_0060089F9D1C__INCLUDED_)
