#include "stdafx.h"
#include "CUI_FadeInOut.h"
#include "CTextureMgr.h"

CUI_FadeInOut::CUI_FadeInOut(CGameWorld & _rGameWorld)
	:
	CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"LAYER"), D3DXVECTOR3(WINCX >> 1, WINCY >> 1, 0.f))
{
	SetRenderColor(D3DCOLOR_ARGB(0, 0, 0, 0));
	SetOutputArea(0, 0, WINCX, WINCY);
}

CUI_FadeInOut::~CUI_FadeInOut()
{
}

int CUI_FadeInOut::Update(float _fDeltaTime)
{
	if (!m_bIsEffectStarting) return 0;

	if ((m_fKeepTime -= _fDeltaTime) > 0.f) {
		float fT = m_fKeepTime / m_fMaxKeepTime;	// 1->0으로 변화
		if (fT > 1.f) fT = 1.f;
		if (fT < 0.f) fT = 0.f;
		if (m_bIsFadeIn) {
			// 점점 밝아지는
			SetRenderColor(D3DCOLOR_ARGB(static_cast<int>(255 * fT), 0, 0, 0));
		}
		else {
			// 점점 어두워지는
			SetRenderColor(D3DCOLOR_ARGB(static_cast<int>(255 * (1.f - fT)), 0, 0, 0));
		}
	}
	else {
		if (m_bIsFadeIn) {
			// 점점 밝아지는
			SetRenderColor(D3DCOLOR_ARGB(0, 0, 0, 0));
		}
		else {
			// 점점 어두워지는
			SetRenderColor(D3DCOLOR_ARGB(255, 0, 0, 0));
		}
		m_bIsEffectStarting = false;

		// 효과가 종료되었을때 1을 반환한다.
		return 1;
	}

	return 0;
}

void CUI_FadeInOut::Render(CCamera * _pCamera)
{
	if (!m_bIsEffectStarting && m_bIsFadeIn) return;
	CUI_Image::Render(_pCamera);
}

void CUI_FadeInOut::StartFadeInOut(float _fKeepTime, bool _bIsFadeIn)
{
	if (_fKeepTime <= 0.f) return;

	m_fKeepTime = _fKeepTime;
	m_fMaxKeepTime = _fKeepTime;
	m_bIsFadeIn = _bIsFadeIn;

	if (m_bIsFadeIn)
		// 점점 밝아지는 => 게임 진입 시도 //타이틀 씬
		SetRenderColor(D3DCOLOR_ARGB(0, 0, 0, 0));
	else
		// 점점 어두워지는 => 게임 진입 완료 // 플레이 씬
		SetRenderColor(D3DCOLOR_ARGB(255, 0, 0, 0));

	m_bIsEffectStarting = true;
}
