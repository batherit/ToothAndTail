#include "stdafx.h"
#include "CSquirrelState_Run.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrel.h"
#include "CStateMgr.h"

CSquirrelState_Run::CSquirrelState_Run(CGameWorld & _rGameWorld, CSquirrel & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSquirrelState_Run::~CSquirrelState_Run()
{
}

void CSquirrelState_Run::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 1.f, 0, false));
	m_rOwner.SetSpeed(SQUIRREL_SPEED);
}

int CSquirrelState_Run::Update(float _fDeltaTime)
{
	if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
		m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Run::LateUpdate(void)
{
}

void CSquirrelState_Run::OnExited(void)
{
}
