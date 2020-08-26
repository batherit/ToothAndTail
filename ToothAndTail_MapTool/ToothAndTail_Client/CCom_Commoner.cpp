#include "stdafx.h"
#include "CCom_Commoner.h"
#include "CTextureMgr.h"
#include "CTexture.h"


CCom_Commoner::CCom_Commoner(CGameWorld & _rGameWorld, float _fX, float _fY, D3DCOLOR _clIdentificationTint_ARGB)
	:
	CSpriteObj(_rGameWorld, _fX, _fY, COM_COMMONER_WIDTH, COM_COMMONER_HEIGHT)
{
	PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"COM_COMMONER"));
	SetScaleXY(3.f, 3.f);

	m_pIdentificationTintSprite = new CSpriteObj(_rGameWorld, 0.f, 0.f, COM_COMMONER_WIDTH, COM_COMMONER_HEIGHT);
	m_pIdentificationTintSprite->SetParent(this);
	m_pIdentificationTintSprite->PushTexture(CTextureMgr::GetInstance()->GetTextureInfo(L"COM_COMMONER_TINT"));
	m_pIdentificationTintSprite->SetColor(_clIdentificationTint_ARGB);

	AnimInfo stAnimInfo(0, 8, 0, 10, 1.f, 0, false); // 무한 애님 테스트
	SetNewAnimInfo(stAnimInfo);
	m_pIdentificationTintSprite->SetNewAnimInfo(stAnimInfo);
}

CCom_Commoner::~CCom_Commoner()
{
	Release();
}

void CCom_Commoner::Ready(void)
{
}

int CCom_Commoner::Update(float _fDeltaTime)
{
	m_pIdentificationTintSprite->UpdateAnim(_fDeltaTime);
	return UpdateAnim(_fDeltaTime);
}

void CCom_Commoner::LateUpdate(void)
{
}

void CCom_Commoner::Release(void)
{
}

void CCom_Commoner::Render(CCamera * _pCamera)
{
	CSpriteObj::Render(_pCamera);
	m_pIdentificationTintSprite->Render(_pCamera);
}
