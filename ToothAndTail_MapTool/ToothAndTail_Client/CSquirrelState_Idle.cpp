#include "stdafx.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrelState_Run.h"
#include "CSquirrel.h"
#include "CStateMgr.h"

CSquirrelState_Idle::CSquirrelState_Idle(CGameWorld & _rGameWorld, CSquirrel & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSquirrelState_Idle::~CSquirrelState_Idle()
{
}

void CSquirrelState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CSquirrelState_Idle::Update(float _fDeltaTime)
{
	if (m_rOwner.IsThereNewTargetPoint()) {
		m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
	}
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Idle::LateUpdate(void)
{
}

void CSquirrelState_Idle::OnExited(void)
{
}
