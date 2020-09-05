#include "stdafx.h"
#include "CBadgerState_Idle.h"
#include "CBadger.h"

CBadgerState_Idle::CBadgerState_Idle(CGameWorld & _rGameWorld, CBadger & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CBadgerState_Idle::~CBadgerState_Idle()
{
}

void CBadgerState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CBadgerState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CBadgerState_Idle::LateUpdate(void)
{
}

void CBadgerState_Idle::OnExited(void)
{
}