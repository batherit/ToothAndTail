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
#include "CBurst.h"
#include "CTunnelGenerator.h"

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

CTunnel::CTunnel(CGameWorld& _rGameWorld, CTunnelGenerator* _pTunnelGenerator, const TileSiteInfo& _rTileSiteInfo, CTunnel::E_SIZE _eSize, UNIT::E_TYPE _eUnitType, CCommander* _pCommander, int _iID)
	:
	m_iID(_iID),
	CComDepObj(_rGameWorld, _pCommander, 0.f, 0.f),
	m_eUnitType(_eUnitType),
	m_eSize(_eSize),
	m_pTunnelGenerator(_pTunnelGenerator)
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

	if (IsObjInCamera(this, GetGameWorld().GetMainCamera()))
		CSoundMgr::GetInstance()->PlaySound(L"Build_UnitFactory.wav", CSoundMgr::PLAYER);
}

CTunnel::~CTunnel()
{
	Release();
}

int CTunnel::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_P)) {
		InvalidateObj();
	}
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
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseMaxSupplyNum(m_iMaxSupplyNum);
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
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseMaxSupplyNum(m_iMaxSupplyNum);
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
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseMaxSupplyNum(m_iMaxSupplyNum);
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
		
		if (m_bIsGenerating) {
			// TODO : 생산하는 과정
			if ((m_fElapsedTime += _fDeltaTime) >= m_fGenTime) {
				D3DXVECTOR3 vPos = GetXY();
				CComDepObj* pUnit = nullptr;
				switch (m_eUnitType) {
				case UNIT::TYPE_SQUIRREL:
					pUnit = new CSquirrel(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID);
					m_listUnits.emplace_back(pUnit);
					GetGameWorld().GetListObjs().emplace_back(pUnit);
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseUnitsNum();
					break;
				case UNIT::TYPE_LIZARD:
					pUnit = new CLizard(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID);
					m_listUnits.emplace_back(pUnit);
					GetGameWorld().GetListObjs().emplace_back(pUnit);
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseUnitsNum();
					break;
				case UNIT::TYPE_MOLE:
					pUnit = new CMole(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID);
					m_listUnits.emplace_back(pUnit);
					GetGameWorld().GetListObjs().emplace_back(pUnit);
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseUnitsNum();
					break;
				case UNIT::TYPE_SKUNK:
					pUnit = new CSkunk(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID);
					m_listUnits.emplace_back(pUnit);
					GetGameWorld().GetListObjs().emplace_back(pUnit);
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseUnitsNum();
					break;
				case UNIT::TYPE_BADGER:
					pUnit = new CBadger(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID);
					m_listUnits.emplace_back(pUnit);
					GetGameWorld().GetListObjs().emplace_back(pUnit);
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseUnitsNum();
					break;
				case UNIT::TYPE_FOX:
					pUnit = new CFox(GetGameWorld(), GetCommander(), this, vPos.x, vPos.y, m_iID);
					m_listUnits.emplace_back(pUnit);
					GetGameWorld().GetListObjs().emplace_back(pUnit);
					if (m_pTunnelGenerator) m_pTunnelGenerator->IncreaseUnitsNum();
					break;
				}
				m_bIsGenerating = false;
			}
			return 0;
		}
		else {
			// 1) 보급 수 이하인지? true
			// 2) 커멘더의 자본이 충분한지? true
			// => 생성을 시작한다.
			// 3) 시간이 다 됐으면, 유닛을 생성한다.
			if (m_listUnits.size() >= m_iMaxSupplyNum) return 1;
			if (GetCommander()->GetMoney() < m_iUnitCost) return 1;
			//++m_iSupplyNum;
			GetCommander()->DecreseMoney(m_iUnitCost);
			m_bIsGenerating = true;
			m_fElapsedTime = 0.f;
		}
	}

	

	return 0;
}

void CTunnel::LateUpdate()
{
	/*if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_P)) {
		InvalidateObj();
	}*/
}

void CTunnel::Release(void)
{
	SafelyDeleteObj(m_pBuildGauge);
	m_listUnits.clear();
}

void CTunnel::InvalidateObj(void)
{
	if(m_pTunnelGenerator && m_eState != CTunnel::STATE_BUILDING) m_pTunnelGenerator->ReleaseTunnel(this);
	for (auto& pUnit : m_listUnits) {
		pUnit->ReleaseTunnel(this);
	}
	m_listUnits.clear();
	//SafelyDeleteObjs(m_listUnits);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();

	// 파괴 사운드 처리
	if (!IsObjInCamera(this, GetGameWorld().GetMainCamera())) return;
		
	switch (m_eSize) {
	case CTunnel::SIZE_SMALL:
		CSoundMgr::GetInstance()->PlaySound(L"Destroy_Small.wav", CSoundMgr::PLAYER);
		break;
	case CTunnel::SIZE_MIDDLE:
		CSoundMgr::GetInstance()->PlaySound(L"Destroy_Mid.wav", CSoundMgr::PLAYER);
		break;
	case CTunnel::SIZE_BIG:
		CSoundMgr::GetInstance()->PlaySound(L"Destroy_Big.wav", CSoundMgr::PLAYER);
		break;
	}
}

void CTunnel::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	m_pBuildGauge->RegisterToRenderList(_vecRenderList);
}
