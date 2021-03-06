#pragma once
#include "CState.h"

class CSquirrel;
class CSquirrelState_Idle :
	public CState<CSquirrel>
{
public:
	CSquirrelState_Idle(CGameWorld& _rGameWorld, CSquirrel& _rOwner);
	virtual ~CSquirrelState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

