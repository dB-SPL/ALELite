// SlotAddr.h : header file
//  13 Jan 1999

#if !defined(AFX_SLOTADDR_H__4D577202_AAD8_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SLOTADDR_H__4D577202_AAD8_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSlotAddr dialog

class CSlotAddr : public CDialog
{
private:
  CEdit& ctlSlotAddr() {return *(CEdit *) GetDlgItem(IDC_EDIT1); }

// Construction
public:
	CSlotAddr(CWnd* pParent = NULL);   // standard constructor

  CStringArray *pSlotStrings;
  BOOL bNew;  // TRUE if this is a new record, FALSE to edit existing record
  int pos;  // The position of the string to edit or where to add a new string

  // Dialog Data
	//{{AFX_DATA(CSlotAddr)
	enum { IDD = IDD_SLOTADDR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSlotAddr)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSlotAddr)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SLOTADDR_H__4D577202_AAD8_11D2_8F3A_0060089F9D1C__INCLUDED_)
