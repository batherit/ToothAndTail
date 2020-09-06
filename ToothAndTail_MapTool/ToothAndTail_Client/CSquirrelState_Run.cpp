#include "stdafx.h"
#include "CSquirrelState_Run.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrelState_Attack.h"
#include "CSquirrel.h"
#include "CStateMgr.h"
#include "CCommander.h"

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
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 5, 0.5f, 0, false));
	m_rOwner.SetSpeed(SQUIRREL_SPEED);
}

int CSquirrelState_Run::Update(float _fDeltaTime)
{
	// 커멘더의 명령을 확인한다.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// 주변에 적이 있는지 확인해본다.
		m_rOwner.DetectEnemyAround();
		if (m_rOwner.GetTargetEnemy()) {
			// 주변에 적을 감지했다면, 공격 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Attack(m_rGameWorld, m_rOwner));
		}
		else if(!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 명령에 해당하는 병력은 새로운 목표지점을 세팅한다.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : 타겟을 향해 집중 공격한다.
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Run::LateUpdate(void)
{
}

void CSquirrelState_Run::OnExited(void)
{
}
