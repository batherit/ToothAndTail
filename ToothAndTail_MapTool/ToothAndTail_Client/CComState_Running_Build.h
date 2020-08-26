#pragma once
#include "CState.h"

class CCommander;
class CComState_Running_Build :
	public CState<CCommander>
{
public:
	CComState_Running_Build(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Running_Build();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

