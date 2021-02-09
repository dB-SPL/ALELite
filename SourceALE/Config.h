// Config.h : header file
//  30 Jun 1999

#if !defined(AFX_CONFIG_H__1641AFC1_2A20_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_CONFIG_H__1641AFC1_2A20_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog

class CConfig : public CDialog
{
private:
  CComboBox& ctlComboRadio() {return *(CComboBox *) GetDlgItem(IDC_COMBO1); }
  CEdit& ctlRadioAddr() {return *(CEdit *) GetDlgItem(IDC_RADIOADDR); }
  CComboBox& ctlComboCommPort() {return *(CComboBox *) GetDlgItem(IDC_COMBO2); }
  CComboBox& ctlComboBaud() {return *(CComboBox *) GetDlgItem(IDC_COMBO3); }

  // Construction
public:
	CConfig(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  HANDLE hComm;

  // Dialog Data
	//{{AFX_DATA(CConfig)
	enum { IDD = IDD_CONFIG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfig)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIG_H__1641AFC1_2A20_11D3_8F3A_0060089F9D1C__INCLUDED_)
