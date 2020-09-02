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

	// �� ��������Ʈ ��ü ����
	m_pMap = new CSpriteObj(*this, (MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE, MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	m_pMap->SetScale(BASE_SCALE);

	m_pToolView->SetScrollSizes(MM_TEXT, CSize(m_pMap->GetWidth() * BASE_SCALE * 1.5f, m_pMap->GetHeight() * BASE_SCALE * 1.5f));

	// Grid Tiles ����
	m_iMapRow = (MAP_HEIGHT << 1) / TILE_HEIGHT + 1;
	m_iMapCol = MAP_WIDTH / TILE_WIDTH + 1;
	D3DXVECTOR3 vPoint;
	CTile* pTile = nullptr;
	for (int i = 0; i < m_iMapRow; ++i) {
		for (int j = 0; j < m_iMapCol; ++j) {
			vPoint.x = (static_cast<float>((TILE_WIDTH * j) + ((i % 2)* (TILE_WIDTH >> 1)))) * BASE_SCALE ;
			vPoint.y = (static_cast<float>((TILE_HEIGHT >> 1) * i)) * BASE_SCALE;
			vPoint.z = 0.f;

			if (IsPointInPolygon(vPoint, m_vMapBorderLines, 4)) {
				pTile = new CTile(*this, vPoint.x, vPoint.y);
				pTile->SetScale(BASE_SCALE);
				m_listTiles.emplace_back(pTile);
			}

			// �� ��� �ۿ� �ִ� ��ǥ�� ���ؼ��� Ÿ���� �߰����� �ʴ´�.
			/*else {
				m_vecGrid.emplace_back(new CTile(*this, vPoint.x, vPoint.y, CTile::TYPE_BLOCKING));
			}*/
		}
	}

	// ī�޶� ����
	m_pCamera = new CCamera(*this, nullptr, 0.f, 0.f);
	SetMainCamera(m_pCamera);		//.. ���� ī�޶� ����
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

	// TODO : �׸��� ������ �����մϴ�.
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
				// ���õ� Ÿ���� Ÿ���� �����Ѵ�.
				pTile->SetTileType(static_cast<CTile::E_TYPE>(m_pForm->GetTileType()));
				break;
			}
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
	// TODO : ������ ����ϴ�.
	// 3��� ����
	//for(int i = 0; i )


	//m_MapBorderLines[4]
	//for(auto&)

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

void CMapEditor::SaveMapBorderLines(HANDLE& _hfOut)
{
	//DWORD dwByte = 0;
	for (int i = 0; i < 4; i++) {
		WriteFile(_hfOut, &m_vMapBorderLines[i], sizeof(m_vMapBorderLines[i]), nullptr, nullptr);
	}
}

void CMapEditor::SaveTiles(HANDLE & _hfOut)
{
	int iTilesSize = m_listTiles.size();
	WriteFile(_hfOut, &iTilesSize, sizeof(iTilesSize), nullptr, nullptr);
	for (auto& pTile : m_listTiles) {
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
	SafelyDeleteObjs(m_listTiles);
	SafelyDeleteObjs(m_listDecos);
}

void CMapEditor::LoadMapBorderLines(HANDLE & _hfIn)
{
	//DWORD dwByte = 0;
	for (int i = 0; i < 4; ++i) {
		ReadFile(_hfIn, &m_vMapBorderLines[i], sizeof(m_vMapBorderLines[i]), nullptr, nullptr);
	}
}

void CMapEditor::LoadTiles(HANDLE & _hfIn)
{
	int iTilesSize = 0;
	ReadFile(_hfIn, &iTilesSize, sizeof(iTilesSize), nullptr, nullptr);
	CTile* pTile = nullptr;
	for (int i = 0; i < iTilesSize; ++i) {
		pTile = new CTile(*this);
		pTile->LoadInfo(_hfIn);
		m_listTiles.emplace_back(pTile);
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
	// ���̽� �� �ؽ��� ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Map/Map0.png", L"MAP")))
		return;

	// Ÿ��
	// 1) �븻
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile0.png", L"TILE_NORMAL")))
		return;
	// 2) ���ŷ
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile1.png", L"TILE_BLOCKING")))
		return;
	// 3) OK
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile5.png", L"TILE_GREEN")))
		return;
	// 4) NO
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_SINGLE, L"../Texture/Map/Tile/CollisionTile3.png", L"TILE_RED")))
		return;

	// ����
	// 1) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Stone/Stone%d.png", L"Deco", L"Stone", 3)))
		return;
	// 2) ����
	if (FAILED(CTextureMgr::GetInstance()->InsertTexture(CTextureMgr::TYPE_MULTI, L"../Texture/Map/Tree/Tree%d.png", L"Deco", L"Tree", 11)))
		return;
	// ���� ����Ʈ�� ���
	m_pForm->m_pTab2_Deco->m_DecoList.AddString(L"Delete");
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(AfxGetApp()->GetMainWnd());
	CForm* pForm = dynamic_cast<CForm*>(pMain->m_MainSplitter.GetPane(0, 0));
	wstring wstrKey = L"";
	wstrKey = L"Stone";
	for (int i = 0; i < 3; i++) m_pForm->m_pTab2_Deco->m_DecoList.AddString((wstrKey + to_wstring(i)).c_str());
	wstrKey = L"Tree";
	for (int i = 0; i < 11; i++) pForm->m_pTab2_Deco->m_DecoList.AddString((wstrKey + to_wstring(i)).c_str());
}
