#include "stdafx.h"
#include "CTunnel.h"
#include "CUnitGenerator.h"
#include "CTextureMgr.h"
#include "CGameWorld.h"
#include "CMapLoader.h"
#include "CCommander.h"
#include "CSquirrel.h"
#include "CLizard.h"
#include "CMole.h"
#include "CSkunk.h"
#include "CBadger.h"
#include "CFox.h"
#include "CUI_BuildGauge.h"

//
//CTunnel::CTunnel(CGameWorld& _rGameWorld, float _fX, float _fY, CTunnel::E_SIZE _eSize, UNIT::E_TYPE _eUnitType, CCommander* _pCommander, int _iID)
//	:
//	m_iID(_iID),
//	CComDepObj(_rGameWorld, _pCommander, _fX, _fY),
//	m_eUnitType(_eUnitType),
//	m_eSize(_eSize)
//	//m_pUnitGenerator(_pUnitGenerator)	// 외부에서 할당받아서 내부에서 해제한다.
//{
//	SetMinimapSign(MINIMAP::SIGN_TUNNEL);
//	// 자식들은 각자의 빌딩 애니메이션 정보를 세팅한다.
//	SetRenderLayer(6);
//	SetScaleXY(BASE_SCALE, BASE_SCALE);
//
//	switch (_eSize) {
//	case CTunnel::SIZE_SMALL: {
//		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_SMALL"));
//		SetSize(TUNNEL_SMALL_WIDTH, TUNNEL_SMALL_HEIGHT);
//		GenerateIdentificationTintObj(TUNNEL_SMALL_WIDTH, TUNNEL_SMALL_HEIGHT, L"TUNNEL_SMALL_TINT");
//		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
//	}
//		break;
//	case CTunnel::SIZE_MIDDLE: {
//		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_MIDDLE"));
//		SetSize(TUNNEL_MIDDLE_WIDTH, TUNNEL_MIDDLE_HEIGHT);
//		GenerateIdentificationTintObj(TUNNEL_MIDDLE_WIDTH, TUNNEL_MIDDLE_HEIGHT, L"TUNNEL_MIDDLE_TINT");
//		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
//	}
//		break;
//	case CTunnel::SIZE_BIG: {
//		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_BIG"));
//		SetSize(TUNNEL_BIG_WIDTH, TUNNEL_BIG_HEIGHT);
//		GenerateIdentificationTintObj(TUNNEL_BIG_WIDTH, TUNNEL_BIG_HEIGHT, L"TUNNEL_BIG_TINT");
//		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
//	}
//		break;
//	}
//
//	switch (m_eUnitType) {
//	case UNIT::TYPE_SQUIRREL:
//		m_fGenTime = SQUIRREL_GEN_TIME;
//		m_iMaxSupplyNum = SQUIRREL_SUPPLY_NUM;
//		m_iUnitCost = SQUIRREL_COST;
//		break;
//	case UNIT::TYPE_LIZARD:
//		m_fGenTime = LIZARD_GEN_TIME;
//		m_iMaxSupplyNum = LIZARD_SUPPLY_NUM;
//		m_iUnitCost = LIZARD_COST;
//		break;
//	case UNIT::TYPE_MOLE:
//		m_fGenTime = MOLE_GEN_TIME;
//		m_iMaxSupplyNum = MOLE_SUPPLY_NUM;
//		m_iUnitCost = MOLE_COST;
//		break;
//	case UNIT::TYPE_SKUNK:
//		m_fGenTime = SKUNK_GEN_TIME;
//		m_iMaxSupplyNum = SKUNK_SUPPLY_NUM;
//		m_iUnitCost = SKUNK_COST;
//		break;
//	case UNIT::TYPE_BADGER:
//		m_fGenTime = BADGER_GEN_TIME;
//		m_iMaxSupplyNum = BADGER_SUPPLY_NUM;
//		m_iUnitCost = BADGER_COST;
//		break;
//	case UNIT::TYPE_FOX:
//		m_fGenTime = FOX_GEN_TIME;
//		m_iMaxSupplyNum = FOX_SUPPLY_NUM;
//		m_iUnitCost = FOX_COST;
//		break;
//	}
//}

CTunnel::CTunnel(CGameWorld& _rGameWorld, const TileSiteInfo& _rTileSiteInfo, CTunnel::E_SIZE _eSize, UNIT::E_TYPE _eUnitType, CCommander* _pCommander, int _iID)
	:
	m_iID(_iID),
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f),
	m_eUnitType(_eUnitType),
	m_eSize(_eSize)
{

	SetMinimapSign(MINIMAP::SIGN_TUNNEL);
	// 자식들은 각자의 빌딩 애니메이션 정보를 세팅한다.
	SetRenderLayer(6);
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	SetTileSiteInfo(_rTileSiteInfo);
	_rGameWorld.GetMapLoader()->SetSiteType(_rTileSiteInfo, TILE::TYPE_NO);
	SetXY(_rGameWorld.GetMapLoader()->GetSiteCenter(_rTileSiteInfo));

	switch (_eSize) {
	case CTunnel::SIZE_SMALL: {
		SetMaxHP(TUNNEL_SMALL_MAX_HP);
		SetHP(1.f);
		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_SMALL"));
		SetSize(TUNNEL_SMALL_WIDTH, TUNNEL_SMALL_HEIGHT);
		SetRenderOffsetY(-5.f * BASE_SCALE);
		GenerateIdentificationTintObj(TUNNEL_SMALL_WIDTH, TUNNEL_SMALL_HEIGHT, L"TUNNEL_SMALL_TINT");
		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
		break;
	}							  
	case CTunnel::SIZE_MIDDLE: {
		SetMaxHP(TUNNEL_MIDDLE_MAX_HP);
		SetHP(1.f);
		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_MIDDLE"));
		SetSize(TUNNEL_MIDDLE_WIDTH, TUNNEL_MIDDLE_HEIGHT);
		SetRenderOffsetXY(1.f * BASE_SCALE, -9.f * BASE_SCALE);
		GenerateIdentificationTintObj(TUNNEL_MIDDLE_WIDTH, TUNNEL_MIDDLE_HEIGHT, L"TUNNEL_MIDDLE_TINT");
		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
		break;
	}						   
	case CTunnel::SIZE_BIG: {
		SetMaxHP(TUNNEL_BIG_MAX_HP);
		SetHP(1.f);
		PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"TUNNEL_BIG"));
		SetSize(TUNNEL_BIG_WIDTH, TUNNEL_BIG_HEIGHT);
		SetRenderOffsetXY(3.f * BASE_SCALE, -11.f * BASE_SCALE);
		GenerateIdentificationTintObj(TUNNEL_BIG_WIDTH, TUNNEL_BIG_HEIGHT, L"TUNNEL_BIG_TINT");
		SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 1, false));
		break;
	}						
	}

	switch (m_eUnitType) {
	case UNIT::TYPE_SQUIRREL:
		m_fGenTime = SQUIRREL_GEN_SEC;
		m_iMaxSupplyNum = SQUIRREL_SUPPLY_NUM;
		m_iUnitCost = SQUIRREL_COST;
		break;
	case UNIT::TYPE_LIZARD:
		m_fGenTime = LIZARD_GEN_SEC;
		m_iMaxSupplyNum = LIZARD_SUPPLY_NUM;
		m_iUnitCost = LIZARD_COST;
		break;
	case UNIT::TYPE_MOLE:
		m_fGenTime = MOLE_GEN_SEC;
		m_iMaxSupplyNum = MOLE_SUPPLY_NUM;
		m_iUnitCost = MOLE_COST;
		break;
	case UNIT::TYPE_SKUNK:
		m_fGenTime = SKUNK_GEN_SEC;
		m_iMaxSupplyNum = SKUNK_SUPPLY_NUM;
		m_iUnitCost = SKUNK_COST;
		break;
	case UNIT::TYPE_BADGER:
		m_fGenTime = BADGER_GEN_SEC;
		m_iMaxSupplyNum = BADGER_SUPPLY_NUM;
		m_iUnitCost = BADGER_COST;
		break;
	case UNIT::TYPE_FOX:
		m_fGenTime = FOX_GEN_SEC;
		m_iMaxSupplyNum = FOX_SUPPLY_NUM;
		m_iUnitCost = FOX_COST;
		break;
	}

	m_pBuildGauge = new CUI_BuildGauge(_rGameWorld, this, m_eUnitType);
	m_pBuildGauge->SetY(-15.f);
}

CTunnel::~CTunnel()
{
	Release();
}

int CTunnel::Update(float _fDeltaTime)
{
	switch (m_eSize)
	{
	case CTunnel::SIZE_SMALL: {
		switch (m_eState)
		{
		case CTunnel::STATE_BUILDING: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetHP(GetHP() + _fDeltaTime * TUNNEL_SMALL_MAX_HP / TUNNEL_SMALL_BUILD_SEC);
				if (IsFullHP()) {
					SetNewAnimInfo(AnimInfo(0, 8, 4, 11, 1.f, 1, false));
					m_eState = CTunnel::STATE_COMPLETED;
					m_fElapsedTime = 0.f;
					m_pBuildGauge->UpdateGauge(1.f);
				}
				else {
					// TODO : UI 갱신 해주기 등...
					m_pBuildGauge->UpdateGauge(GetHPRatio());
				}
			}
			break;		
		}
		case CTunnel::STATE_COMPLETED: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetNewAnimInfo(AnimInfo(0, 8, 14, 1, 0.f, 0, false));
				m_eState = CTunnel::STATE_GENERATE_UNIT;
			}
			break;
		}
		}
		break;
	}
	case CTunnel::SIZE_MIDDLE: {
		switch (m_eState)
		{
		case CTunnel::STATE_BUILDING: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetHP(GetHP() + _fDeltaTime * TUNNEL_MIDDLE_MAX_HP / TUNNEL_MIDDLE_BUILD_SEC);
				if (IsFullHP()) {
					SetNewAnimInfo(AnimInfo(0, 8, 4, 18, 2.f, 1, false));
					m_eState = CTunnel::STATE_COMPLETED;
					m_fElapsedTime = 0.f;
					m_pBuildGauge->UpdateGauge(1.f);
				}
				else {
					// TODO : UI 갱신 해주기 등...
					m_pBuildGauge->UpdateGauge(GetHPRatio());
				}
			}
			break;
		}
		case CTunnel::STATE_COMPLETED: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetNewAnimInfo(AnimInfo(0, 8, 21, 1, 0.f, 0, false));
				m_eState = CTunnel::STATE_GENERATE_UNIT;
			}
			break;
		}
		}
		break;
	}
	case CTunnel::SIZE_BIG: {
		switch (m_eState)
		{
		case CTunnel::STATE_BUILDING: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetHP(GetHP() + _fDeltaTime * TUNNEL_BIG_MAX_HP / TUNNEL_BIG_BUILD_SEC);
				if (IsFullHP()) {
					SetNewAnimInfo(AnimInfo(0, 8, 4, 34, 3.f, 1, false));
					m_eState = CTunnel::STATE_COMPLETED;
					m_fElapsedTime = 0.f;
					m_pBuildGauge->UpdateGauge(1.f);
				}
				else {
					// TODO : UI 갱신 해주기 등...
					m_pBuildGauge->UpdateGauge(GetHPRatio());
				}
			}
			break;
		}
		case CTunnel::STATE_COMPLETED: {
			if (1 == UpdateAnim(_fDeltaTime)) {
				SetNewAnimInfo(AnimInfo(0, 8, 37, 1, 0.f, 0, false));
				m_eState = CTunnel::STATE_GENERATE_UNIT;
			}
			break;
		}
		}
		break;
	}
	}


	if (CTunnel::STATE_GENERATE_UNIT == m_eState) {
		// 1) 보급 수 이하인지? true
		// 2) 커멘더의 자본이 충분한지? true
		// => 생성을 시작한다.
		// 3) 시간이 다 됐으면, 유닛을 생성한다.
		if (m_bIsGenerating) {
			// TODO : 생산하는 과정
			if ((m_fElapsedTime += _fDeltaTime) >= m_fGenTime) {
				D3DXVECTOR3 vPos = GetXY();
				switch (m_eUnitType) {
				case UNIT::TYPE_SQUIRREL:
					GetGameWorld().GetListObjs().emplace_back(new CSquirrel(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID));
					break;
				case UNIT::TYPE_LIZARD:
					GetGameWorld().GetListObjs().emplace_back(new CLizard(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID));
					break;
				case UNIT::TYPE_MOLE:
					GetGameWorld().GetListObjs().emplace_back(new CMole(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID));
					break;
				case UNIT::TYPE_SKUNK:
					GetGameWorld().GetListObjs().emplace_back(new CSkunk(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID));
					break;
				case UNIT::TYPE_BADGER:
					GetGameWorld().GetListObjs().emplace_back(new CBadger(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID));
					break;
				case UNIT::TYPE_FOX:
					GetGameWorld().GetListObjs().emplace_back(new CFox(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID));
					break;
				}
				m_bIsGenerating = false;
			}
			return 0;
		}
		if (m_iSupplyNum >= m_iMaxSupplyNum) return 1;
		if (GetCommander()->GetMoney() < m_iUnitCost) return 1;
		++m_iSupplyNum;
		GetCommander()->DecreseMoney(m_iUnitCost);
		m_bIsGenerating = true;
		m_fElapsedTime = 0.f;
	}

	return 0;
}

void CTunnel::Release(void)
{
	SafelyDeleteObj(m_pBuildGauge);
}

void CTunnel::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	m_pBuildGauge->RegisterToRenderList(_vecRenderList);
}
