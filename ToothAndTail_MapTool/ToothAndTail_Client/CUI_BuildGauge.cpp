#include "stdafx.h"
#include "CUI_BuildGauge.h"
#include "CUI_UnitSign.h"
#include "CUI_Image.h"
#include "CTextureMgr.h"
#include "CComDepObj.h"

CUI_BuildGauge::CUI_BuildGauge(CGameWorld & _rGameWorld, CComDepObj* _pOwner, UNIT::E_TYPE _eUnitType, const D3DXVECTOR3 & vPos)
	:
	CObj(_rGameWorld, vPos.x, vPos.y)
{
	SetRenderLayer(20);

	SetParent(_pOwner);
	SetScale(1.f / 4.f);

	m_pUnitSign = new CUI_UnitSign(_rGameWorld, _pOwner->GetCommander(), _eUnitType);
	m_pUnitSign->SetParent(this);

	m_pBuildGaugeFill = new CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"UI_SET"));
	m_pBuildGaugeFill->SetParent(this);
	m_pBuildGaugeFill->SetRenderColor(D3DCOLOR_ARGB(50, 0, 0, 255));
	m_pBuildGaugeFill->SetExtractionArea(1905, 167, 1905 + 60, 157 + 50);
	m_pBuildGaugeFill->SetOutputArea(POINT({ 0, 0 }), 60, m_ciGaugeLength);

	UpdateGauge(m_fProgress);
}

CUI_BuildGauge::~CUI_BuildGauge()
{
	Release();
}

void CUI_BuildGauge::Ready(void)
{
}

int CUI_BuildGauge::Update(float _fDeltaTime)
{
	// 수동 객체라서, 직접 값을 업데이트 시켜주어야 함.
	return 0;
}

void CUI_BuildGauge::LateUpdate(void)
{
}

void CUI_BuildGauge::Release(void)
{
	SafelyDeleteObj(m_pUnitSign);
	SafelyDeleteObj(m_pBuildGaugeFill);
}

void CUI_BuildGauge::Render(CCamera * _pCamera)
{
	m_pUnitSign->Render(_pCamera);
	if(m_fProgress < 1.f)
		m_pBuildGaugeFill->Render(_pCamera);
}

void CUI_BuildGauge::UpdateGauge(float _fCurrentVal, float _fMaxVal)
{
	UpdateGauge(_fCurrentVal / _fMaxVal);
}

void CUI_BuildGauge::UpdateGauge(float _fProgress)
{
	m_fProgress = Clamp(_fProgress, 0.f, 1.f);
	RECT rcOutputArea = m_pBuildGaugeFill->GetOutputArea();
	rcOutputArea.top = rcOutputArea.bottom - m_fProgress * m_ciGaugeLength;
	m_pBuildGaugeFill->SetSyncOutputArea(rcOutputArea);
}
