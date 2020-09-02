// CForm.cpp: 구현 파일
//

//#include "pch.h"
#include "stdafx.h"
#include "ToothAndTail_MapTool.h"
#include "CForm.h"
#include "CTab1_Tile.h"
#include "CTab2_Deco.h"
#include "CMapEditor.h"


// CForm

IMPLEMENT_DYNCREATE(CForm, CFormView)

CForm::CForm()
	: CFormView(IDD_FORM)
{

}

CForm::~CForm()
{
}

void CForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Control(pDX, IDC_PIC_SELECTED, m_PicView);
}

BEGIN_MESSAGE_MAP(CForm, CFormView)
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CForm::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON1, &CForm::OnBnClickedSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CForm::OnBnClickedLoad)
END_MESSAGE_MAP()


// CForm 진단

#ifdef _DEBUG
void CForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CForm 메시지 처리기


void CForm::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = FORMCX;
	lpMMI->ptMinTrackSize.y = FORMCY;
	lpMMI->ptMaxTrackSize.x = FORMCX;
	lpMMI->ptMaxTrackSize.y = FORMCY;

	CFormView::OnGetMinMaxInfo(lpMMI);
}

MAP_OBJ::E_TYPE CForm::GetSelectedTab()
{
	int iSelectedTabIndex = m_TabCtrl.GetCurSel();
	
	if (0 == iSelectedTabIndex) return MAP_OBJ::TYPE_TILE;
	else if (1 == iSelectedTabIndex) return MAP_OBJ::TYPE_DECO;
	
	return MAP_OBJ::TYPE_NONE;
}

int CForm::GetTileType()
{
	return m_pTab1_Tile->GetTileType();
}

pair<wstring, int> CForm::GetDecoType()
{
	return m_pTab2_Deco->GetDecoType();
}

void CForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	// 두 개의 탭을 만든다.
	m_TabCtrl.InsertItem(0, L"Tile");
	m_TabCtrl.InsertItem(1, L"Deco");

	CRect rcFormView;
	m_TabCtrl.GetWindowRect(&rcFormView);

	m_pTab2_Deco = new CTab2_Deco;
	m_pTab2_Deco->Create(IDD_TAB2_DECO, &m_TabCtrl);
	m_pTab2_Deco->MoveWindow(0, 25, rcFormView.Width(), rcFormView.Height());
	m_pTab2_Deco->ShowWindow(SW_SHOW);

	m_pTab1_Tile = new CTab1_Tile;
	m_pTab1_Tile->Create(IDD_TAB1_TILE, &m_TabCtrl);
	m_pTab1_Tile->MoveWindow(0, 25, rcFormView.Width(), rcFormView.Height());
	m_pTab1_Tile->ShowWindow(SW_SHOW);

	
	
	// 초기 탭은 0번째로 선택한다.
	m_TabCtrl.SetCurSel(0);
}


void CForm::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SafelyDeleteObj(m_pTab1_Tile);
	SafelyDeleteObj(m_pTab2_Deco);

	CFormView::PostNcDestroy();
}


void CForm::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 탭이 변경되면 다른 탭의 대화상자는 숨긴다.
	int iSelectedTabIndex = m_TabCtrl.GetCurSel();

	switch (iSelectedTabIndex) {
	case 0:
		m_pTab1_Tile->ShowWindow(SW_SHOW);
		m_pTab2_Deco->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_pTab1_Tile->ShowWindow(SW_HIDE);
		m_pTab2_Deco->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}


//BOOL CForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
//}


void CForm::OnBnClickedSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"정말로 저장하시겠습니까?", L"저장", MB_OK);
	m_pMapEditor->SaveInfo();
}


void CForm::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MessageBox(L"정말로 로드하시겠습니까?", L"로드", MB_OK);
	m_pMapEditor->LoadInfo();
}
