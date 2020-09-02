#include "stdafx.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CDeco.h"
#include "CTextureMgr.h"


CMapLoader::CMapLoader(CGameWorld & _rGameWorld, const wstring & wstrMapFilePath)
	:
	m_rGameWorld(_rGameWorld)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HANDLE hFile = CreateFile(wstrMapFilePath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (INVALID_HANDLE_VALUE == hFile)
		return;

	ClearObjs();
	m_pMap = new CSpriteObj(m_rGameWorld, (MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE, MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	m_pMap->SetScaleXY(BASE_SCALE, BASE_SCALE);
	
	LoadMapBorderLines(hFile);
	LoadTiles(hFile);
	LoadDecos(hFile);

	CloseHandle(hFile);
}

CMapLoader::~CMapLoader()
{
	Release();
}

void CMapLoader::Release(void)
{
	ClearObjs();
}

void CMapLoader::RenderMap(CCamera * _pCamera)
{
	m_pMap->Render(_pCamera);
}

void CMapLoader::RenderTile(CCamera * _pCamera)
{
	for (auto& pTile : m_vecTiles) {
		pTile->Render(_pCamera);
	}
}

void CMapLoader::ClearObjs()
{
	SafelyDeleteObj(m_pMap);
	SafelyDeleteObjs(m_vecTiles);
	m_vecBlockingTiles.clear();
	m_vecBlockingTiles.shrink_to_fit();
	SafelyDeleteObjs(m_vecDecos);
}

void CMapLoader::LoadMapBorderLines(HANDLE & _hfIn)
{
	ReadFile(_hfIn, m_vMapBorderLines, sizeof(m_vMapBorderLines[0]) * 4, nullptr, nullptr);
}

void CMapLoader::LoadTiles(HANDLE & _hfIn)
{
	int iTilesSize = 0;
	ReadFile(_hfIn, &iTilesSize, sizeof(iTilesSize), nullptr, nullptr);
	CTile* pTile = nullptr;
	for (int i = 0; i < iTilesSize; ++i) {
		pTile = new CTile(m_rGameWorld);
		pTile->LoadInfo(_hfIn);
		m_vecTiles.emplace_back(pTile);
	}
}

void CMapLoader::LoadDecos(HANDLE & _hfIn)
{
	int iDecosSize = 0;
	ReadFile(_hfIn, &iDecosSize, sizeof(iDecosSize), nullptr, nullptr);
	CDeco* pDeco = nullptr;
	for (int i = 0; i < iDecosSize; ++i) {
		pDeco = new CDeco(m_rGameWorld);
		pDeco->LoadInfo(_hfIn);
		m_vecDecos.emplace_back(pDeco);
	}
}
