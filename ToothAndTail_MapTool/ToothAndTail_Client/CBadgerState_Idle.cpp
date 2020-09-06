#include "stdafx.h"
#include "CBadgerState_Idle.h"
#include "CBadgerState_Run.h"
#include "CBadger.h"
#include "CCommander.h"
#include "CStateMgr.h"

CBadgerState_Idle::CBadgerState_Idle(CGameWorld & _rGameWorld, CBadger & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CBadgerState_Idle::~CBadgerState_Idle()
{
}

void CBadgerState_Idle::OnLoaded(void)
{
	// 돼지는 StartPatrol에서 방향을 바꾼다,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 0.5f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CBadgerState_Idle::Update(float _fDeltaTime)
{
	CommandInfo tCommandInfo = m_rOwner.GetCommander()->GetCurrentCommandInfo();
	switch (tCommandInfo.eCommand) {
	case COMMANDER::COMMAND_NOTHING:
		// TODO : 아무것도 하지 않는다.
		break;
	case COMMANDER::COMMAND_GATHERING:
		if (-1 == tCommandInfo.iUnitID || m_rOwner.GetID() == tCommandInfo.iUnitID) {
			m_rOwner.SetTargetPos(tCommandInfo.vTargetPos);

			// 목표 지점에 위치해있지 않다면 달리기 상태로 변경한다.
			if (!m_rOwner.IsLocatedAtTargetPoint())
				m_rOwner.GetStateMgr()->SetNextState(new CBadgerState_Run(m_rGameWorld, m_rOwner));
		}
		break;
	case COMMANDER::COMMAND_SATURATION:
		// TODO : 타겟을 향해 집중 공격한다.
		break;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CBadgerState_Idle::LateUpdate(void)
{
}

void CBadgerState_Idle::OnExited(void)
{
}