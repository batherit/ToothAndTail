#pragma once
#include "CState.h"
class CFox;
class CFoxState_Attack :
	public CState<CFox>
{
public:
	CFoxState_Attack(CGameWorld& _rGameWorld, CFox& _rOwner);
	virtual ~CFoxState_Attack();

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

