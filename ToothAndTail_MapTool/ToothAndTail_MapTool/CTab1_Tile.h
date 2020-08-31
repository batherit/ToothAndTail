#pragma once


// CTab1_Tile 대화 상자

class CTab1_Tile : public CDialogEx
{
	DECLARE_DYNAMIC(CTab1_Tile)

public:
	CTab1_Tile(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTab1_Tile();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB1_TILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
};
