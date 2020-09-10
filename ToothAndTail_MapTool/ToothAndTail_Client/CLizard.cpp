#include "stdafx.h"
#include "CLizard.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CLizardState_Idle.h"
#include "CUI_UnitHP.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CTunnel.h"
#include "CTunnelGenerator.h"

CLizard::CLizard(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, LIZARD_WIDTH, LIZARD_HEIGHT, 0.f, 1.f, LIZARD_SPEED, _iID),
	m_pTunnel(_pTunnel),
	m_pTunnelGenerator(_pTunnel->GetTunnelGenerator())
{
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(LIZARD_DETECTION_RANGE);
	SetAttackRange(LIZARD_ATTACK_RANGE);

	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"LIZARD"));
	GenerateIdentificationTintObj(LIZARD_WIDTH, LIZARD_HEIGHT, L"LIZARD_TINT");

	m_pStateMgr = new CStateMgr<CLizard>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CLizardState_Idle(GetGameWorld(), *this));
}

CLizard::~CLizard()
{
	Release();
}

int CLizard::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CLizard::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CLizard::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}

void CLizard::InvalidateObj(void)
{
	if (m_pTunnel) m_pTunnel->ReleaseUnit(this);
	if (m_pTunnelGenerator) m_pTunnelGenerator->ReleaseUnit(this);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();
}

void CLizard::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}
