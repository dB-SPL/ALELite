// SelectRxChannel.h : header file
//  3 Nov 1999

#if !defined(AFX_SELECTRXCHANNEL_H__A544D901_2407_11D3_8F3A_0060089F9D1C__INCLUDED_)
#define AFX_SELECTRXCHANNEL_H__A544D901_2407_11D3_8F3A_0060089F9D1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSelectRxChannel dialog

class CSelectRxChannel : public CDialog
{
private:
  CListCtrl& ctlList() { return *(CListCtrl*) GetDlgItem(IDC_LIST1); }

// Construction
public:
	CSelectRxChannel(CWnd* pParent = NULL);   // standard constructor

  struct CALEDoc::DATA *pData;
  int iSelfAddrMode;  // There are 3 cases when this dialog is called
                      // iSelfAddrMode = 0 when all channels are displayed
                      // iSelfAddrMode = 1 when only the channels associated
                      //   with iSelfAddr are displayed
                      // iSelfAddrMode = 2 when only the common channels
                      //   associated with both iSelfAddr and iOtherAddr are displayed
  int iSelfAddr;
  int iOtherAddr;
  int iChanNum;

// Dialog Data
	//{{AFX_DATA(CSelectRxChannel)
	enum { IDD = IDD_RXCHANNEL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectRxChannel)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectRxChannel)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTRXCHANNEL_H__A544D901_2407_11D3_8F3A_0060089F9D1C__INCLUDED_)
