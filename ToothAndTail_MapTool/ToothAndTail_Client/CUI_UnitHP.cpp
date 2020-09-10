#include "stdafx.h"
#include "CUI_UnitHP.h"
#include "CComDepObj.h"
#include "CUI_Image.h"
#include "CTextureMgr.h"

CUI_UnitHP::CUI_UnitHP(CGameWorld & _rGameWorld, CComDepObj * _pOwner)
	:
	CObj(_rGameWorld, 0.f, 0.f),
	m_pOwner(_pOwner)
{
	SetRenderLayer(20);

	SetParent(_pOwner);
	SetScale(1.f / 6.f);
	// healthgauge_factory_bg = 1905 96 60 60
	// healthgauge_factory_fill = 1905, 157 60 60
	m_pUnitHPBack = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
	m_pUnitHPBack->SetParent(this);
	m_pUnitHPBack->SetExtractionArea(1905, 96, 1905 + 60, 96 + 60);
	m_pUnitHPBack->SetOutputArea(POINT({ 0, 0}), 60, 60);

	m_pUnitHPFill = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
	m_pUnitHPFill->SetParent(this);
	m_pUnitHPFill->SetRenderColor(D3DCOLOR_ARGB(255, 255, 0, 0));
	m_pUnitHPFill->SetExtractionArea(1905, 167, 1905 + 60, 157 + 50);
	m_pUnitHPFill->SetOutputArea(POINT({ 0, 0 }), 60, m_ciHPLength);
}

CUI_UnitHP::~CUI_UnitHP()
{
	Release();
}

void CUI_UnitHP::Ready(void)
{
}

int CUI_UnitHP::Update(float _fDeltaTime)
{
	if ((m_fElapsedTime += _fDeltaTime) >= m_cfKeepingTime) {
		m_fElapsedTime = m_cfKeepingTime + 1;
	}

	return 0;
}

void CUI_UnitHP::LateUpdate(void)
{
}

void CUI_UnitHP::Release(void)
{
	SafelyDeleteObj(m_pUnitHPBack);
	SafelyDeleteObj(m_pUnitHPFill);
}

void CUI_UnitHP::Render(CCamera * _pCamera)
{
	if (m_fElapsedTime >= m_cfKeepingTime) return;

	m_pUnitHPBack->Render(_pCamera);
	m_pUnitHPFill->Render(_pCamera);
}

void CUI_UnitHP::DisplayHP(void)
{
	m_fElapsedTime = 0.f;
	RECT rcOutputArea = m_pUnitHPFill->GetOutputArea();
	rcOutputArea.top = rcOutputArea.bottom - m_pOwner->GetHPRatio() * m_ciHPLength;
	m_pUnitHPFill->SetSyncOutputArea(rcOutputArea);
}

void CUI_UnitHP::CloseHP(void)
{
	m_fElapsedTime = m_cfKeepingTime;
}
