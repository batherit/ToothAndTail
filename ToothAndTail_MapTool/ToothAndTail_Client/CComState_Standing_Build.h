#pragma once
#include "CState.h"

class CCommander;
class CComState_Standing_Build :
	public CState<CCommander>
{
public:
	CComState_Standing_Build(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Standing_Build();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;
};

