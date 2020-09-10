#include "stdafx.h"
#include "CFox.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CFoxState_Idle.h"
#include "CUI_UnitHP.h"
#include "CGameWorld.h"
#include "CBurst.h"
#include "CTunnel.h"
#include "CTunnelGenerator.h"

CFox::CFox(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, FOX_WIDTH, FOX_HEIGHT, 0.f, 1.f, FOX_SPEED, _iID),
	m_pTunnel(_pTunnel),
	m_pTunnelGenerator(_pTunnel->GetTunnelGenerator())
{
	GetUIUnitHP()->SetY(-20.f);

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
	GetUIUnitHP()->Update(_fDeltaTime);
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

void CFox::InvalidateObj(void)
{
	if (m_pTunnel) m_pTunnel->ReleaseUnit(this);
	if (m_pTunnelGenerator) m_pTunnelGenerator->ReleaseUnit(this);
	GetGameWorld().GetListObjs().emplace_back(new CBurst(GetGameWorld(), GetXY()));
	CComDepObj::InvalidateObj();
}

void CFox::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}
