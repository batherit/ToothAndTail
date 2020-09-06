#pragma once
#include "CState.h"

class CBadger;
class CBadgerState_Run :
	public CState<CBadger>
{
public:
	CBadgerState_Run(CGameWorld& _rGameWorld, CBadger& _rOwner);
	virtual ~CBadgerState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};


