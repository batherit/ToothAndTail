#pragma once
#include "CState.h"

class CLizard;
class CLizardState_Attack :
	public CState<CLizard>
{
public:
	CLizardState_Attack(CGameWorld& _rGameWorld, CLizard& _rOwner);
	virtual ~CLizardState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsShooting = false;
	float m_fElapsedTime = 0.f;
	//float m_fTickTime = 0.f;
};

