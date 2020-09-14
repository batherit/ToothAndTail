#include "stdafx.h"
#include "CParticle_Blood.h"
#include "CTextureMgr.h"

CParticle_Blood::CParticle_Blood(CGameWorld & _rGameWorld, float _fX, float _fY)
	:
	CUI_Image(_rGameWorld, CTextureMgr::GetInstance()->GetTextureInfo(L"PARTICLE"), D3DXVECTOR3(_fX + GetNumberMinBetweenMax(-30.f, 30.f), _fY + GetNumberMinBetweenMax(-10.f, 10.f), 0.f))
{
	SetRenderLayer(1);
	m_fLifeTime = GetNumberMinBetweenMax(15.f, 22.f);
	SetScaleX(GetNumberMinBetweenMax(1.5f, 2.f));
	SetScaleY(GetNumberMinBetweenMax(1.f, 1.5f));
	SetRenderColor(D3DCOLOR_ARGB(GetNumberMinBetweenMax(20, 90), GetNumberMinBetweenMax(50, 120), 0, 0));
}

CParticle_Blood::~CParticle_Blood()
{
}

int CParticle_Blood::Update(float _fDeltaTime)
{
	DO_IF_IS_NOT_VALID_OBJ(this) return 1;

	if ((m_fElapsedTime += _fDeltaTime) < m_fLifeTime) {
	/*	float fT = 1.f - m_fElapsedTime / m_fLifeTime;
		if (fT < 0.f) fT = 0.f;
		SetRenderColor(D3DCOLOR_ARGB(static_cast<int>(150 * fT), 50, 0, 0));*/
	}
	else {
		SetValid(false);
	}

	return 0;
}
