// SelectSoundAddrs.h : header file
//  5 Apr 1999

#if !defined(AFX_SELECTSOUNDADDRS_H__E2C077CE_E8FD_11D2_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SELECTSOUNDADDRS_H__E2C077CE_E8FD_11D2_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelectSoundAddrs dialog

class CSelectSoundAddrs : public CDialog
{
private:
  CListBox& ctlList() { return *(CListBox*) GetDlgItem(IDC_LIST1); }

// Construction
public:
	CSelectSoundAddrs(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;

// Dialog Data
	//{{AFX_DATA(CSelectSoundAddrs)
	enum { IDD = IDD_SOUNDADDRS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectSoundAddrs)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectSoundAddrs)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTSOUNDADDRS_H__E2C077CE_E8FD_11D2_8F3A_0060089F9D1C__INCLUDED_)
