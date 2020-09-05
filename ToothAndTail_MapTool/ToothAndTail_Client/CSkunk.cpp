#include "stdafx.h"
#include "CSkunk.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CSkunkState_Idle.h"

CSkunk::CSkunk(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, SKUNK_WIDTH, SKUNK_HEIGHT, 0.f, 1.f, SKUNK_SPEED),
	m_iID(_iID),
	m_pTunnel(_pTunnel)
{
	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"SKUNK"));
	GenerateIdentificationTintObj(SKUNK_WIDTH, SKUNK_HEIGHT, L"SKUNK_TINT");

	m_pStateMgr = new CStateMgr<CSkunk>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CSkunkState_Idle(GetGameWorld(), *this));
}

CSkunk::~CSkunk()
{
}

int CSkunk::Update(float _fDeltaTime)
{
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CSkunk::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}
