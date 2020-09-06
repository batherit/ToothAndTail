#pragma once
#include "CState.h"

class CPig;
class CPigState_Attack :
	public CState<CPig>
{
public:
	CPigState_Attack(CGameWorld& _rGameWorld, CPig& _rOwner);
	virtual ~CPigState_Attack();

	// CState을(를) 통해 상속됨
	virtual void OnLoaded(void) override;
	virtual int Update(float _fDeltaTime) override;
	virtual void LateUpdate(void) override;
	virtual void OnExited(void) override;

private:
	bool m_bIsShooting = false;
	float m_fElapsedTime = 0.f;
};

