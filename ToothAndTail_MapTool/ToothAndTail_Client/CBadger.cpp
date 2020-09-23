#include "stdafx.h"
#include "CBadger.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CBadgerState_Idle.h"
#include "CUI_UnitHP.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CTunnel.h"
#include "CTunnelGenerator.h"
#include "CParticle_Blood.h"

CBadger::CBadger(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, BADGER_WIDTH, BADGER_HEIGHT, 0.f, 1.f, BADGER_SPEED, _iID),
	m_pTunnel(_pTunnel),
	m_pTunnelGenerator(_pTunnel->GetTunnelGenerator())
{
	InitHP(BADGER_MAX_HP);
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(BADGER_DETECTION_RANGE);
	SetAttackRange(BADGER_ATTACK_RANGE);
	SetMaxSpeed(BADGER_SPEED);

	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"BADGER"));
	GenerateIdentificationTintObj(BADGER_WIDTH, BADGER_HEIGHT, L"BADGER_TINT");

	m_pStateMgr = new CStateMgr<CBadger>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CBadgerState_Idle(GetGameWorld(), *this));
}

CBadger::~CBadger()
{
	Release();
}

int CBadger::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);
	
	return 0;
}

void CBadger::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CBadger::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}

void CBadger::InvalidateObj(void)
{
	if (m_pTunnel) m_pTunnel->ReleaseUnit(this);
	if (m_pTunnelGenerator) m_pTunnelGenerator->ReleaseUnit(this);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();
}

void CBadger::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}

void CBadger::TakeDamage(float _fDamageAmount, CObj* _pAttacker)
{
	D3DXVECTOR3 vPos = GetXY();
	GetGameWorld().GetListObjs().emplace_back(new CParticle_Blood(GetGameWorld(), vPos.x, vPos.y + 70.f, static_cast<int>(_fDamageAmount)));
	CComDepObj::TakeDamage(_fDamageAmount);
}
