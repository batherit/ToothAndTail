#pragma once
#include "CState.h"

class CFox;
class CFoxState_Run :
	public CState<CFox>
{
public:
	CFoxState_Run(CGameWorld& _rGameWorld, CFox& _rOwner);
	virtual ~CFoxState_Run();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

