// CalibrateDoc.h : interface of the CCalibrateDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALIBRATEDOC_H__DF63468B_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
#define AFX_CALIBRATEDOC_H__DF63468B_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
class CCalibrateDoc : public CDocument
{
protected: // create from serialization only
	CCalibrateDoc();
	DECLARE_DYNCREATE(CCalibrateDoc)

// Attributes
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalibrateDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCalibrateDoc();
  void DataAvail();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCalibrateDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALIBRATEDOC_H__DF63468B_AD5F_11D3_B8DB_0080C84B206B__INCLUDED_)
