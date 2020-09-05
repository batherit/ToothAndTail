#include "stdafx.h"
#include "CSkunkState_Idle.h"
#include "CSkunk.h"

CSkunkState_Idle::CSkunkState_Idle(CGameWorld & _rGameWorld, CSkunk & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSkunkState_Idle::~CSkunkState_Idle()
{
}

void CSkunkState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CSkunkState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSkunkState_Idle::LateUpdate(void)
{
}

void CSkunkState_Idle::OnExited(void)
{
}