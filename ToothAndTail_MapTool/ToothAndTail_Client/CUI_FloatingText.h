#pragma once
#include "CObj.h"

class CCamera;
class CUI_FloatingText :
	public CObj
{
public:
	CUI_FloatingText(CGameWorld& _rGameWorld, float _fX, float _fY, const wstring& _wstrText, float _fLifeTime = 0.7f);
	virtual ~CUI_FloatingText();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(CCamera* _pCamera);
	virtual void Ready(void) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;

private:
	wstring m_wstrText = L"";
	const float m_cfLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

