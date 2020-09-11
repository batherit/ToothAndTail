#include "stdafx.h"
#include "CSkunk.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CSkunkState_Idle.h"
#include "CUI_UnitHP.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CTunnel.h"
#include "CTunnelGenerator.h"

CSkunk::CSkunk(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, SKUNK_WIDTH, SKUNK_HEIGHT, 0.f, 1.f, SKUNK_SPEED, _iID),
	m_pTunnel(_pTunnel),
	m_pTunnelGenerator(_pTunnel->GetTunnelGenerator())
{
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(SKUNK_DETECTION_RANGE);
	SetAttackRange(SKUNK_ATTACK_RANGE);
	SetMaxSpeed(SKUNK_SPEED);

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
	Release();
}

int CSkunk::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);

	return 0;
}

void CSkunk::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CSkunk::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}

void CSkunk::InvalidateObj(void)
{
	if (m_pTunnel) m_pTunnel->ReleaseUnit(this);
	if (m_pTunnelGenerator) m_pTunnelGenerator->ReleaseUnit(this);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();
}

void CSkunk::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}
