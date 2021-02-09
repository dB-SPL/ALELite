// OtherAddrList.h : header file
//  4 Jan 1999

#if !defined(AFX_OTHERADDRLIST_H__AB71A3A1_A3C2_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_OTHERADDRLIST_H__AB71A3A1_A3C2_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// COtherAddrList dialog

class COtherAddrList : public CDialog
{
  CListCtrl& ctlList () { return *(CListCtrl*) GetDlgItem(IDC_LIST1); }
  CButton& ctlEdit () { return *(CButton*) GetDlgItem(IDC_EDIT_SADDR); }
  CButton& ctlAdd () { return *(CButton*) GetDlgItem(IDC_ADD_SADDR); }
  CButton& ctlDelete () { return *(CButton*) GetDlgItem(IDC_DELETE_SADDR); }
  CBitmapButton m_ctlMoveUp;
  CBitmapButton m_ctlMoveDown;

  void AddOtherAddrList(int);
  void DispOtherAddrList();

// Construction
public:
	COtherAddrList(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;

// Dialog Data
	//{{AFX_DATA(COtherAddrList)
	enum { IDD = IDD_OTHADDR_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COtherAddrList)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COtherAddrList)
	virtual BOOL OnInitDialog();
	afx_msg void OnMovedown();
	afx_msg void OnMoveup();
	afx_msg void OnAddOaddr();
	afx_msg void OnDeleteOaddr();
	afx_msg void OnEditOaddr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERADDRLIST_H__AB71A3A1_A3C2_11D2_8F3A_0060089F9D1C__INCLUDED_)
