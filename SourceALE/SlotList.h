// SlotList.h : header file
//  13 Jan 1999

#if !defined(AFX_SLOTLIST_H__4D577201_AAD8_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SLOTLIST_H__4D577201_AAD8_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSlotList dialog

class CSlotList : public CDialog
{
private:
  CListCtrl& ctlList () { return *(CListCtrl*) GetDlgItem(IDC_LIST1); }
  CBitmapButton m_ctlMoveUp;
  CBitmapButton m_ctlMoveDown;

  void DispSlotList();

// Construction
public:
	CSlotList(CWnd* pParent = NULL);   // standard constructor

  CStringArray *pSlotStrings;

  // Dialog Data
	//{{AFX_DATA(CSlotList)
	enum { IDD = IDD_SLOT_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlotList)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSlotList)
	afx_msg void OnAddSaddr();
	afx_msg void OnDeleteSaddr();
	afx_msg void OnEditSaddr();
	afx_msg void OnMovedown();
	afx_msg void OnMoveup();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLOTLIST_H__4D577201_AAD8_11D2_8F3A_0060089F9D1C__INCLUDED_)
