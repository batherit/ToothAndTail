#include "stdafx.h"
#include "CMoleState_Run.h"
#include "CMoleState_Idle.h"
#include "CMoleState_Attack.h"
#include "CMole.h"
#include "CStateMgr.h"
#include "CCommander.h"

CMoleState_Run::CMoleState_Run(CGameWorld & _rGameWorld, CMole & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CMoleState_Run::~CMoleState_Run()
{
}

void CMoleState_Run::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 4, 8, 0.5f, 0, false));
	m_rOwner.SetSpeed(MOLE_SPEED);
}

int CMoleState_Run::Update(float _fDeltaTime)
{
	// 커멘더의 명령을 확인한다.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// 주변에 적이 있는지 확인해본다.
		m_rOwner.DetectEnemyAround();
		if (m_rOwner.GetTargetEnemy()) {
			// 주변에 적을 감지했다면, 공격 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Attack(m_rGameWorld, m_rOwner));
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 명령에 해당하는 병력은 새로운 목표지점을 세팅한다.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : 타겟을 향해 집중 공격한다.
		// 집중공격.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 기수가 선정한 타겟이 유효한가?
			DO_IF_IS_VALID_OBJ(tCommandInfo.pTarget) {
				// 공격 타겟은 기수가 선정한 타겟이다.
				m_rOwner.SetTargetPos(tCommandInfo.pTarget->GetXY());
				m_rOwner.SetTargetEnemy(tCommandInfo.pTarget);
				if (m_rOwner.CanAttackTargetEnemy()) {
					// 적을 공격할 수 있는 상황이라면, 바로 공격한다.
					m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					// 적을 공격할 수 없는 거리에 있다면 적이 있는 곳으로 달려간다.
					m_rOwner.GoToTargetPoint(_fDeltaTime);
				}
			}
			ELSE{
				if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
					// 명령에 해당하는 병력은 새로운 목표지점을 세팅한다.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				}
				if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
					m_rOwner.GetStateMgr()->SetNextState(new CMoleState_Idle(m_rGameWorld, m_rOwner));
				}
			}
		}

		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CMoleState_Run::LateUpdate(void)
{
}

void CMoleState_Run::OnExited(void)
{
}
