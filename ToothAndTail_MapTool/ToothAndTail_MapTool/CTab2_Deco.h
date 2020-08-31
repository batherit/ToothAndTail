#pragma once


// CTab2_Deco 대화 상자

class CTab2_Deco : public CDialogEx
{
	DECLARE_DYNAMIC(CTab2_Deco)

public:
	CTab2_Deco(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab2_Deco();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB2_DECO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
