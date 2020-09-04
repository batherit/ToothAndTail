#include "stdafx.h"
#include "CTunnelGenerator.h"
#include "CCommander.h"
#include "CGameWorld.h"
#include "CTunnel.h"
#include "CMapLoader.h"
#include "CTile.h"


CTunnelGenerator::CTunnelGenerator(CGameWorld & _rGameWorld, UNIT::E_TYPE _eUnitType, CCommander * _pCommander)
	:
	m_rGameWorld(_rGameWorld),
	m_eUnitType(_eUnitType),
	m_pCommander(_pCommander)
{
}

CTunnelGenerator::~CTunnelGenerator()
{
}

bool CTunnelGenerator::GenerateTunnel()
{
	if (!m_pCommander) return false;
	
	CTile* pTile = m_rGameWorld.GetMapLoader()->GetTile(m_pCommander->GetXY());
	if (!pTile) return false;

	D3DXVECTOR3 vToPlayer = m_pCommander->GetXY() - pTile->GetXY();
	int iPivotRow = 0;
	int iPivotCol = 0;
	switch (GetDirByVector(m_pCommander->GetToXY())) {
	case OBJ::DIR_RIGHT:
		iPivotRow = 0;
		iPivotCol = 0;
		break;
	case OBJ::DIR_LEFT:
		iPivotRow = 1;
		iPivotCol = 1;
		break;
	case OBJ::DIR_DOWN:
		iPivotRow = 0;
		iPivotCol = 1;
		break;
	case OBJ::DIR_UP:
		iPivotRow = 1;
		iPivotCol = 0;
		break;
	}

	//  TYPE_SQUIRREL, TYPE_LIZARD, TYPE_MOLE, TYPE_SKUNK, TYPE_BADGER, TYPE_FOX,
	switch (m_eUnitType)
	{
		// 1) ������ ��ġ�� �� ���� ������ ���� ���ϰ� �ִ���?
		// 2) ������ ���� �� �ִ� ��ġ����?
		// 3) ������ ��ġ�Ѵ�.
	case UNIT::TYPE_SQUIRREL: {// ���� ���� 
		if (TUNNEL_SMALL_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_SMALL_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, tTileSiteInfo, CTunnel::SIZE_SMALL, UNIT::TYPE_SQUIRREL, m_pCommander));
		
		break;
	}
	case UNIT::TYPE_LIZARD: {// ���� ���� 
		if (TUNNEL_SMALL_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_SMALL_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, tTileSiteInfo, CTunnel::SIZE_SMALL, UNIT::TYPE_LIZARD, m_pCommander));

		break;
	}
	case UNIT::TYPE_MOLE: {// ���� ���� 
		if (TUNNEL_SMALL_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_SMALL_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, tTileSiteInfo, CTunnel::SIZE_SMALL, UNIT::TYPE_MOLE, m_pCommander));

		break;
	}
	case UNIT::TYPE_SKUNK: {// ���� ���� 
		if (TUNNEL_MIDDLE_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_MIDDLE_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, tTileSiteInfo, CTunnel::SIZE_MIDDLE, UNIT::TYPE_SKUNK, m_pCommander));

		break;
	}
	case UNIT::TYPE_BADGER: {// ���� ���� 
		if (TUNNEL_BIG_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_BIG_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, tTileSiteInfo, CTunnel::SIZE_BIG, UNIT::TYPE_BADGER, m_pCommander));

		break;
	}
	case UNIT::TYPE_FOX: {// ���� ���� 
		if (TUNNEL_BIG_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_BIG_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, tTileSiteInfo, CTunnel::SIZE_BIG, UNIT::TYPE_FOX, m_pCommander));

		break;
	}
	default:
		break;
	}

	return false;
}
