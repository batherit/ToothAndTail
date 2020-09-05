#pragma once
#include "CState.h"

class CFox;
class CFoxState_Idle :
	public CState<CFox>
{
public:
	CFoxState_Idle(CGameWorld& _rGameWorld, CFox& _rOwner);
	virtual ~CFoxState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

