#include "stdafx.h"
#include "CMapLoader.h"
#include "CTile.h"
#include "CDeco.h"
#include "CTextureMgr.h"
#include "CUI_Image.h"


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
	//m_pMap = new CSpriteObj(m_rGameWorld, (MAP_WIDTH >> 1) * BASE_SCALE, (MAP_HEIGHT >> 1) * BASE_SCALE, MAP_WIDTH, MAP_HEIGHT);
	//m_pMap->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	//m_pMap->SetScaleXY(BASE_SCALE, BASE_SCALE);
	m_pMapImage = new CUI_Image(m_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"MAP"));
	m_pMapImage->SetOutputArea(0, 0, MAP_WIDTH * BASE_SCALE, MAP_HEIGHT * BASE_SCALE);
	
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
	m_pMapImage->Render(_pCamera);
}

void CMapLoader::RenderTile(CCamera * _pCamera)
{
	for (auto& pTile : m_vecTiles) {
		pTile->Render(_pCamera);
	}
}

bool CMapLoader::IsTileInRange(int iRow, int iCol) const
{
	if (0 > iRow || iRow >= m_ciMapRow) return false;
	if (0 > iCol || iCol >= m_ciMapCol) return false;
	return true;
}

bool CMapLoader::IsTileInRange(int iLineIndex) const
{
	if (0 > iLineIndex || iLineIndex >= m_vecTiles.size()) return false;
	return true;
}

bool CMapLoader::IsTileInRange(POINT ptRowCol) const
{
	return IsTileInRange(ptRowCol.y, ptRowCol.x);
}

bool CMapLoader::IsEmptyLot(const D3DXVECTOR3 & _vPos, int _iCoveredRow, int _iCoveredCol, int _iPivotRow, int _iPivotCol)
{
	POINT ptTileRowCol = GetRowColIndex(_vPos);
	if (ptTileRowCol.x < 0 || ptTileRowCol.y < 0) return false;

	CTile* pTile = nullptr;
	for (int i = 0 - _iPivotRow; i < _iCoveredRow - _iPivotRow; ++i) {
		for (int j = 0 - _iPivotCol; j < _iCoveredCol - _iPivotCol; ++j) {
			pTile = GetTile(ptTileRowCol.y + i, ptTileRowCol.x + j);
			if (!pTile) return false;
			if (pTile->GetTileType() == TILE::TYPE_BLOCKING ||
				pTile->GetTileType() == TILE::TYPE_NO) return false;
		}
	}

	return true;
}

bool CMapLoader::IsEmptyLot(const POINT & ptRowColIndexes, int _iCoveredRow, int _iCoveredCol, int _iPivotRow, int _iPivotCol)
{
	if (ptRowColIndexes.x < 0 || ptRowColIndexes.y < 0) return false;

	CTile* pTile = nullptr;
	for (int i = 0 - _iPivotRow; i < _iCoveredRow - _iPivotRow; ++i) {
		for (int j = 0 - _iPivotCol; j < _iCoveredCol - _iPivotCol; ++j) {
			pTile = GetTile(ptRowColIndexes.y + i, ptRowColIndexes.x + j);
			if (!pTile) return false;
			if (pTile->GetTileType() == TILE::TYPE_BLOCKING ||
				pTile->GetTileType() == TILE::TYPE_NO) return false;
		}
	}

	return true;
}

CTile * CMapLoader::GetTile(D3DXVECTOR3 _vPos) const
{
	int iLineIndex = GetLineIndex(_vPos);

	if (-1 == iLineIndex) return nullptr;
	return m_vecTiles[iLineIndex];
}

CTile * CMapLoader::GetTile(int _iRow, int _iCol) const
{
	if (!IsTileInRange(_iRow, _iCol)) return nullptr;
	return GetTile(_iRow * m_ciMapCol + _iCol);
}

CTile * CMapLoader::GetTile(int _iLineIndex) const
{
	if (!IsTileInRange(_iLineIndex)) return nullptr;
	return m_vecTiles[_iLineIndex];
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
	SafelyDeleteObj(m_pMapImage);
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
		pTile = new CTile(m_rGameWorld, i);
		pTile->LoadInfo(_hfIn);
		m_vecTiles.emplace_back(pTile);
		if (pTile->GetTileType() == TILE::TYPE_BLOCKING) {
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
		pDeco->SetRenderLayer(10);
		m_vecDecos.emplace_back(pDeco);
	}
}

POINT CMapLoader::GetRowColIndex(const D3DXVECTOR3 & _vPos) const
{
	POINT ptIndexes = { -1, -1 };
	if (m_vecTiles.empty()) return ptIndexes;
	D3DXVECTOR3 vTileStartPos = m_vecTiles[0]->GetXY();

	float fSumIJ = (_vPos.x - vTileStartPos.x) / ((TILE_WIDTH >> 1) * BASE_SCALE);	// i + j
	float fSubIJ = (_vPos.y - vTileStartPos.y) / ((TILE_HEIGHT >> 1) * BASE_SCALE);	// i - j

	ptIndexes.x = static_cast<LONG>(round((fSumIJ - fSubIJ) * 0.5f)); // j
	ptIndexes.y = static_cast<LONG>(round((fSumIJ + fSubIJ) * 0.5f)); // i

	return ptIndexes;
}

POINT CMapLoader::GetRowColIndex(int _iLineIndex) const
{
	POINT ptIndexes = {-1, -1};

	if (!IsTileInRange(_iLineIndex)) return ptIndexes;

	ptIndexes.x = _iLineIndex % m_ciMapCol;	// Col
	ptIndexes.y = _iLineIndex / m_ciMapCol;	// Row

	return ptIndexes;
}

int CMapLoader::GetLineIndex(const D3DXVECTOR3 & _vPos) const
{
	POINT ptIndexes = GetRowColIndex(_vPos);
	if (ptIndexes.x < 0 || ptIndexes.y < 0) return -1;
	return ptIndexes.y * m_ciMapCol + ptIndexes.x;
}

void CMapLoader::UpdateBlockingTiles()
{
	m_vecBlockingTiles.clear();
	for (int i = 0; i < m_vecTiles.size(); ++i) {
		if (m_vecTiles[i]->GetTileType() == TILE::TYPE_BLOCKING) {
			m_vecBlockingTiles.emplace_back(m_vecTiles[i]);
		}
	}
}

D3DXVECTOR3 CMapLoader::GetSiteCenter(const TileSiteInfo & _rTileSiteInfo)
{
	D3DXVECTOR3 vCenter(0.f, 0.f, 0.f);
	int iCnt = 0;
	CTile* pTile = nullptr;
	POINT ptRowColIndex = GetRowColIndex(_rTileSiteInfo.iTileLineIndex);
	
	for (int i = 0 - _rTileSiteInfo.iPivotRow; i < _rTileSiteInfo.iCoveredRow - _rTileSiteInfo.iPivotRow; ++i) {
		for (int j = 0 - _rTileSiteInfo.iPivotCol; j < _rTileSiteInfo.iCoveredCol - _rTileSiteInfo.iPivotCol; ++j) {
			pTile = GetTile(ptRowColIndex.y + i, ptRowColIndex.x + j);
			if (pTile) {
				vCenter += pTile->GetXY();
				++iCnt;
			}
		}
	}

	if (0 == iCnt) return vCenter;

	vCenter /= static_cast<FLOAT>(iCnt);
	return vCenter;
}

void CMapLoader::SetSiteType(const TileSiteInfo & _rTileSiteInfo, TILE::E_TYPE _eTileType)
{
	POINT ptTileIndexes = GetRowColIndex(_rTileSiteInfo.iTileLineIndex);
	CTile* pTile = nullptr;
	for (int i = 0 - _rTileSiteInfo.iPivotRow; i < _rTileSiteInfo.iCoveredRow - _rTileSiteInfo.iPivotRow; ++i) {
		for (int j = 0 - _rTileSiteInfo.iPivotCol; j < _rTileSiteInfo.iCoveredCol - _rTileSiteInfo.iPivotCol ; ++j) {
			pTile = GetTile(ptTileIndexes.y + i, ptTileIndexes.x + j);
			if (pTile)
				pTile->SetTileType(_eTileType);
		}
	}
}
