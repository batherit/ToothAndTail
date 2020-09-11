#include "stdafx.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrelState_Run.h"
#include "CSquirrelState_Attack.h"
#include "CSquirrel.h"
#include "CStateMgr.h"
#include "CCommander.h"

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
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CSquirrelState_Idle::Update(float _fDeltaTime)
{
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	m_rOwner.DetectUnitsAround();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// 위치 조정(펼쳐진다.)
		m_rOwner.AdjustPosition(_fDeltaTime);
		if (m_rOwner.GetTargetEnemy()) {
			// 주변에 적을 감지했다면, 공격 상태로 전환한다.
			m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Attack(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			// 해당 부대에게 명령이 내려졌다면,
			// 새 타겟 포인트를 세팅
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);	
			// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다. 목표 지점에 있다면 그자리에 있게 된다.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
		}
		else m_rOwner.AdjustPosition(_fDeltaTime);
		
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
					m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Attack(m_rGameWorld, m_rOwner));
				}
				else {
					// 적을 공격할 수 없는 거리에 있다면 적이 있는 곳으로 달려간다.
					m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
				}
			}
			ELSE {
			// 유효하지 않은 타겟이면 COMMANDER::COMMAND_GATHERING과 같다.
			// 기수가 선정한 타겟이 유효하지 않다면, 기수 위치를 목표 지점으로 삼는다.
				m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);
				// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다.
				if (!m_rOwner.IsLocatedAtTargetPoint())
					m_rOwner.GetStateMgr()->SetNextState(new CSquirrelState_Run(m_rGameWorld, m_rOwner));
				//else
					
			}
		}
		
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Idle::LateUpdate(void)
{
}

void CSquirrelState_Idle::OnExited(void)
{
}
