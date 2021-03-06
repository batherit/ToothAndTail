#include "stdafx.h"
#include "CFoxState_Run.h"
#include "CFoxState_Idle.h"
#include "CFoxState_Attack.h"
#include "CFox.h"
#include "CStateMgr.h"
#include "CCommander.h"

CFoxState_Run::CFoxState_Run(CGameWorld & _rGameWorld, CFox & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CFoxState_Run::~CFoxState_Run()
{
}

void CFoxState_Run::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 8, 8, 0.5f, 0, false));
	m_rOwner.SetSpeed(FOX_SPEED);
}

int CFoxState_Run::Update(float _fDeltaTime)
{
	// 커멘더의 명령을 확인한다.
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		if (m_rOwner.GetTargetEnemy()) {
			if (m_rOwner.CanAttackTargetEnemy()) {
				// 주변에 적을 감지했다면, 공격 상태로 전환한다.
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
			}
			else {
				// 주변에 적을 감지했지만, 공격 범위 밖에 있으면 타겟쪽으로 달려간다.
				m_rOwner.GoToTarget(_fDeltaTime);
			}
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Idle(m_rGameWorld, m_rOwner));
		}

		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 명령에 해당하는 병력은 새로운 목표지점을 세팅한다.
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
		}
		else if (m_rOwner.GetTargetEnemy()) {
			// 주변에 적을 감지했다면, 공격 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
		}
		// 목표 지점까지 달리기를 수행한다.
		if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) { // 이동에 실패하다 => 목표지점에 도착했다, 갈 곳이 없다.
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Idle(m_rGameWorld, m_rOwner));
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
			}
			ELSE{
				if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
					// 명령에 해당하는 병력은 새로운 목표지점을 세팅한다.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				}
			}
		}
		if (m_rOwner.CanAttackTargetEnemy()) {
			// 적을 공격할 수 있는 상황이라면, 바로 공격한다.
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
		}
		else if (!m_rOwner.GoToTargetPoint(_fDeltaTime)) {
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Idle(m_rGameWorld, m_rOwner));
		}
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CFoxState_Run::LateUpdate(void)
{
}

void CFoxState_Run::OnExited(void)
{
}
