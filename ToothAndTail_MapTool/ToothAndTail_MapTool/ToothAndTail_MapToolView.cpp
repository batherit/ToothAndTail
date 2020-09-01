
// ToothAndTail_MapToolView.cpp: CToothAndTailMapToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "ToothAndTail_MapTool.h"
#endif

#include "ToothAndTail_MapToolDoc.h"
#include "ToothAndTail_MapToolView.h"
#include "MainFrm.h"
#include "CTextureMgr.h"
#include "CMapEditor.h"
#include "CCamera.h"
#include "CForm.h"
#include "CTile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToothAndTailMapToolView
HWND g_hWND;
IMPLEMENT_DYNCREATE(CToothAndTailMapToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToothAndTailMapToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CToothAndTailMapToolView 생성/소멸

CToothAndTailMapToolView::CToothAndTailMapToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToothAndTailMapToolView::~CToothAndTailMapToolView()
{
}

BOOL CToothAndTailMapToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToothAndTailMapToolView 그리기

void CToothAndTailMapToolView::OnDraw(CDC* /*pDC*/)
{
	CToothAndTailMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	float fNewX = static_cast<float>(GetScrollPos(0));
	float fNewY = static_cast<float>(GetScrollPos(1));

	m_pMapEditor->GetMainCamera()->SetXY(fNewX, fNewY);
	m_pMapEditor->Render();
}
//&RECT({ 0, 0, WINCX, WINCY })

// CToothAndTailMapToolView 인쇄

BOOL CToothAndTailMapToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToothAndTailMapToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToothAndTailMapToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToothAndTailMapToolView 진단

#ifdef _DEBUG
void CToothAndTailMapToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToothAndTailMapToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToothAndTailMapToolDoc* CToothAndTailMapToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToothAndTailMapToolDoc)));
	return (CToothAndTailMapToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToothAndTailMapToolView 메시지 처리기


void CToothAndTailMapToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// Scroll Size 세팅
	SetScrollSizes(MM_TEXT, CSize(MAP_WIDTH + (MAP_WIDTH >> 1), MAP_HEIGHT + (MAP_HEIGHT >> 1)));
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// Gap을 구하기 위해서 MainFrame 크기를 구한다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};
	pMain->GetWindowRect(&rcMain);														// MainFrame 렉트를 구하고,
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);		// 렉트 시작 위치를 (0, 0)으로 변환

	// Gap을 구하기 위해서 View 크기를 구한다.
	RECT rcView = {};
	GetClientRect(&rcView); ;
	float fGapX = rcMain.right - float(rcView.right);
	float fGapY = WINCY + (float(rcMain.bottom) - rcView.bottom);

	pMain->SetWindowPos(nullptr, 0, 0, LONG(WINCX + fGapX), LONG(fGapY), SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	// 장치 생성
	g_hWND = m_hWnd;
	if (FAILED(CGraphicDevice::GetInstance()->GenerateGraphicDevice()))
	{
		ERR_MSG(L"Failed to create the hareware device.");
		return;
	}
	// Map Editor 생성
	m_pMapEditor = new CMapEditor();
	m_pMapEditor->Ready();
}


BOOL CToothAndTailMapToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (zDelta > 0) {
		m_pMapEditor->GetMainCamera()->ZoomIn(0.05f);
	}
	else {
		m_pMapEditor->GetMainCamera()->ZoomOut(0.05f);
	}
	InvalidateRect(nullptr, 0);
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CToothAndTailMapToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// TODO: 타일 클릭시 반응을 처리합니다.
	m_pMapEditor->OnLButtonDown(nFlags, point);

	InvalidateRect(nullptr, 0);
	CScrollView::OnLButtonDown(nFlags, point);
}


void CToothAndTailMapToolView::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = WINCX; 
	lpMMI->ptMinTrackSize.y = WINCY; 
	lpMMI->ptMaxTrackSize.x = WINCX; 
	lpMMI->ptMaxTrackSize.y = WINCY;

	CScrollView::OnGetMinMaxInfo(lpMMI);
}


void CToothAndTailMapToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	//CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	//POINT ptCursorPoint = GetClientCursorPoint(g_hWND);

	if (m_bIsDragScrolling) {
		POINT ptCurClickedPoint = GetClientCursorPoint(GetSafeHwnd());
		CCamera* pCamera = m_pMapEditor->GetMainCamera();
		POINT ptDeltaMove = {
			(m_ptClickedPoint.x - ptCurClickedPoint.x) / pCamera->GetZoomMultiple(),
			(m_ptClickedPoint.y - ptCurClickedPoint.y) / pCamera->GetZoomMultiple()
		};
		SetScrollPos(0, ptDeltaMove.x + GetScrollPos(0));
		SetScrollPos(1, ptDeltaMove.y + GetScrollPos(1));
		m_ptClickedPoint = ptCurClickedPoint;
	}

	InvalidateRect(nullptr, 0);
	CScrollView::OnMouseMove(nFlags, point);
}


void CToothAndTailMapToolView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bIsDragScrolling = true;
	m_ptClickedPoint = GetClientCursorPoint(GetSafeHwnd());
	CScrollView::OnRButtonDown(nFlags, point);
}


void CToothAndTailMapToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_bIsDragScrolling = false;
	CScrollView::OnRButtonUp(nFlags, point);
}


void CToothAndTailMapToolView::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SafelyDeleteObj(m_pMapEditor);
	CGraphicDevice::GetInstance()->DestroyInstance();

	CScrollView::PostNcDestroy();
}
