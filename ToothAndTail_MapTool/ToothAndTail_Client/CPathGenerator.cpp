#include "stdafx.h"
#include "CPathGenerator.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CTile.h"

CPathGenerator::CPathGenerator(CGameWorld& _rGameWorld)
	:
	m_rGameWorld(_rGameWorld)
{
}

CPathGenerator::~CPathGenerator()
{
	Release();
}

bool CPathGenerator::GeneratePath(const D3DXVECTOR3 & _vStartPos, const D3DXVECTOR3 & _vGoalPos)
{
	m_listPath.clear();
	m_listOpens.clear();
	m_listCloses.clear();

	CMapLoader* pMapLoader = m_rGameWorld.GetMapLoader();
	m_iStartLineIndex = pMapLoader->GetLineIndex(_vStartPos);
	m_iGoalLineIndex = pMapLoader->GetLineIndex(_vGoalPos);
	// 잘못된 시작/끝 지점이 들어온 경우, false를 반환 why. 맵 범위 밖
	if (m_iStartLineIndex < 0 || m_iGoalLineIndex < 0) return false;
	// 시작과 끝 지점이 같은 경우, false를 반환 why. 이미 도착한 지점
	if (D3DXVec3Length(&(_vStartPos - _vGoalPos)) < 0.00001f) return false;
	if (m_iStartLineIndex == m_iGoalLineIndex) return false;
	// 끝 지점이 블로킹 타일일 경우, false를 반환 why. 막혀있는 지점
	CTile* pGoalTile = pMapLoader->GetTile(m_iGoalLineIndex);
	if (pGoalTile && pGoalTile->GetTileType() == TILE::TYPE_BLOCKING) return false;
	
	// 길 생성을 시도한다. 길이 막히면 false를 반환한다.
	if (FindPath(m_iStartLineIndex, m_iGoalLineIndex)) {
		MakePath(m_iStartLineIndex, m_iGoalLineIndex);
		// 목표지점은 보정해준다.
		m_listPath.emplace_back(_vGoalPos);
		return true;
	}
	return false;
}

bool CPathGenerator::GeneratePath(const int & _iStartLineIndex, const int & _iGoalLineIndex)
{
	m_listPath.clear();
	m_listOpens.clear();
	m_listCloses.clear();

	CMapLoader* pMapLoader = m_rGameWorld.GetMapLoader();
	m_iStartLineIndex = _iStartLineIndex;
	m_iGoalLineIndex = _iGoalLineIndex;
	// 잘못된 시작/끝 지점이 들어온 경우, false를 반환 why. 맵 범위 밖
	if (!pMapLoader->IsTileInRange(m_iStartLineIndex) || !pMapLoader->IsTileInRange(m_iGoalLineIndex)) return false;
	// 시작과 끝 지점이 같은 경우, false를 반환 why. 이미 도착한 지점
	if (m_iStartLineIndex == m_iGoalLineIndex) return false;
	// 끝 지점이 블로킹 타일일 경우, false를 반환 why. 막혀있는 지점
	CTile* pGoalTile = pMapLoader->GetTile(m_iGoalLineIndex);
	if (pGoalTile && pGoalTile->GetTileType() == TILE::TYPE_BLOCKING) return false;

	// 길 생성을 시도한다. 길이 막히면 false를 반환한다.
	if (FindPath(m_iStartLineIndex, m_iGoalLineIndex)) {
		MakePath(m_iStartLineIndex, m_iGoalLineIndex);
		return true;
	}
	return false;
}

void CPathGenerator::Release()
{
	m_listOpens.clear();
	m_listCloses.clear();
	m_listPath.clear();
}

// FindPath 함수는 재귀함수로서 작동한다.
bool CPathGenerator::FindPath(int _iStartLineIndex, int _iGoalLineIndex)
{
	/*반복
	//	1) 열린 목록에서 가장 낮은 F 비용의 노드를 선택한다.
	//	2) 선택한 노드를 닫힌 목록에 집어넣는다.
	//	3) 선택한 사각형에 인접한 8 개의 사각형에 대해 탐색 / 처리 과정을 거친다.*/

	// 1) 열린 목록에서 빼낸다. (빼낸 노드는 가장 작은 F비용을 가진 노드(_iStartIndex)이다.)
	if (!m_listOpens.empty())
		m_listOpens.pop_front();
	// 2) 닫힌 목록에 노드를 집어넣는다.
	m_listCloses.emplace_back(_iStartLineIndex);

	// 3) 인접한 8개의 마름모에 대해 탐색/처리 과정을 진행한다.
	CMapLoader* pMapLoader = m_rGameWorld.GetMapLoader();
	POINT RowColIndexes = pMapLoader->GetRowColIndex(_iStartLineIndex);
	CTile* pTile = nullptr;
	for (int i = -1; i <= 1; ++i) {
		// 좌상단에서부터 시작하여 우하단으로 타일을 살펴본다.
		for (int j = -1; j <= 1; ++j) {
			if (0 == i && 0 == j) 
				continue;	// 자기 자신을 나타내면 다음 타일을 진행한다.
			
			pTile = pMapLoader->GetTile(RowColIndexes.y + i, RowColIndexes.x + j);
			if (!pTile) 
				continue;	// 유효하지 않은 타일이면 다음 타일을 진행한다.
			
			if (pTile->GetTileType() == TILE::TYPE_BLOCKING)
				continue;	// 갈 수 없는 타일이면 다음 타일을 진행한다.

			if (pTile->GetLineIndex() == _iGoalLineIndex) {
				pTile->SetParentTileIndex(_iStartLineIndex);
				return true;	// 목표 지점에 도달했으면, true를 반환한다.
			}
			
			if (CheckOpenList(pTile->GetLineIndex()) || CheckCloseList(pTile->GetLineIndex()))
				continue;	// 열린 목록 또는 닫힌 목록에 해당 노드가 있으면 건너뛴다.

			pTile->SetParentTileIndex(_iStartLineIndex);
			m_listOpens.emplace_back(pTile->GetLineIndex());
		}
	}

	if (m_listOpens.empty())
		return false;	// 방문할 곳이 없다면 false를 반환한다.

	auto& rTiles = pMapLoader->GetTiles();
	D3DXVECTOR3 vCostG;
	D3DXVECTOR3 vCostH;
	float fCostF1 = 0,  fCostF2 = 0;
	m_listOpens.sort([&](int iPreIndex, int iNextIndex)
	{
		vCostG = rTiles[m_iStartLineIndex]->GetXY() - rTiles[iPreIndex]->GetXY();
		vCostH = rTiles[m_iGoalLineIndex]->GetXY() - rTiles[iPreIndex]->GetXY();
		fCostF1 = D3DXVec3Length(&vCostG) + D3DXVec3Length(&vCostH);

		vCostG = rTiles[m_iStartLineIndex]->GetXY() - rTiles[iNextIndex]->GetXY();
		vCostH = rTiles[m_iGoalLineIndex]->GetXY() - rTiles[iNextIndex]->GetXY();
		fCostF2 = D3DXVec3Length(&vCostG) + D3DXVec3Length(&vCostH);

		return fCostF1 < fCostF2;
	});

	return FindPath(m_listOpens.front(), _iGoalLineIndex);
}

void CPathGenerator::MakePath(int _iStartLineIndex, int _iGoalLineIndex)
{
	auto& rTiles = m_rGameWorld.GetMapLoader()->GetTiles();
	m_listPath.emplace_back(rTiles[_iGoalLineIndex]->GetXY());
	//D3DXVECTOR3 vPos = rTiles[_iGoalLineIndex]->GetXY();
	//vPos.x -= (TILE_WIDTH >> 1) * BASE_SCALE;
	//m_listPath.emplace_back(vPos);
	int iParentLineIndex = rTiles[_iGoalLineIndex]->GetParentTileIndex();

	while (true)
	{
		if (_iStartLineIndex == iParentLineIndex)
			break;

		// 앞쪽에 노드를 추가한다.
		m_listPath.emplace_front(rTiles[iParentLineIndex]->GetXY());
		iParentLineIndex = rTiles[iParentLineIndex]->GetParentTileIndex();
	}
}

bool CPathGenerator::CheckOpenList(int _iLineIndex)
{
	auto& iter_find = find(m_listOpens.begin(), m_listOpens.end(), _iLineIndex);
	return m_listOpens.end() != iter_find;
}

bool CPathGenerator::CheckCloseList(int _iLineIndex)
{
	auto& iter_find = find(m_listCloses.begin(), m_listCloses.end(), _iLineIndex);
	return m_listCloses.end() != iter_find;
}
