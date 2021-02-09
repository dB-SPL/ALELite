// AMDDisplay.h : header file
//  12 Oct 99

#if !defined(AFX_AMDDISPLAY_H__8E74B801_7C9D_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_AMDDISPLAY_H__8E74B801_7C9D_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAMDDisplay dialog

class CAMDDisplay : public CDialog
{
private:
  CRichEditCtrl& ctlEdit() { return *(CRichEditCtrl*) GetDlgItem(IDC_RICHEDIT1); }

// Construction
public:
	CAMDDisplay(CWnd* pParent = NULL);   // standard constructor
  void AMDModified(LONG index);

  // Dialog Data
  struct CALEDoc::DATA *pData;
  CALEView* view;

	//{{AFX_DATA(CAMDDisplay)
	enum { IDD = IDD_AMD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMDDisplay)
	//}}AFX_VIRTUAL

// Implementation
private:
  int iMsg;  // index to where new text will go in the RichEdit control

protected:
  virtual void OnCancel();
  virtual void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(CAMDDisplay)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMDDISPLAY_H__8E74B801_7C9D_11D3_8F3A_0060089F9D1C__INCLUDED_)
