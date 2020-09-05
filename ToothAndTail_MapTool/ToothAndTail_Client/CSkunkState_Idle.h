#pragma once
#include "CState.h"

class CSkunk;
class CSkunkState_Idle :
	public CState<CSkunk>
{
public:
	CSkunkState_Idle(CGameWorld& _rGameWorld, CSkunk& _rOwner);
	virtual ~CSkunkState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

