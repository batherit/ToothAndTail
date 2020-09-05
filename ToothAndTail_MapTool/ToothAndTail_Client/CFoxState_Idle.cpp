#include "stdafx.h"
#include "CFoxState_Idle.h"
//#include "CFoxState_Run.h"
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
	// 새로운 목표 지점을 발견했는지?
	/*if (m_rOwner.IsThereNewTargetPoint()) {
	}*/

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CFoxState_Idle::LateUpdate(void)
{
}

void CFoxState_Idle::OnExited(void)
{
}