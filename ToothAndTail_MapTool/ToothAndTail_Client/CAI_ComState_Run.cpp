#include "stdafx.h"
#include "CAI_ComState_Run.h"
#include "CAI_ComState_Idle.h"
#include "CAI_ComState_Standing_Action.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"

CAI_ComState_Run::CAI_ComState_Run(CGameWorld & _rGameWorld, CCommanderAI & _rOwner, bool _bIsActivating)
	:
	CState(_rGameWorld, _rOwner),
	m_bIsActivating(_bIsActivating)
{
}

CAI_ComState_Run::~CAI_ComState_Run()
{
}

void CAI_ComState_Run::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 22, 10, 1.0f, 0, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);
}

int CAI_ComState_Run::Update(float _fDeltaTime)
{
	if (!m_rOwner.MoveAlongPath(_fDeltaTime)) {
		if(m_bIsActivating) m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_Action(m_rGameWorld, m_rOwner));
		else m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
	}
		 
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Run::LateUpdate(void)
{
}

void CAI_ComState_Run::OnExited(void)
{
}
