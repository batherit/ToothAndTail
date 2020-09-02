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

	// 맵 정보 생성
	m_pMap = new CSpriteObj(m_rGameWorld, (MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE, MAP_WIDTH, MAP_HEIGHT);
	m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	m_pMap->SetScaleXY(BASE_SCALE, BASE_SCALE);
	
	// 맵 오브젝트 정보 생성
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

const void CMapLoader::PushObjectInMap(CObj * pObj)
{
	// 기울기 구하기
	float fGradient[4] = { 0.f, };
	for (int i = 0; i < 4; ++i) {
		fGradient[i] = ((m_vMapBorderLines[(i + 1) % 4].y - m_vMapBorderLines[i % 4].y)) 
			/ (m_vMapBorderLines[(i + 1) % 4].x - m_vMapBorderLines[i % 4].x);
	}

	// 절편 구하기 (b = y - ax)
	float fIntercept[4] = { 0.f, };
	for (int i = 0; i < 4; ++i) {
		fIntercept[i] = m_vMapBorderLines[i].y - fGradient[i] * m_vMapBorderLines[i].x;
	}

	// 법선 벡터 구하기
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vNormal[4];
	for (int i = 0; i < 4; ++i) {
		vDir = m_vMapBorderLines[(i + 1) % 4] - m_vMapBorderLines[i % 4];
		D3DXVec3Normalize(&vDir, &vDir);
		vNormal[i] = D3DXVECTOR3(-vDir.y, vDir.x, 0.f); // 윈도우 좌표계에선 마름모 안쪽으로 향하는 법선 벡터
	}

	float fLength = 0.f;
	D3DXVECTOR3 vPos;
	const float fSign[4] = { 1.f, -1.f, -1.f, 1.f };
	for (int i = 0; i < 4; ++i) {
		vPos = pObj->GetXY();
		if (fSign[i] * (fGradient[i] * vPos.x + fIntercept[i] - vPos.y) > 0) {
			// (y =) ax + b => ax -y + b = 0;
			// 점과 직선 사이 거리 공식으로 거리를 구한다.
			fLength = fabs(fGradient[i] * vPos.x - vPos.y + fIntercept[i])
				/ sqrtf(fGradient[i] * fGradient[i] + 1/*-1 * -1*/);
			// 법선 벡터(마름모 안쪽)쪽으로 밀어낸다.
			pObj->SetXY(vPos.x + vNormal[i].x * fLength, vPos.y + vNormal[i].y * fLength);
		}
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
		if (pTile->GetTileType() == CTile::TYPE_BLOCKING) {
			m_vecBlockingTiles.emplace_back(pTile);
		}
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
