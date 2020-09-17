#pragma once
#include "CUI_Image.h"
class CUI_FadeInOut :
	public CUI_Image
{
public:
	CUI_FadeInOut(CGameWorld& _rGameWorld);
	virtual ~CUI_FadeInOut();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(CCamera* _pCamera) override;
	void StartFadeInOut(float _fKeepTime, bool _bIsFadeIn);

public:
	bool m_bIsEffectStarting = false;
	bool m_bIsFadeIn = true;
	float m_fKeepTime = 0.f;
	float m_fMaxKeepTime = 0.f;
};

