#include "stdafx.h"
#include "CAI_ComState_Standing_Action.h"
#include "CAI_ComState_Idle.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"

CAI_ComState_Standing_Action::CAI_ComState_Standing_Action(CGameWorld & _rGameWorld, CCommanderAI & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CAI_ComState_Standing_Action::~CAI_ComState_Standing_Action()
{
}

void CAI_ComState_Standing_Action::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 20, 2, 0.15f, 1, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(0);
	m_rOwner.SetActivating(true);
	m_rOwner.AdjustTunnelGeneratorIndex();
	m_rOwner.GenerateTunnel();
}

int CAI_ComState_Standing_Action::Update(float _fDeltaTime)
{
	if (!m_rOwner.IsAnimPlaying())
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Standing_Action::LateUpdate(void)
{
}

void CAI_ComState_Standing_Action::OnExited(void)
{
	m_rOwner.SetActivating(false);
}
