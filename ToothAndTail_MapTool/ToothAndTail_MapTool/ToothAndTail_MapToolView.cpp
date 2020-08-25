
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToothAndTailMapToolView
HWND g_hWND;
IMPLEMENT_DYNCREATE(CToothAndTailMapToolView, CView)

BEGIN_MESSAGE_MAP(CToothAndTailMapToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEWHEEL()
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

	return CView::PreCreateWindow(cs);
}

// CToothAndTailMapToolView 그리기

void CToothAndTailMapToolView::OnDraw(CDC* /*pDC*/)
{
	CToothAndTailMapToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CGraphicDevice::GetInstance()->BeginRender();
	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// 베이스 맵 그리기
	const TextureInfo* pTextureInfo = CTextureMgr::GetInstance()->GetTextureInfo(L"T_BASE_MAP");
	float fCenterX = float(pTextureInfo->tImageInfo.Width >> 1);
	float fCenterY = float(pTextureInfo->tImageInfo.Height >> 1);
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXMatrixScaling(&matScale, static_cast<float>(WINCX) / pTextureInfo->tImageInfo.Width, static_cast<float>(WINCY) / pTextureInfo->tImageInfo.Height, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX >> 1, WINCY >> 1, 0);
	matWorld = matScale * matTrans;
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
	CGraphicDevice::GetInstance()->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CGraphicDevice::GetInstance()->EndRender();
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
	CView::AssertValid();
}

void CToothAndTailMapToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	RECT rcMain = {};

	// Gap을 구하기 위해서 MainFrame 크기를 구한다.
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

	// 베이스 맵 텍스쳐 생성
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"T_BASE_MAP")))
		return;



}


BOOL CToothAndTailMapToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
