#pragma once
#include "CState.h"

class CBadger;
class CBadgerState_Run :
	public CState<CBadger>
{
public:
	CBadgerState_Run(CGameWorld& _rGameWorld, CBadger& _rOwner);
	virtual ~CBadgerState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};


