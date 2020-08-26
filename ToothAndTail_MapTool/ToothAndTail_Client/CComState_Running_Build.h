#pragma once
#include "CState.h"

class CCommander;
class CComState_Running_Build :
	public CState<CCommander>
{
public:
	CComState_Running_Build(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Running_Build();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

