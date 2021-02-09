// ChannelList.h : header file
//  30 Mar 1998

#if !defined(AFX_CHANNELLIST_H__A3C17B64_3849_11D2_8F39_0060089F9D1C__INCLUDED_)
#define AFX_CHANNELLIST_H__A3C17B64_3849_11D2_8F39_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChannelList dialog

class CChannelList : public CDialog
{
private:
  CListCtrl& ctlList () { return *(CListCtrl*) GetDlgItem(IDC_LIST1); }
  CButton& ctlEdit () { return *(CButton*) GetDlgItem(IDC_EDIT_CHAN); }
  CButton& ctlAdd () { return *(CButton*) GetDlgItem(IDC_ADD_CHAN); }
  CButton& ctlDelete () { return *(CButton*) GetDlgItem(IDC_DELETE_CHAN); }
  CBitmapButton m_ctlMoveUp;
  CBitmapButton m_ctlMoveDown;

  void DispChannelList();

// Construction
public:
	CChannelList(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;

// Dialog Data
	//{{AFX_DATA(CChannelList)
	enum { IDD = IDD_CHANNEL_LIST };
    // NOTE: the ClassWizard will add members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelList)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChannelList)
	virtual BOOL OnInitDialog();
	afx_msg void OnDeleteChan();
	afx_msg void OnAddChan();
	afx_msg void OnEditChan();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELLIST_H__A3C17B64_3849_11D2_8F39_0060089F9D1C__INCLUDED_)
