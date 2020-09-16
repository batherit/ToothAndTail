#include "stdafx.h"
#include "CAI_ComState_Running_GatheringForDefense.h"
#include "CAI_ComState_Standing_WavingFlag.h"
#include "CAI_ComState_Idle.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"

CAI_ComState_Running_GatheringForDefense::CAI_ComState_Running_GatheringForDefense(CGameWorld & _rGameWorld, CCommanderAI & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CAI_ComState_Running_GatheringForDefense::~CAI_ComState_Running_GatheringForDefense()
{
}

void CAI_ComState_Running_GatheringForDefense::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 32, 10, 1.f, 0, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);

	// 명령어 작성
	CommandInfo tNewCommandInfo;
	tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
	tNewCommandInfo.iUnitID = -1;
	tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
	m_rOwner.SetCommandInfo(tNewCommandInfo);
	m_bIsGathering = true;
	m_fTickTime = 0.f;
}

int CAI_ComState_Running_GatheringForDefense::Update(float _fDeltaTime)
{
	m_rOwner.GetCurrentCommandInfo().vTargetPos = m_rOwner.GetXY();

	if ((m_fTickTime += _fDeltaTime) >= 0.8f) {
		if (m_bIsGathering) {
			AnimInfo stAnimInfo(0, 8, 32, 10, 1.f, 0, false);
			m_rOwner.SetNewAnimInfo(stAnimInfo);

			CommandInfo tNewCommandInfo;
			tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
			tNewCommandInfo.iUnitID = -1;
			tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
			m_rOwner.SetCommandInfo(tNewCommandInfo);
			m_bIsGathering = false;
		}
		else {
			AnimInfo stAnimInfo(0, 8, 22, 10, 1.0f, 0, false);
			m_rOwner.SetNewAnimInfo(stAnimInfo);
			m_rOwner.SetCommandInfo(CommandInfo());
			m_bIsGathering = true;
		}
		m_fTickTime = 0.f;
	}

	if (!m_rOwner.MoveAlongPath(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_WavingFlag(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Running_GatheringForDefense::LateUpdate(void)
{
}

void CAI_ComState_Running_GatheringForDefense::OnExited(void)
{
	m_rOwner.SetCommandInfo(CommandInfo());	// 더미 커맨드 인포 세팅
}
