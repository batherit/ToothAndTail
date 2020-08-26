#pragma once
#include "CState.h"

class CCommander;
class CComState_Running_Jump :
	public CState<CCommander>
{
public:
	CComState_Running_Jump(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Running_Jump();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsBuilded = false;
};

