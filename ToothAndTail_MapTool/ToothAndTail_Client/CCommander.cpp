#include "stdafx.h"
#include "CCommander.h"
#include "CTextureMgr.h"
#include "CTexture.h"
#include "CStateMgr.h"
#include "CComState_Idle.h"
#include "CTunnelGenerator.h"
#include "CGameWorld.h"
#include "CWindmill.h"



CCommander::CCommander(CGameWorld & _rGameWorld, float _fX, float _fY, CCommander::E_COM_TYPE _eCommanderType, D3DCOLOR _clIdentificationTint_ARGB)
	:
	CComDepObj(_rGameWorld, this, _fX, _fY, COMMANDER_WIDTH, COMMANDER_HEIGHT, 1.f, 0.f, COMMANDER_SPEED),
	m_eCommanderType(_eCommanderType)/*,
	m_clIdentificationTint_ARGB(_clIdentificationTint_ARGB)*/
{
	SetScaleXY(BASE_SCALE, BASE_SCALE);
	wstring wstrCommander = L"";
	switch (_eCommanderType)
	{
	case CCommander::COM_TYPE_COMMONER:
		wstrCommander = L"COM_COMMONER";
		break;
	case CCommander::COM_TYPE_CAPITALIST:
		wstrCommander = L"COM_CAPITALIST";
		break;
	case CCommander::COM_TYPE_MILITARY:
		wstrCommander = L"COM_MILITARY";
		break;
	case CCommander::COM_TYPE_CLERGY:
		wstrCommander = L"COM_CLERGY";
		break;
	default:
		break;
	}
	
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(wstrCommander));
	SetShadow(true);
	SetRenderLayer(10);
	SetRenderOffsetY(-((COMMANDER_HEIGHT >> 1) - 3.f) * BASE_SCALE);
	GenerateIdentificationTintObj(COMMANDER_WIDTH, COMMANDER_HEIGHT, wstrCommander + L"_TINT", _clIdentificationTint_ARGB);

	m_pStateMgr = new CStateMgr<CCommander>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CComState_Idle(GetGameWorld(), *this));

	m_vecTunnelGenerator.emplace_back(new CTunnelGenerator(GetGameWorld(), UNIT::TYPE_SQUIRREL, this));
	m_vecTunnelGenerator.emplace_back(new CTunnelGenerator(GetGameWorld(), UNIT::TYPE_LIZARD, this));
	m_vecTunnelGenerator.emplace_back(new CTunnelGenerator(GetGameWorld(), UNIT::TYPE_MOLE, this));
	m_vecTunnelGenerator.emplace_back(new CTunnelGenerator(GetGameWorld(), UNIT::TYPE_SKUNK, this));
	m_vecTunnelGenerator.emplace_back(new CTunnelGenerator(GetGameWorld(), UNIT::TYPE_BADGER, this));
	m_vecTunnelGenerator.emplace_back(new CTunnelGenerator(GetGameWorld(), UNIT::TYPE_FOX, this));
}

CCommander::~CCommander()
{
	Release();
}

void CCommander::Ready(void)
{
}

int CCommander::Update(float _fDeltaTime)
{
	if (m_bIsAI) return 1;

	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_Q)) DesignatePrevUnit();
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_E)) DesignateNextUnit();

	UpdateCommand(_fDeltaTime);

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE)) {
		// 점령되지 않은 제분소를 찾는다.
		CWindmill* pWindmill = nullptr;
		for (auto& pObj : GetGameWorld().GetListObjs()) {
			pWindmill = dynamic_cast<CWindmill*>(pObj);
			// 제분소 중앙은 2*2 타일을 점령하고 있기 때문에 2를 곱하고 0.5배만큼의 오프셋을 두었다.
			if (pWindmill && (pWindmill->GetState() == WINDMILL::STATE_UNOCCUPIED || pWindmill->GetState() == WINDMILL::STATE_DESTROYED)
				&& IsPointInTile(GetXY(), pWindmill->GetXY(), TILE_WIDTH * BASE_SCALE * 2.5f, TILE_HEIGHT * BASE_SCALE * 2.5f)) {
				break;
			}
			else pWindmill = nullptr;
		}

		if (pWindmill) {
			pWindmill->Occupied(GetCommander());
		}
	}

	return 0;
}

void CCommander::LateUpdate(void)
{
	if (m_bIsAI) return;

	

	m_pStateMgr->LateUpdate();
}

void CCommander::Release(void)
{
	//SafelyDeleteObj(m_pCommanderSprite);
	//SafelyDeleteObj(m_pIdentificationTintSprite);
	SafelyDeleteObj(m_pStateMgr);
	SafelyDeleteObjs(m_vecTunnelGenerator);
}

//void CCommander::RegisterToRenderList(vector<CObj*>& _vecRenderList)
//{
//	//if (m_pCommanderSprite) m_pCommanderSprite->RegisterToRenderList(_vecRenderList);
//	//if (m_pIdentificationTintSprite) m_pCommanderSprite->RegisterToRenderList(_vecRenderList);
//}
//
//void CCommander::SetNewAnimInfo(const AnimInfo & _stAnimInfo)
//{
//	return m_pCommanderSprite->SetNewAnimInfo(_stAnimInfo);
//}
//
//int CCommander::UpdateAnim(float _fDeltaTime)
//{
//	return m_pCommanderSprite->UpdateAnim(_fDeltaTime);
//}
//
//D3DCOLOR CCommander::GetIdentificationTint(void) const
//{
//	return m_pCommanderSprite->GetIdentificationTint();
//}

bool CCommander::IsMoveKeyPressed(float & _fToX, float & _fToY)
{
	_fToX = 0.f;
	_fToY = 0.f;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_W) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_W)) {
		_fToX += cfDeltaX[OBJ::DIR_UP];
		_fToY += cfDeltaY[OBJ::DIR_UP];
		SetScaleX(BASE_SCALE);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_S) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_S)) {
		_fToX += cfDeltaX[OBJ::DIR_DOWN];
		_fToY += cfDeltaY[OBJ::DIR_DOWN];
		SetScaleX(-BASE_SCALE);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_A) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_A)) {
		_fToX += cfDeltaX[OBJ::DIR_LEFT];
		_fToY += cfDeltaY[OBJ::DIR_LEFT];
		SetScaleX(-BASE_SCALE);
	}

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_D) ||
		CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_D)) {
		_fToX += cfDeltaX[OBJ::DIR_RIGHT];
		_fToY += cfDeltaY[OBJ::DIR_RIGHT];
		SetScaleX(BASE_SCALE);
	}

	if (_fToX == 0.f && _fToY == 0.f) return false;
	return true;
}

bool CCommander::IsBuildKeyPressed(void) const
{
	return CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE);
}

bool CCommander::IsFlagKeyPressed(CCommander::E_FLAG_TYPE & _eFlagType) const
{
	_eFlagType = CCommander::FLAG_TYPE_NONE;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON) || CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_LBUTTON))
		_eFlagType = CCommander::FLAG_TYPE_UNIT;

	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON) || CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_RBUTTON))
		_eFlagType = CCommander::FLAG_TYPE_MILITARY;

	return _eFlagType != CCommander::FLAG_TYPE_NONE;
}

bool CCommander::IsSpaceKeyPressed() const
{
	return CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_SPACE);
}

void CCommander::GenerateTunnel()
{
	if (m_vecTunnelGenerator.empty()) return;

	m_vecTunnelGenerator[m_iTunnelGeneratorIndex]->GenerateTunnel(m_iTunnelGeneratorIndex);
	// m_iTunnelGeneratorIndex를 ID로 삼는다.
}

void CCommander::UpdateCommand(float _fDeltaTime)
{
	CommandInfo tNewCommandInfo;
	tNewCommandInfo.vTargetPos = GetXY();

	// 유닛은 COMMANDER::COMMAND_GATHERING 명령을 받으면 매 목표지점을 갱신한다.
	// 그렇지 않으면, 목표 지점을 갱신하지 않는다.
	// 명령어 작성
	if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_RBUTTON) &&
		CKeyMgr::GetInstance()->IsKeyNone(KEY::KEY_LBUTTON)) {
		tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
		tNewCommandInfo.iUnitID = -1;
		//tNewCommandInfo.bIgnoreEnemy = false;
		m_fElapsedTime = _fDeltaTime;
	}
	else if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_RBUTTON)) {
		tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
		tNewCommandInfo.iUnitID = -1;
		if ((m_fElapsedTime += _fDeltaTime) >= 0.2f) {
			// 적을 탐색하고, 적을 발견했다면 이 적을 집중 공격하라
			DetectEnemyAround();
			if (GetTargetEnemy()) {
				tNewCommandInfo.eCommand = COMMANDER::COMMAND_SATURATION;
				tNewCommandInfo.pTarget = GetTargetEnemy();
			}
			m_fElapsedTime = 0.2f;
		}
	}
	else if (CKeyMgr::GetInstance()->IsKeyDown(KEY::KEY_LBUTTON)) {
		tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
		tNewCommandInfo.iUnitID = m_iTunnelGeneratorIndex;
		m_fElapsedTime = _fDeltaTime;
	}
	else if (CKeyMgr::GetInstance()->IsKeyPressing(KEY::KEY_LBUTTON)) {
		// Index는 LBUTTON을 누르고 있는 도중에 변할 수 있기 때문에 매번 갱신한다.
		tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
		tNewCommandInfo.iUnitID = m_iTunnelGeneratorIndex;
		if ((m_fElapsedTime += _fDeltaTime) >= 0.2f) {
			// 적을 탐색하고, 적을 발견했다면 이 적을 집중 공격하라
			DetectEnemyAround();
			if (GetTargetEnemy()) {
				tNewCommandInfo.eCommand = COMMANDER::COMMAND_SATURATION;
				tNewCommandInfo.pTarget = GetTargetEnemy();
			}
			m_fElapsedTime = 0.2f;
		}
	}
	m_tCommandInfo = tNewCommandInfo;
}
