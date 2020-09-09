#include "stdafx.h"
#include "CBadger.h"
#include "CTexture.h"
#include "CTextureMgr.h"
#include "CStateMgr.h"
#include "CBadgerState_Idle.h"
#include "CUI_UnitHP.h"

CBadger::CBadger(CGameWorld & _rGameWorld, CCommander * _pCommander, CTunnel* _pTunnel, float _fX, float _fY, int _iID)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, BADGER_WIDTH, BADGER_HEIGHT, 0.f, 1.f, BADGER_SPEED, _iID),
	m_pTunnel(_pTunnel)
{
	GetUIUnitHP()->SetY(-20.f);

	SetMinimapSign(MINIMAP::SIGN_UNIT);
	SetTargetPos(D3DXVECTOR3(_fX, _fY, 0.f));
	SetDetectionRange(BADGER_DETECTION_RANGE);

	SetRenderLayer(10);
	SetScale(BASE_SCALE);
	SetShadow(true);
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"BADGER"));
	GenerateIdentificationTintObj(BADGER_WIDTH, BADGER_HEIGHT, L"BADGER_TINT");

	m_pStateMgr = new CStateMgr<CBadger>(GetGameWorld(), *this);
	m_pStateMgr->SetNextState(new CBadgerState_Idle(GetGameWorld(), *this));
}

CBadger::~CBadger()
{
	Release();
}

int CBadger::Update(float _fDeltaTime)
{
	GetUIUnitHP()->Update(_fDeltaTime);
	if (!m_pStateMgr->ConfirmValidState()) return 1;
	m_pStateMgr->Update(_fDeltaTime);
	
	return 0;
}

void CBadger::LateUpdate()
{
	m_pStateMgr->LateUpdate();
}

void CBadger::Release()
{
	SafelyDeleteObj(m_pStateMgr);
}

void CBadger::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	CObj::RegisterToRenderList(_vecRenderList);
	GetUIUnitHP()->RegisterToRenderList(_vecRenderList);
}
