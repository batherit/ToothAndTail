#pragma once
#include "CObj.h"
class CUnitGenerator :
	public CObj
{
public:
	CUnitGenerator(CGameWorld& _rGameWorld);
	virtual ~CUnitGenerator();

public:
	// CObj을(를) 통해 상속됨
	virtual void Ready(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void Release(void) override;

protected:
	float m_fElapedTime = 0.f;
};

