#pragma once
#include "CState.h"

class CCommander;
class CComState_Idle :
	public CState<CCommander>
{
public:
	CComState_Idle(CGameWorld& _rGameWorld, CCommander& _rOwner);
	virtual ~CComState_Idle();

	// CState��(��) ���� ��ӵ�
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	wstring m_wstrBuildText[3];
	float m_fShoutOutTime = 0.f;
};

