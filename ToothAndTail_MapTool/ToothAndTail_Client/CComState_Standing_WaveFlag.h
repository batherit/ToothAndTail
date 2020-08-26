#pragma once
#include "CState.h"
class CCommander;
class CComState_Standing_WaveFlag :
	public CState<CCommander>
{
public:
	CComState_Standing_WaveFlag(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Standing_WaveFlag();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

