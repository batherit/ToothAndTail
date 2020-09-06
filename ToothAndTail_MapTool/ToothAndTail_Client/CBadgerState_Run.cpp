#include "stdafx.h"
#include "CBadgerState_Run.h"
#include "CBadgerState_Idle.h"
#include "CBadger.h"
#include "CStateMgr.h"
#include "CCommander.h"

CBadgerState_Run::CBadgerState_Run(CGameWorld & _rGameWorld, CBadger & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CBadgerState_Run::~CBadgerState_Run()
{
}

void CBadgerState_Run::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 0.5f, 0, false));
	m_rOwner.SetSpeed(BADGER_SPEED);
}

int CBadgerState_Run::Update(float _fDeltaTime)
{
	// 커멘더의 명령을 확인한다.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// 커멘더의 명령이 없다면, 타겟 지점까지 이동하며, 이동 도중 적을 만나면 적과 싸운다.
		// 적을 만난 경우와 만나지 않은 경우로 나누어야 할 것이다. 지금은 적 개념이 없으니 만나지 않은 경우만 고려한다.
		// TODO : 적을 만난 경우
		// 적과 만나지 않은 경우
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 명령에 해당하는 병력은 새로운 목표지점을 세팅한다.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : 타겟을 향해 집중 공격한다.
		break;
	}



	if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
		m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CBadgerState_Run::LateUpdate(void)
{
}

void CBadgerState_Run::OnExited(void)
{
}
