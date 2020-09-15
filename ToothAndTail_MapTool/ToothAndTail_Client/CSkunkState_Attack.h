#pragma once
#include "CState.h"

class CSkunk;
class CSkunkState_Attack :
	public CState<CSkunk>
{
public:
	CSkunkState_Attack(CGameWorld& _rGameWorld, CSkunk& _rOwner);
	virtual ~CSkunkState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsShooting = false;
	float m_fElapsedTime = 0.f;
	float m_fAttackDelayOffset = 0.f;
};

