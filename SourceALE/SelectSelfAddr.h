// SelectSelfAddr.h : header file
//  4 Oct 99

#if !defined(AFX_SELECTSELFADDR_H__67B76E43_7A60_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SELECTSELFADDR_H__67B76E43_7A60_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelectSelfAddr dialog

class CSelectSelfAddr : public CDialog
{
private:
  CListBox& ctlSelfList() { return *(CListBox*) GetDlgItem(IDC_LIST1); }

// Construction
public:
	CSelectSelfAddr(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  int iSelfAddr, iChannel;

  // Dialog Data
	//{{AFX_DATA(CSelectSelfAddr)
	enum { IDD = IDD_SELSELFADDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectSelfAddr)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectSelfAddr)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSELFADDR_H__67B76E43_7A60_11D3_8F3A_0060089F9D1C__INCLUDED_)
