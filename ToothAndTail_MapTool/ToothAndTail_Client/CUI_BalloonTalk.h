#pragma once
#include "CObj.h"
class CUI_BalloonTalk :
	public CObj
{
public:
	CUI_BalloonTalk(CGameWorld& _rGameWorld, CObj* _pParent);
	virtual ~CUI_BalloonTalk();

public:
	virtual int Update(float _fDeltaTime) override;
	virtual void Render(CCamera* _pCamera);
	virtual void Ready(void) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;

public:
	void SetText(float _fOffsetX, float _fOffsetY, const wstring& _wstrText, float _fLifeTime = 2.f);

private:
	wstring m_wstrText = L"";
	float m_fLifeTime = 0.f;
	float m_fElapsedTime = 0.f;
};

