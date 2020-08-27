#include "stdafx.h"
#include "CWindmill.h"
#include "CTextureMgr.h"
#include "CCommander.h"
#include "CTurbine.h"


CWindmill::CWindmill(CGameWorld & _rGameWorld, float _fX, float _fY, CWindmill::E_STATE _eState, CCommander* _pCommander)
	:
	CComDepObj(_rGameWorld, _pCommander, _fX, _fY, WINDMILL_WIDTH, WINDMILL_HEIGHT),
	m_eState(_eState)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_BASE"));
	SetScaleXY(BASE_SCALE, BASE_SCALE);

	m_pIdentificationTintSprite = new CSpriteObj(_rGameWorld, 0.f, 0.f, WINDMILL_WIDTH, WINDMILL_HEIGHT);
	m_pIdentificationTintSprite->SetParent(this);
	m_pIdentificationTintSprite->SetColor(_pCommander->GetIdentificationTint());
	m_pIdentificationTintSprite->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"WINDMILL_BASE_TINT"));

	m_pTurbine = new CTurbine(_rGameWorld, 0.f, 0.f, _pCommander);
}


CWindmill::~CWindmill()
{
	Release();
}

void CWindmill::Ready(void)
{
}

int CWindmill::Update(float _fDeltaTime)
{
	switch (m_eState) {
	case CWindmill::STATE_UNOCCUPIED:
		break;
	case CWindmill::STATE_BUILDING:
		break;
	case CWindmill::STATE_OCCUPIED:
		break;
	}

	return 0;
}

void CWindmill::LateUpdate(void)
{
}

void CWindmill::RegisterToRenderList(vector<CObj*>& _vecRenderList)
{
	_vecRenderList.emplace_back(this);
	if(m_pTurbine) _vecRenderList.emplace_back(m_pTurbine);
	_vecRenderList.emplace_back(m_pGround);
}

void CWindmill::Release(void)
{
	SafelyDeleteObj(m_pIdentificationTintSprite);
	SafelyDeleteObj(m_pTurbine);
	SafelyDeleteObj(m_pGround);
}
