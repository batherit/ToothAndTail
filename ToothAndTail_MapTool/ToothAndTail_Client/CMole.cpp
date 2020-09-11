#include "stdafx.h"
#include "CMole.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CMoleState_Idle.h"
#include "CUI_UnitHP.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CTunnel.h"
#include "CTunnelGenerator.h"

CMole::CMole(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, MOLE_WIDTH, MOLE_HEIGHT, 0.f, 1.f, MOLE_SPEED, _iID),
	m_pTunnel(_pTunnel),
	m_pTunnelGenerator(_pTunnel->GetTunnelGenerator())
{
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(MOLE_DETECTION_RANGE);
	SetAttackRange(MOLE_ATTACK_RANGE);
	SetMaxSpeed(MOLE_SPEED);

	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"MOLE"));
	GenerateIdentificationTintObj(MOLE_WIDTH, MOLE_HEIGHT, L"MOLE_TINT");

	m_pStateMgr = new CStateMgr<CMole>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CMoleState_Idle(GetGameWorld(), *this));
}

CMole::~CMole()
{
	Release();
}

int CMole::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);

	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CMole::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CMole::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}

void CMole::InvalidateObj(void)
{
	if (m_pTunnel) m_pTunnel->ReleaseUnit(this);
	if (m_pTunnelGenerator) m_pTunnelGenerator->ReleaseUnit(this);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();
}

void CMole::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}
