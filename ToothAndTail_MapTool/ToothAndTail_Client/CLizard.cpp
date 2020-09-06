#include "stdafx.h"
#include "CLizard.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CLizardState_Idle.h"

CLizard::CLizard(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, LIZARD_WIDTH, LIZARD_HEIGHT, 0.f, 1.f, LIZARD_SPEED, _iID),
	m_pTunnel(_pTunnel)
{
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
}

int CLizard::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CLizard::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}