#pragma once
#include "CState.h"

class CSquirrel;
class CSquirrelState_Attack :
	public CState<CSquirrel>
{
public:
	CSquirrelState_Attack(CGameWorld& _rGameWorld, CSquirrel& _rOwner);
	virtual ~CSquirrelState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsShooting = false;
	float m_fElapsedTime = 0.f;
	//float m_fTickTime = 0.f;
};

