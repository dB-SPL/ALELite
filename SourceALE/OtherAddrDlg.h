// OtherAddrDlg.h : header file
//  4 Jan 1999

#if !defined(AFX_OTHERADDRDLG_H__AB71A3A2_A3C2_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_OTHERADDRDLG_H__AB71A3A2_A3C2_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// COtherAddrDlg dialog

class COtherAddrDlg : public CDialog
{
private:
  CEdit& ctlOtherAddr() {return *(CEdit *) GetDlgItem(IDC_EDIT1); }
  CEdit& ctlReplyWait() {return *(CEdit *) GetDlgItem(IDC_EDIT3); }
  CListBox& ctlSlotList() {return *(CListBox *) GetDlgItem(IDC_LIST2); }
  CListBox& ctlChannelList() {return *(CListBox *) GetDlgItem(IDC_LIST1); }
  CButton& ctlEditSlots () { return *(CButton*) GetDlgItem(IDC_BUTTON1); }

// Construction
public:
	COtherAddrDlg(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  BOOL bNew;  // TRUE if this is a new record, FALSE to edit existing record
  int pos;  // The position of the record to edit or where to add a new record

  CStringArray LocalSlotStrings;

// Dialog Data
	//{{AFX_DATA(COtherAddrDlg)
	enum { IDD = IDD_OTHADDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COtherAddrDlg)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COtherAddrDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnEditSlots();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERADDRDLG_H__AB71A3A2_A3C2_11D2_8F3A_0060089F9D1C__INCLUDED_)
