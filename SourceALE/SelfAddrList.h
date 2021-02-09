// SelfAddrList.h : header file
//  20 Nov 1998

#if !defined(AFX_SELFADDRLIST_H__602ADF42_7FBB_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SELFADDRLIST_H__602ADF42_7FBB_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelfAddrList dialog

class CSelfAddrList : public CDialog
{
private:
  CListCtrl& ctlList () { return *(CListCtrl*) GetDlgItem(IDC_LIST1); }
//  CButton& ctlEdit () { return *(CButton*) GetDlgItem(IDC_EDIT_SADDR); }
//  CButton& ctlAdd () { return *(CButton*) GetDlgItem(IDC_ADD_SADDR); }
//  CButton& ctlDelete () { return *(CButton*) GetDlgItem(IDC_DELETE_SADDR); }
  CBitmapButton m_ctlMoveUp;
  CBitmapButton m_ctlMoveDown;

  void AddAddrList(int);
  void DispSelfAddrList();

// Construction
public:
	CSelfAddrList(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;

// Dialog Data
	//{{AFX_DATA(CSelfAddrList)
	enum { IDD = IDD_SELFADDR_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelfAddrList)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelfAddrList)
	virtual BOOL OnInitDialog();
	afx_msg void OnMovedown();
	afx_msg void OnMoveup();
	afx_msg void OnAddSaddr();
	afx_msg void OnDeleteSaddr();
	afx_msg void OnEditSaddr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELFADDRLIST_H__602ADF42_7FBB_11D2_8F3A_0060089F9D1C__INCLUDED_)
