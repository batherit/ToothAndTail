#include "stdafx.h"
#include "CLizardState_Idle.h"
#include "CLizard.h"

CLizardState_Idle::CLizardState_Idle(CGameWorld & _rGameWorld, CLizard & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CLizardState_Idle::~CLizardState_Idle()
{
}

void CLizardState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CLizardState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CLizardState_Idle::LateUpdate(void)
{
}

void CLizardState_Idle::OnExited(void)
{
}
