#include "stdafx.h"
#include "CSquirrel.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CSquirrelState_Idle.h"

CSquirrel::CSquirrel(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, SQUIRREL_WIDTH, SQUIRREL_HEIGHT, 0.f, 1.f, SQUIRREL_SPEED, _iID),
	m_pTunnel(_pTunnel)
{
	SetDetectionRange(SQUIRREL_DETECTION_RANGE);

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
