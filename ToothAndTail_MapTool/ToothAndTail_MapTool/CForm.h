#pragma once



// CForm 폼 보기
//#include <afxext.h>

class CTab1_Tile;
class CTab2_Deco;

class CForm : public CFormView
{
	DECLARE_DYNCREATE(CForm)

protected:
	CForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	
public:
	pair<MAP_OBJ::E_TYPE, wstring> GetSelectedObjKey(void);

	CTab1_Tile* m_pTab1_Tile = nullptr;
	CTab2_Deco* m_pTab2_Deco = nullptr;
	CTabCtrl m_TabCtrl;

public:
	virtual void OnInitialUpdate();
	virtual void PostNcDestroy();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	
	CStatic m_PicView;
};


