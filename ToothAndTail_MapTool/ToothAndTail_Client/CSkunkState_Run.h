#pragma once
#include "CState.h"

class CSkunk;
class CSkunkState_Run :
	public CState<CSkunk>
{
public:
	CSkunkState_Run(CGameWorld& _rGameWorld, CSkunk& _rOwner);
	virtual ~CSkunkState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

