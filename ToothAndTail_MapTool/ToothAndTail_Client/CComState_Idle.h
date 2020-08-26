#pragma once
#include "CState.h"

class CCommander;
class CComState_Idle :
	public CState<CCommander>
{
public:
	CComState_Idle(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Idle();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

