#include "stdafx.h"
#include "CFox.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CFoxState_Idle.h"

CFox::CFox(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, FOX_WIDTH, FOX_HEIGHT, 0.f, 1.f, FOX_SPEED, _iID),
	m_pTunnel(_pTunnel)
{
	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(FOX_DETECTION_RANGE);

	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"FOX"));
	GenerateIdentificationTintObj(FOX_WIDTH, FOX_HEIGHT, L"FOX_TINT");

	m_pStateMgr = new CStateMgr<CFox>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CFoxState_Idle(GetGameWorld(), *this));
}

CFox::~CFox()
{
	Release();
}

int CFox::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CFox::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CFox::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}
