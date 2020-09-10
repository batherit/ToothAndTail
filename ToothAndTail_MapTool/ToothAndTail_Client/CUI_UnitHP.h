#pragma once
#include "CObj.h"

class CCamera;
class CUI_Image;

class CUI_UnitHP :
	public CObj
{
public:
	CUI_UnitHP(CGameWorld& _rGameWorld, CComDepObj* _pOwner);
	virtual ~CUI_UnitHP();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;
	virtual void Render(CCamera* _pCamera) override;

public:
	void DisplayHP(void);
	void CloseHP(void);

private:
	CComDepObj* m_pOwner = nullptr;

	CUI_Image* m_pUnitHPBack = nullptr;
	CUI_Image* m_pUnitHPFill = nullptr;

	const int m_ciHPLength = 40;
	const float m_cfKeepingTime = 3.f;
	float m_fElapsedTime = m_cfKeepingTime;
};

