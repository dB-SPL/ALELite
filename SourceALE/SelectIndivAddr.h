// SelectIndivAddr.h : header file
//  9 Jul 1999

#if !defined(AFX_SELECTINDIVADDR_H__C7DE9D61_E5F1_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SELECTINDIVADDR_H__C7DE9D61_E5F1_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelectIndivAddr dialog

class CSelectIndivAddr : public CDialog
{
private:
  CListBox& ctlToList() { return *(CListBox*) GetDlgItem(IDC_LIST1); }
  CListBox& ctlSelfList() { return *(CListBox*) GetDlgItem(IDC_LIST2); }

// Construction
public:
	CSelectIndivAddr(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  int iOtherAddr, iSelfAddr, iChannel;
  CHANNELMODE eChannelMode;

// Dialog Data
	//{{AFX_DATA(CSelectIndivAddr)
	enum { IDD = IDD_SELINDIV };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectIndivAddr)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectIndivAddr)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTINDIVADDR_H__C7DE9D61_E5F1_11D2_8F3A_0060089F9D1C__INCLUDED_)
