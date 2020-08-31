#include "stdafx.h"
#include "CMapEditor.h"
#include "CCamera.h"
#include "CTextureMgr.h"
#include "CSpriteObj.h"
#include "CTile.h"
//#include "CDecoObj.h"


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
	LoadTextures();

	// 맵 스프라이트 객체 생성
	m_pMap = new CSpriteObj(*this, (MAP_WIDTH >> 1), (MAP_HEIGHT >> 1), MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));

	// Grid Tiles 생성
	m_iMapRow = (MAP_HEIGHT << 1) / TILE_HEIGHT + 1;
	m_iMapCol = MAP_WIDTH / TILE_WIDTH + 1;
	for (int i = 0; i < m_iMapRow; ++i) {
		for (int j = 0; j < m_iMapCol; ++j) {
			m_vecGrid.emplace_back(new CTile(*this
				, static_cast<float>((TILE_WIDTH * j) + ((i % 2)* (TILE_WIDTH >> 1)))
				, static_cast<float>((TILE_HEIGHT >> 1) * i)));
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
	RenderGrid();
	RenderTiles();

	EndRender();
}

void CMapEditor::Release(void)
{
	SafelyDeleteObj(m_pMap);
	SafelyDeleteObj(m_pCamera);
	SafelyDeleteObjs(m_vecGrid);
	SafelyDeleteObjs(m_vecTiles);
}

void CMapEditor::RenderMap(void)
{
	m_pMap->Render(m_pCamera);
}

void CMapEditor::RenderGrid(void)
{
	TCHAR szBuf[MAX_PATH] = L"";
	for (int i = 0; i < m_vecGrid.size(); ++i) {
		m_vecGrid[i]->Render(m_pCamera);
		// TODO : 인덱스를 출력합니다.
		swprintf_s(szBuf, L"%d", i);
		CGraphicDevice::GetInstance()->GetFont()->DrawTextW(CGraphicDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CMapEditor::RenderTiles(void)
{
	for (auto& pTile : m_vecTiles) {
		pTile->Render(m_pCamera);
	}
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

}
