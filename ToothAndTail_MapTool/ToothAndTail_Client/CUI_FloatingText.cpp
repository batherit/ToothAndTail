#include "stdafx.h"
#include "CUI_FloatingText.h"
#include "CCamera.h"

CUI_FloatingText::CUI_FloatingText(CGameWorld & _rGameWorld, float _fX, float _fY, const wstring & _wstrText, float _fLifeTime)
	:
	CObj(_rGameWorld, _fX, _fY, 10.f, 10.f, 0.f, -1.f, 90.f),
	m_wstrText(_wstrText),
	m_cfLifeTime(_fLifeTime)
{
	SetRenderLayer(20);
}

CUI_FloatingText::~CUI_FloatingText()
{
	Release();
}

int CUI_FloatingText::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if ((m_fElapsedTime += _fDeltaTime) > m_cfLifeTime) {
		SetValid(false);
		return 1;
	}
	MoveByDeltaTime(_fDeltaTime);
	return 0;
}

void CUI_FloatingText::Render(CCamera * _pCamera)
{
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

	CGraphicDevice::GetInstance()->RenderText(m_wstrText, vPos, 0.25f, D3DCOLOR_ARGB(255, 230, 230, 230));
}

void CUI_FloatingText::Ready(void)
{
}

void CUI_FloatingText::LateUpdate(void)
{
}

void CUI_FloatingText::Release(void)
{
}
