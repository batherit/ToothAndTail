#pragma once
#include "CState.h"

class CCommander;
class CComState_Run :
	public CState<CCommander>
{
public:
	CComState_Run(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

