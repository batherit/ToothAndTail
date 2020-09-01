#include "stdafx.h"
#include "CMapEditor.h"
#include "CCamera.h"
#include "CTextureMgr.h"
#include "CSpriteObj.h"
#include "CTile.h"
#include "CDeco.h"
#include "MainFrm.h"
#include "CForm.h"
#include "ToothAndTail_MapToolView.h"
#include "CTab2_Deco.h"


CMapEditor::CMapEditor()
{
}


CMapEditor::~CMapEditor()
{
	Release();
}

//LRESULT CMapEditor::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
//{
//	return LRESULT();
//}

void CMapEditor::Ready(void)
{
	LinkView();
	LoadTextures();

	// 맵 스프라이트 객체 생성
	m_pMap = new CSpriteObj(*this, (MAP_WIDTH >> 1), (MAP_HEIGHT >> 1), MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));

	// Grid Tiles 생성
	m_iMapRow = (MAP_HEIGHT << 1) / TILE_HEIGHT + 1;
	m_iMapCol = MAP_WIDTH / TILE_WIDTH + 1;
	D3DXVECTOR3 vPoint;
	for (int i = 0; i < m_iMapRow; ++i) {
		for (int j = 0; j < m_iMapCol; ++j) {
			vPoint.x = static_cast<float>((TILE_WIDTH * j) + ((i % 2)* (TILE_WIDTH >> 1)));
			vPoint.y = static_cast<float>((TILE_HEIGHT >> 1) * i);
			vPoint.z = 0.f;

			if (IsPointInPolygon(vPoint, m_MapBorderLines, 4)) {
				m_listTiles.emplace_back(new CTile(*this, vPoint.x, vPoint.y));
			}

			// 맵 경계 밖에 있는 좌표에 대해서는 타일을 추가하지 않는다.
			/*else {
				m_vecGrid.emplace_back(new CTile(*this, vPoint.x, vPoint.y, CTile::TYPE_BLOCKING));
			}*/
		}
	}

	// 카메라 생성
	m_pCamera = new CCamera(*this, nullptr, 0.f, 0.f);
	SetMainCamera(m_pCamera);		//.. 메인 카메라 세팅
}

void CMapEditor::Update(void)
{
}

void CMapEditor::LateUpdate(void)
{
}

void CMapEditor::Render(void)
{
	StartRender();

	// TODO : 그리기 연산을 수행합니다.
	RenderMap();
	RenderTiles();
	RenderDecos();
	RenderSelectedObj();
	
	TCHAR szBuf[MAX_PATH];
	POINT ptMouseCursorS = GetClientCursorPoint(g_hWND);
	D3DXVECTOR3 vMouseCursorW = m_pCamera->GetWorldPoint(D3DXVECTOR3(static_cast<FLOAT>(ptMouseCursorS.x), static_cast<FLOAT>(ptMouseCursorS.y), 0.f));
	swprintf_s(szBuf, L"%f %f", vMouseCursorW.x, vMouseCursorW.y);
	D3DXMATRIX matFont;
	D3DXMatrixScaling(&matFont, 3.f, 3.f, 0.f);
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matFont);
	CGraphicDevice::GetInstance()->GetFont()->DrawTextW(CGraphicDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	EndRender();
}

void CMapEditor::Release(void)
{
	SafelyDeleteObj(m_pMap);
	SafelyDeleteObj(m_pCamera);
	SafelyDeleteObjs(m_listTiles);
	SafelyDeleteObjs(m_listDecos);
	CTextureMgr::DestroyInstance();
}

void CMapEditor::OnLButtonDown(UINT nFlags, CPoint point)
{
	POINT ptCursorS = GetClientCursorPoint(m_pToolView->GetSafeHwnd());
	D3DXVECTOR3 vCursorW = GetMainCamera()->GetWorldPoint(D3DXVECTOR3(static_cast<FLOAT>(ptCursorS.x), static_cast<FLOAT>(ptCursorS.y), 0.f));
	switch (m_pForm->GetSelectedTab())
	{
	case MAP_OBJ::TYPE_TILE: {
		for (auto& pTile : m_listTiles) {
			if (IsPointInTile(vCursorW, pTile->GetXY(), pTile->GetWidth(), pTile->GetHeight())) {
				// 선택된 타일의 타입을 변경한다.
				pTile->SetTileType(static_cast<CTile::E_TYPE>(m_pForm->GetTileType()));
				break;
			}
		}
	}
		break;
	case MAP_OBJ::TYPE_DECO: {
		auto& rDecoType = m_pForm->GetDecoType();
		if (rDecoType.second != -1) {
			m_listDecos.emplace_back(new CDeco(*this, vCursorW.x, vCursorW.y, rDecoType.first, rDecoType.second));
			m_listDecos.sort([](CDeco* pDeco1, CDeco* pDeco2) {
				return pDeco1->GetBottom() < pDeco2->GetBottom();
			});
		}
		else {
			std::list<CDeco*>::reverse_iterator rIter = m_listDecos.rbegin();
			for (; rIter != m_listDecos.rend(); ++rIter) {
				if (IsPointInRect((*rIter)->GetRect(), vCursorW)) {
					break;
				}
			}
			if (rIter != m_listDecos.rend()) {
				SafelyDeleteObj(*rIter);
				m_listDecos.erase(std::next(rIter).base());
			}
		}
	}
		break;
	default:
		break;
	}
}

void CMapEditor::RenderMap(void)
{
	m_pMap->Render(m_pCamera);
}

void CMapEditor::RenderTiles(void)
{
	TCHAR szBuf[MAX_PATH] = L"";
	int iIndex = 0;
	for (auto& pTile : m_listTiles) {
		pTile->Render(m_pCamera);
		swprintf_s(szBuf, L"%d", iIndex++);
		CGraphicDevice::GetInstance()->GetFont()->DrawTextW(CGraphicDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CMapEditor::RenderDecos(void)
{
	TCHAR szBuf[MAX_PATH] = L"";
	int iIndex = 0;
	for (auto& pTile : m_listDecos) {
		pTile->Render(m_pCamera);
		swprintf_s(szBuf, L"%d", iIndex++);
		CGraphicDevice::GetInstance()->GetFont()->DrawTextW(CGraphicDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 255));
	}
}

void CMapEditor::RenderSelectedObj()
{

}

void CMapEditor::LinkView(void)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pToolView = dynamic_cast<CToothAndTailMapToolView*>(pMain->m_MainSplitter.GetPane(0, 1));
}

void CMapEditor::LoadTextures(void)
{
	// 베이스 맵 텍스쳐 생성
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP")))
		return;

	// 타일
	// 1) 노말
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile0.png", L"TILE_NORMAL")))
		return;
	// 2) 블로킹
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile1.png", L"TILE_BLOCKING")))
		return;
	// 3) OK
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile5.png", L"TILE_GREEN")))
		return;
	// 4) NO
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile3.png", L"TILE_RED")))
		return;

	// 데코
	// 1) 바위
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Stone/Stone%d.png", L"Deco", L"Stone", 3)))
		return;
	// 2) 나무
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Tree/Tree%d.png", L"Deco", L"Tree", 11)))
		return;
	// 데코 리스트에 등록
	m_pForm->m_pTab2_Deco->m_DecoList.AddString(L"Delete");
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	wstring wstrKey = L"";
	wstrKey = L"Stone";
	for (int i = 0; i < 3; i++) m_pForm->m_pTab2_Deco->m_DecoList.AddString((wstrKey + to_wstring(i)).c_str());
	wstrKey = L"Tree";
	for (int i = 0; i < 11; i++) pForm->m_pTab2_Deco->m_DecoList.AddString((wstrKey + to_wstring(i)).c_str());
}
