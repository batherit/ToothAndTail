#pragma once
#include "CState.h"

class CBadger;
class CBadgerState_Idle :
	public CState<CBadger>
{
public:
	CBadgerState_Idle(CGameWorld& _rGameWorld, CBadger& _rOwner);
	virtual ~CBadgerState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

