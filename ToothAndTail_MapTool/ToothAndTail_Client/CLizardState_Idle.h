#pragma once
#include "CState.h"

class CLizard;
class CLizardState_Idle :
	public CState<CLizard>
{
public:
	CLizardState_Idle(CGameWorld& _rGameWorld, CLizard& _rOwner);
	virtual ~CLizardState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

