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

bool CTunnelGenerator::GenerateTunnel(int _iID)
{
	if (!m_pCommander) return false;
	
	CTile* pTile = m_rGameWorld.GetMapLoader()->GetTile(m_pCommander->GetXY());
	if (!pTile) return false;

	D3DXVECTOR3 vToPlayer = m_pCommander->GetXY() - pTile->GetXY();
	D3DXVec3Normalize(&vToPlayer, &vToPlayer);

	int iPivotRow = 0;
	int iPivotCol = 0;
	switch (GetDirByVector(vToPlayer)) {
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
		// 1) ¶¥±¼À» ¼³Ä¡ÇÒ ¼ö ÀÖÀ» Á¤µµÀÇ µ·À» Áö´Ï°í ÀÖ´ÂÁö?
		// 2) ¶¥±¼ÀÌ ³õÀÏ ¼ö ÀÖ´Â À§Ä¡ÀÎÁö?
		// 3) ¶¥±¼À» ¼³Ä¡ÇÑ´Ù.
	case UNIT::TYPE_SQUIRREL: {// ¼ÒÇü ¶¥±¼ 
		if (TUNNEL_SMALL_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_SMALL_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, this, tTileSiteInfo, CTunnel::SIZE_SMALL, UNIT::TYPE_SQUIRREL, m_pCommander, _iID));
		
		break;
	}
	case UNIT::TYPE_LIZARD: {// ¼ÒÇü ¶¥±¼ 
		if (TUNNEL_SMALL_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_SMALL_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, this, tTileSiteInfo, CTunnel::SIZE_SMALL, UNIT::TYPE_LIZARD, m_pCommander, _iID));

		break;
	}
	case UNIT::TYPE_MOLE: {// ¼ÒÇü ¶¥±¼ 
		if (TUNNEL_SMALL_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_SMALL_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, this, tTileSiteInfo, CTunnel::SIZE_SMALL, UNIT::TYPE_MOLE, m_pCommander, _iID));

		break;
	}
	case UNIT::TYPE_SKUNK: {// ÁßÇü ¶¥±¼ 
		if (TUNNEL_MIDDLE_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_MIDDLE_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, this, tTileSiteInfo, CTunnel::SIZE_MIDDLE, UNIT::TYPE_SKUNK, m_pCommander, _iID));

		break;
	}
	case UNIT::TYPE_BADGER: {// ´ëÇü ¶¥±¼ 
		if (TUNNEL_BIG_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_BIG_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, this, tTileSiteInfo, CTunnel::SIZE_BIG, UNIT::TYPE_BADGER, m_pCommander, _iID));

		break;
	}
	case UNIT::TYPE_FOX: {// ´ëÇü ¶¥±¼ 
		if (TUNNEL_BIG_BUILD_COST > m_pCommander->GetMoney()) return false;
		if (!m_rGameWorld.GetMapLoader()->IsEmptyLot(m_pCommander->GetXY(), 2, 2, iPivotRow, iPivotCol)) return false;
		m_pCommander->DecreseMoney(TUNNEL_BIG_BUILD_COST);
		TileSiteInfo tTileSiteInfo(pTile->GetLineIndex(), 2, 2, iPivotRow, iPivotCol);
		m_rGameWorld.GetListObjs().emplace_back(new CTunnel(m_rGameWorld, this, tTileSiteInfo, CTunnel::SIZE_BIG, UNIT::TYPE_FOX, m_pCommander, _iID));

		break;
	}
	default:
		break;
	}

	return false;
}

void CTunnelGenerator::ReleaseTunnel(CTunnel * _pTunnel)
{
	DecreaseMaxSupplyNum(_pTunnel->GetMaxSupplyNum());
}

void CTunnelGenerator::ReleaseUnit(CComDepObj * _pUnit)
{
	DecreaseUnitsNum();
}

void CTunnelGenerator::PlaySoundForGathering()
{	
	switch (m_eUnitType) {
	case UNIT::TYPE_SQUIRREL:
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Squirrel.wav", CSoundMgr::PLAYER);
		break;
	case UNIT::TYPE_LIZARD:
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Lizard.wav", CSoundMgr::PLAYER);
		break;
	case UNIT::TYPE_MOLE:
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Mole.wav", CSoundMgr::PLAYER);
		break;
	case UNIT::TYPE_SKUNK:
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Skunk.wav", CSoundMgr::PLAYER);
		break;
	case UNIT::TYPE_BADGER:
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Badger.wav", CSoundMgr::PLAYER);
		break;
	case UNIT::TYPE_FOX:
		CSoundMgr::GetInstance()->PlaySound(L"Unit_Fox.wav", CSoundMgr::PLAYER);
		break;
	}
}
