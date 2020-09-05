#pragma once
#include "CState.h"

class CSquirrel;
class CSquirrelState_Run :
	public CState<CSquirrel>
{
public:
	CSquirrelState_Run(CGameWorld& _rGameWorld, CSquirrel& _rOwner);
	virtual ~CSquirrelState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

