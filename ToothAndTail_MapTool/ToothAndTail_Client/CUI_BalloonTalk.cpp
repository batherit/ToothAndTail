#include "stdafx.h"
#include "CUI_BalloonTalk.h"
#include "CCamera.h"

CUI_BalloonTalk::CUI_BalloonTalk(CGameWorld & _rGameWorld, CObj* _pParent)
	:
	CObj(_rGameWorld, 0.f, 0.f)
{
	SetRenderLayer(22);
	SetVisible(false);
	SetParent(_pParent);
}



CUI_BalloonTalk::~CUI_BalloonTalk()
{
	Release();
}

int CUI_BalloonTalk::Update(float _fDeltaTime)
{
	if (IsVisible()) {
		if ((m_fElapsedTime += _fDeltaTime) > m_fLifeTime) {
			SetVisible(false);
			m_fElapsedTime = 0.f;
			return 1;
		}
	}
	

	return 0;
}

void CUI_BalloonTalk::Render(CCamera * _pCamera)
{
	if (!IsVisible()) return;

	D3DXMATRIX matScreen = GetObjectMatrix();
	// RenderOffset을 적용한다.
	matScreen._41 += GetRenderOffsetX();
	matScreen._42 += GetRenderOffsetY();
	if (_pCamera) matScreen = _pCamera->GetScreenMatrix(matScreen);

	// 컬링을 적용한다.
	if (!IsObjInCamera(this, _pCamera)) return;

	// 객체를 그린다.
	D3DXVECTOR3 vPos;
	vPos.x = matScreen._41;
	vPos.y = matScreen._42;
	vPos.z = matScreen._43;

	CGraphicDevice::GetInstance()->RenderText(m_wstrText, vPos, 0.5f, D3DCOLOR_ARGB(255, 230, 230, 230));
}

void CUI_BalloonTalk::Ready(void)
{
}

void CUI_BalloonTalk::LateUpdate(void)
{
}

void CUI_BalloonTalk::Release(void)
{
}

void CUI_BalloonTalk::SetText(float _fOffsetX, float _fOffsetY, const wstring & _wstrText, float _fLifeTime)
{
	if (_wstrText == L"") return;

	//SetXY(_fOffsetX, _fOffsetY);
	SetRenderOffsetXY(_fOffsetX, _fOffsetY);
	m_wstrText = _wstrText;
	m_fLifeTime = _fLifeTime;
	m_fElapsedTime = 0.f;
	SetVisible(true);
}

