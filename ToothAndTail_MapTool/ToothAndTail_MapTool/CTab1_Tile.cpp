// CTab1_Tile.cpp: 구현 파일
//

#include "stdafx.h"
#include "ToothAndTail_MapTool.h"
#include "CTab1_Tile.h"
#include "afxdialogex.h"


// CTab1_Tile 대화 상자

IMPLEMENT_DYNAMIC(CTab1_Tile, CDialogEx)

CTab1_Tile::CTab1_Tile(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB1_TILE, pParent)
{

}

CTab1_Tile::~CTab1_Tile()
{
}

void CTab1_Tile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTab1_Tile, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO1, &CTab1_Tile::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CTab1_Tile 메시지 처리기


void CTab1_Tile::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
