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
	// �߸��� ����/�� ������ ���� ���, false�� ��ȯ why. �� ���� ��
	if (m_iStartLineIndex < 0 || m_iGoalLineIndex < 0) return false;
	// ���۰� �� ������ ���� ���, false�� ��ȯ why. �̹� ������ ����
	if (D3DXVec3Length(&(_vStartPos - _vGoalPos)) < 0.00001f) return false;
	if (m_iStartLineIndex == m_iGoalLineIndex) return false;
	// �� ������ ���ŷ Ÿ���� ���, false�� ��ȯ why. �����ִ� ����
	CTile* pGoalTile = pMapLoader->GetTile(m_iGoalLineIndex);
	if (pGoalTile && pGoalTile->GetTileType() == TILE::TYPE_BLOCKING) return false;
	
	// �� ������ �õ��Ѵ�. ���� ������ false�� ��ȯ�Ѵ�.
	if (FindPath(m_iStartLineIndex, m_iGoalLineIndex)) {
		MakePath(m_iStartLineIndex, m_iGoalLineIndex);
		// ��ǥ������ �������ش�.
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
	// �߸��� ����/�� ������ ���� ���, false�� ��ȯ why. �� ���� ��
	if (!pMapLoader->IsTileInRange(m_iStartLineIndex) || !pMapLoader->IsTileInRange(m_iGoalLineIndex)) return false;
	// ���۰� �� ������ ���� ���, false�� ��ȯ why. �̹� ������ ����
	if (m_iStartLineIndex == m_iGoalLineIndex) return false;
	// �� ������ ���ŷ Ÿ���� ���, false�� ��ȯ why. �����ִ� ����
	CTile* pGoalTile = pMapLoader->GetTile(m_iGoalLineIndex);
	if (pGoalTile && pGoalTile->GetTileType() == TILE::TYPE_BLOCKING) return false;

	// �� ������ �õ��Ѵ�. ���� ������ false�� ��ȯ�Ѵ�.
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

// FindPath �Լ��� ����Լ��μ� �۵��Ѵ�.
bool CPathGenerator::FindPath(int _iStartLineIndex, int _iGoalLineIndex)
{
	/*�ݺ�
	//	1) ���� ��Ͽ��� ���� ���� F ����� ��带 �����Ѵ�.
	//	2) ������ ��带 ���� ��Ͽ� ����ִ´�.
	//	3) ������ �簢���� ������ 8 ���� �簢���� ���� Ž�� / ó�� ������ ��ģ��.*/

	// 1) ���� ��Ͽ��� ������. (���� ���� ���� ���� F����� ���� ���(_iStartIndex)�̴�.)
	if (!m_listOpens.empty())
		m_listOpens.pop_front();
	// 2) ���� ��Ͽ� ��带 ����ִ´�.
	m_listCloses.emplace_back(_iStartLineIndex);

	// 3) ������ 8���� ������ ���� Ž��/ó�� ������ �����Ѵ�.
	CMapLoader* pMapLoader = m_rGameWorld.GetMapLoader();
	POINT RowColIndexes = pMapLoader->GetRowColIndex(_iStartLineIndex);
	CTile* pTile = nullptr;
	for (int i = -1; i <= 1; ++i) {
		// �»�ܿ������� �����Ͽ� ���ϴ����� Ÿ���� ���캻��.
		for (int j = -1; j <= 1; ++j) {
			if (0 == i && 0 == j) 
				continue;	// �ڱ� �ڽ��� ��Ÿ���� ���� Ÿ���� �����Ѵ�.
			
			pTile = pMapLoader->GetTile(RowColIndexes.y + i, RowColIndexes.x + j);
			if (!pTile) 
				continue;	// ��ȿ���� ���� Ÿ���̸� ���� Ÿ���� �����Ѵ�.
			
			if (pTile->GetTileType() == TILE::TYPE_BLOCKING)
				continue;	// �� �� ���� Ÿ���̸� ���� Ÿ���� �����Ѵ�.

			if (pTile->GetLineIndex() == _iGoalLineIndex) {
				pTile->SetParentTileIndex(_iStartLineIndex);
				return true;	// ��ǥ ������ ����������, true�� ��ȯ�Ѵ�.
			}
			
			if (CheckOpenList(pTile->GetLineIndex()) || CheckCloseList(pTile->GetLineIndex()))
				continue;	// ���� ��� �Ǵ� ���� ��Ͽ� �ش� ��尡 ������ �ǳʶڴ�.

			pTile->SetParentTileIndex(_iStartLineIndex);
			m_listOpens.emplace_back(pTile->GetLineIndex());
		}
	}

	if (m_listOpens.empty())
		return false;	// �湮�� ���� ���ٸ� false�� ��ȯ�Ѵ�.

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

		// ���ʿ� ��带 �߰��Ѵ�.
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
