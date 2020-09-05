#include "stdafx.h"
#include "CMoleState_Idle.h"
#include "CMole.h"

CMoleState_Idle::CMoleState_Idle(CGameWorld & _rGameWorld, CMole & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CMoleState_Idle::~CMoleState_Idle()
{
}

void CMoleState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CMoleState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CMoleState_Idle::LateUpdate(void)
{
}

void CMoleState_Idle::OnExited(void)
{
}