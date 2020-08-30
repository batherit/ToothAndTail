#pragma once
#include "CState.h"

class CPig;
class CPigState_Cropping :
	public CState<CPig>
{
public:
	CPigState_Cropping(CGameWorld& _rGameWorld, CPig& _rOwner);
	virtual ~CPigState_Cropping();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

