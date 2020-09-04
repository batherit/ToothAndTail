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
	m_pMap = new CSpriteObj(*this, (MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE, MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	m_pMap->SetScale(BASE_SCALE);

	m_pToolView->SetScrollSizes(MM_TEXT, CSize(
		static_cast<int>(m_pMap->GetWidth() * BASE_SCALE * 1.5f), 
		static_cast<int>(m_pMap->GetHeight() * BASE_SCALE * 1.5f)));

	// Grid Tiles 생성
	//m_iMapRow = (MAP_HEIGHT << 1) / TILE_HEIGHT + 1;
	//m_iMapCol = MAP_WIDTH / TILE_WIDTH + 1;

	// 타일 모서리 길이를 구한다.
	float fTileEdgeLength = sqrtf((TILE_WIDTH * TILE_WIDTH) + (TILE_HEIGHT * TILE_HEIGHT));
	// 맵 모서리 길이를 구한다.
	float fMapEdgeLength = sqrtf((MAP_WIDTH * MAP_WIDTH) + (MAP_HEIGHT * MAP_HEIGHT));
	m_iMapRow = static_cast<int>(fMapEdgeLength / fTileEdgeLength + 1);
	m_iMapCol = static_cast<int>(fMapEdgeLength / fTileEdgeLength + 1);
	
	D3DXVECTOR3 vTilePoint;
	CTile* pTile = nullptr;
	for (int i = 0; i < m_iMapRow; ++i) {
		for (int j = 0; j < m_iMapCol; ++j) {
			//vPoint.x = (static_cast<float>((TILE_WIDTH * j) + ((i % 2)* (TILE_WIDTH >> 1)))) * BASE_SCALE ;
			//vPoint.y = (static_cast<float>((TILE_HEIGHT >> 1) * i)) * BASE_SCALE;
			//vPoint.z = 0.f;
			vTilePoint.x = m_vStartPoint.x + (i + j) * (TILE_WIDTH >> 1) * BASE_SCALE;
			vTilePoint.y = m_vStartPoint.y + (i - j) * (TILE_HEIGHT >> 1) * BASE_SCALE;
			vTilePoint.z = 0.f;


			if (IsPointInPolygon(vTilePoint, m_vMapBorderLines, 4)) {
				pTile = new CTile(*this, vTilePoint.x, vTilePoint.y);
				pTile->SetScale(BASE_SCALE);
				m_vecTiles.emplace_back(pTile);
			}

			// 맵 경계 밖에 있는 좌표에 대해서는 타일을 추가하지 않는다.
			/*else {
				pTile = new CTile(*this, vTilePoint.x, vTilePoint.y);
				pTile->SetScale(BASE_SCALE);
				m_vecTiles.emplace_back(pTile);
				
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
	SafelyDeleteObjs(m_vecTiles);
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
		//for (auto& pTile : m_vecTiles) {
		//	if (IsPointInTile(vCursorW, pTile->GetXY(), pTile->GetWidth(), pTile->GetHeight())) {
		//		// 선택된 타일의 타입을 변경한다.
		//		pTile->SetTileType(static_cast<CTile::E_TYPE>(m_pForm->GetTileType()));
		//		break;
		//	}
		//}
		int iIndex = GetLineIndex(vCursorW);
		if (iIndex >= 0) {
			m_vecTiles[iIndex]->SetTileType(static_cast<TILE::E_TYPE>(m_pForm->GetTileType()));
		}
	}
		break;
	case MAP_OBJ::TYPE_DECO: {
		auto& rDecoType = m_pForm->GetDecoType();
		if (rDecoType.second != -1) {
			CDeco* pDeco = new CDeco(*this, vCursorW.x, vCursorW.y, rDecoType.first, rDecoType.second);
			pDeco->SetY(pDeco->GetY() - (pDeco->GetHeight() * BASE_SCALE * 0.5f) + (TILE_HEIGHT >> 1));
			pDeco->SetScale(BASE_SCALE);
			m_listDecos.emplace_back(pDeco);
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

void CMapEditor::SaveInfo()
{
	// TODO : 저장을 만듭니다.
	// 3배수 기준
	//for(int i = 0; i )


	//m_MapBorderLines[4]
	//for(auto&)

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(FALSE, L"dat", L"MapData.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", m_pForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Data");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		SaveMapBorderLines(hFile);
		SaveTiles(hFile);
		SaveDecos(hFile);

		CloseHandle(hFile);
	}
}

void CMapEditor::LoadInfo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog Dlg(TRUE, L"dat", L"MapData.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"Data File(*.dat) | *.dat||", m_pForm);
	TCHAR szCurPath[MAX_PATH] = L"";
	TCHAR szDataPath[MAX_PATH] = L"";
	GetCurrentDirectory(MAX_PATH, szCurPath);
	PathRemoveFileSpec(szCurPath);
	PathCombine(szDataPath, szCurPath, L"Data");
	Dlg.m_ofn.lpstrInitialDir = szDataPath;
	if (IDOK == Dlg.DoModal())
	{
		CString strPath = Dlg.GetPathName();
		HANDLE hFile = CreateFile(strPath.GetString(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		ClearObjs();
		LoadMapBorderLines(hFile);
		LoadTiles(hFile);
		LoadDecos(hFile);

		CloseHandle(hFile);
	}
	//UpdateData(FALSE);
}

void CMapEditor::RenderMap(void)
{
	m_pMap->Render(m_pCamera);
}

void CMapEditor::RenderTiles(void)
{
	TCHAR szBuf[MAX_PATH] = L"";
	int iIndex = 0;
	for (auto& pTile : m_vecTiles) {
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

void CMapEditor::SaveMapBorderLines(HANDLE& _hfOut)
{
	WriteFile(_hfOut, m_vMapBorderLines, sizeof(m_vMapBorderLines[0]) * 4, nullptr, nullptr);
}

void CMapEditor::SaveTiles(HANDLE & _hfOut)
{
	int iTilesSize = m_vecTiles.size();
	WriteFile(_hfOut, &iTilesSize, sizeof(iTilesSize), nullptr, nullptr);
	for (auto& pTile : m_vecTiles) {
		pTile->SaveInfo(_hfOut);
	}
}

void CMapEditor::SaveDecos(HANDLE & _hfOut)
{
	int iDecosSize = m_listDecos.size();
	WriteFile(_hfOut, &iDecosSize, sizeof(iDecosSize), nullptr, nullptr);
	for (auto& pDeco : m_listDecos) {
		pDeco->SaveInfo(_hfOut);
	}
}

void CMapEditor::ClearObjs()
{
	SafelyDeleteObjs(m_vecTiles);
	SafelyDeleteObjs(m_listDecos);
}

void CMapEditor::LoadMapBorderLines(HANDLE & _hfIn)
{
	ReadFile(_hfIn, m_vMapBorderLines, sizeof(m_vMapBorderLines[0]) * 4, nullptr, nullptr);
}

void CMapEditor::LoadTiles(HANDLE & _hfIn)
{
	int iTilesSize = 0;
	ReadFile(_hfIn, &iTilesSize, sizeof(iTilesSize), nullptr, nullptr);
	CTile* pTile = nullptr;
	for (int i = 0; i < iTilesSize; ++i) {
		pTile = new CTile(*this);
		pTile->LoadInfo(_hfIn);
		m_vecTiles.emplace_back(pTile);
	}
}

void CMapEditor::LoadDecos(HANDLE & _hfIn)
{
	int iDecosSize = 0;
	ReadFile(_hfIn, &iDecosSize, sizeof(iDecosSize), nullptr, nullptr);
	CDeco* pDeco = nullptr;
	for (int i = 0; i < iDecosSize; ++i) {
		pDeco = new CDeco(*this);
		pDeco->LoadInfo(_hfIn);
		m_listDecos.emplace_back(pDeco);
	}
}

void CMapEditor::LinkView(void)
{
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	m_pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	m_pForm->m_pMapEditor = this;
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

POINT CMapEditor::GetRowColIndex(const D3DXVECTOR3 & _vPos) const
{
	POINT ptIndexes = {-1, -1};
	if (m_vecTiles.empty()) return ptIndexes;
	D3DXVECTOR3 vTileStartPos = m_vecTiles[0]->GetXY();

	float fSumIJ = (_vPos.x - vTileStartPos.x) / ((TILE_WIDTH >> 1) * BASE_SCALE);
	float fSubIJ = (_vPos.y - vTileStartPos.y) / ((TILE_HEIGHT >> 1) * BASE_SCALE);

	ptIndexes.x = static_cast<LONG>(round((fSumIJ - fSubIJ) * 0.5f));
	ptIndexes.y = static_cast<LONG>(round((fSumIJ + fSubIJ) * 0.5f));

	return ptIndexes;
}

int CMapEditor::GetLineIndex(const D3DXVECTOR3 & _vPos) const
{
	POINT ptIndexes = GetRowColIndex(_vPos);
	if (ptIndexes.x < 0 || ptIndexes.y < 0) return -1;
	return ptIndexes.y * 47 + ptIndexes.x;
}