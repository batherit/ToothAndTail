#include "stdafx.h"
#include "CAI_ComState_Running_Gathering.h"
#include "CAI_ComState_Idle.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"

CAI_ComState_Running_Gathering::CAI_ComState_Running_Gathering(CGameWorld & _rGameWorld, CCommanderAI & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CAI_ComState_Running_Gathering::~CAI_ComState_Running_Gathering()
{
}

void CAI_ComState_Running_Gathering::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 32, 10, 1.f, 0, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);
	m_rOwner.SetWavingFlag(false);

	// 명령어 작성
	CommandInfo tNewCommandInfo;
	tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
	tNewCommandInfo.iUnitID = -1;
	tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
	m_rOwner.SetCommandInfo(tNewCommandInfo);
}

int CAI_ComState_Running_Gathering::Update(float _fDeltaTime)
{
	m_rOwner.GetCurrentCommandInfo().vTargetPos = m_rOwner.GetXY();

	if (!m_rOwner.MoveAlongPath(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Running_Gathering::LateUpdate(void)
{
}

void CAI_ComState_Running_Gathering::OnExited(void)
{
	m_rOwner.SetCommandInfo(CommandInfo());	// 더미 커맨드 인포 세팅
	m_rOwner.SetWavingFlag(false);
}
