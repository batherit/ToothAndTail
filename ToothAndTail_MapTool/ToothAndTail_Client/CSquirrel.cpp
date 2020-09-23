#include "stdafx.h"
#include "CSquirrel.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CSquirrelState_Idle.h"
#include "CUI_UnitHP.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CTunnel.h"
#include "CTunnelGenerator.h"
#include "CParticle_Blood.h"

CSquirrel::CSquirrel(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, SQUIRREL_WIDTH, SQUIRREL_HEIGHT, 0.f, 1.f, SQUIRREL_SPEED, _iID),
	m_pTunnel(_pTunnel),
	m_pTunnelGenerator(_pTunnel->GetTunnelGenerator())
{
	InitHP(SQUIRREL_MAX_HP);
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(SQUIRREL_DETECTION_RANGE);
	SetAttackRange(SQUIRREL_ATTACK_RANGE);
	SetMaxSpeed(SQUIRREL_SPEED);

	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"SQUIRREL"));
	GenerateIdentificationTintObj(SQUIRREL_WIDTH, SQUIRREL_HEIGHT, L"SQUIRREL_TINT");

	m_pStateMgr = new CStateMgr<CSquirrel>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CSquirrelState_Idle(GetGameWorld(), *this));
}

CSquirrel::~CSquirrel()
{
	Release();
}

int CSquirrel::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CSquirrel::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CSquirrel::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}

void CSquirrel::InvalidateObj(void)
{
	if (m_pTunnel) m_pTunnel->ReleaseUnit(this);
	if (m_pTunnelGenerator) m_pTunnelGenerator->ReleaseUnit(this);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();
}

void CSquirrel::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}

void CSquirrel::TakeDamage(float _fDamageAmount, CObj* _pAttacker)
{
	D3DXVECTOR3 vPos = GetXY();
	GetGameWorld().GetListObjs().emplace_back(new CParticle_Blood(GetGameWorld(), vPos.x, vPos.y + 30.f, static_cast<int>(_fDamageAmount)));
	CComDepObj::TakeDamage(_fDamageAmount);
}
