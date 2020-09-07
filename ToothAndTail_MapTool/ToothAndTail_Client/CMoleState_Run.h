#pragma once
#include "CState.h"

class CMole;
class CMoleState_Run :
	public CState<CMole>
{
public:
	CMoleState_Run(CGameWorld& _rGameWorld, CMole& _rOwner);
	virtual ~CMoleState_Run();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};
