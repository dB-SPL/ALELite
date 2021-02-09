// AMDText.h : header file
//  13 Oct 1999

#if !defined(AFX_AMDTEXT_H__189C4849_80A9_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_AMDTEXT_H__189C4849_80A9_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAMDText dialog

class CAMDText : public CDialog
{
private:
  CEdit& ctlText() { return *(CEdit*) GetDlgItem(IDC_EDIT1); }

// Construction
public:
	CAMDText(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  CString strAMDText;

  // Dialog Data
	//{{AFX_DATA(CAMDText)
	enum { IDD = IDD_AMDMESSAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMDText)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAMDText)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMDTEXT_H__189C4849_80A9_11D3_8F3A_0060089F9D1C__INCLUDED_)
