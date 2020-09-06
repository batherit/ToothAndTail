#include "stdafx.h"
#include "CMole.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CMoleState_Idle.h"

CMole::CMole(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, MOLE_WIDTH, MOLE_HEIGHT, 0.f, 1.f, MOLE_SPEED, _iID),
	m_pTunnel(_pTunnel)
{
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
}

int CMole::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CMole::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}
