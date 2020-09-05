#include "stdafx.h"
#include "CFoxState_Idle.h"
#include "CFox.h"

CFoxState_Idle::CFoxState_Idle(CGameWorld & _rGameWorld, CFox & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CFoxState_Idle::~CFoxState_Idle()
{
}

void CFoxState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CFoxState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CFoxState_Idle::LateUpdate(void)
{
}

void CFoxState_Idle::OnExited(void)
{
}