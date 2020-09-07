#include "stdafx.h"
#include "CFoxState_Idle.h"
#include "CFoxState_Run.h"
#include "CFoxState_Attack.h"
#include "CFox.h"
#include "CCommander.h"
#include "CStateMgr.h"

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
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 8, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CFoxState_Idle::Update(float _fDeltaTime)
{
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// 기수가 아무것도 하지 않는다.
		m_rOwner.DetectEnemyAround();
		if (m_rOwner.GetTargetEnemy()) {
			// 주변에 적을 감지했다면, 공격 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_GATHERING:
		// 집결.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);

			// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// 집중공격.
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 기수가 선정한 타겟이 유효한가?
			DO_IF_IS_VALID_OBJ(tCommandInfo.pTarget) {
				// 공격 타겟은 기수가 선정한 타겟이다.
				m_rOwner.SetTargetPos(tCommandInfo.pTarget->GetXY());
				m_rOwner.SetTargetEnemy(tCommandInfo.pTarget);
				if (m_rOwner.CanAttackTargetEnemy()) {
					// 적을 공격할 수 있는 상황이라면, 바로 공격한다.
					m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					// 적을 공격할 수 없는 거리에 있다면 적이 있는 곳으로 달려간다.
					m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
				}
			}
			ELSE{
				// 유효하지 않은 타겟이면 COMMANDER::COMMAND_GATHERING과 같다.
				// 기수가 선정한 타겟이 유효하지 않다면, 기수 위치를 목표 지점으로 삼는다.
					m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
			// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CFoxState_Run(m_rGameWorld, m_rOwner));
			}
		}

		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CFoxState_Idle::LateUpdate(void)
{
}

void CFoxState_Idle::OnExited(void)
{
}