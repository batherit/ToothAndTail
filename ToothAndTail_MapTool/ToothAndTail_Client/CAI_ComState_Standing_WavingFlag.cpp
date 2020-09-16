#include "stdafx.h"
#include "CAI_ComState_Standing_WavingFlag.h"
#include "CAI_ComState_Idle.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"


CAI_ComState_Standing_WavingFlag::CAI_ComState_Standing_WavingFlag(CGameWorld & _rGameWorld, CCommanderAI & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CAI_ComState_Standing_WavingFlag::~CAI_ComState_Standing_WavingFlag()
{
}

void CAI_ComState_Standing_WavingFlag::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	AnimInfo stAnimInfo(0, 8, 10, 10, 1.f, 0, false); // 무한 애님 테스트
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_fKeepTime = GetNumberMinBetweenMax(3.f, 5.f);
}

int CAI_ComState_Standing_WavingFlag::Update(float _fDeltaTime)
{
	// 주변 유닛들을 검색
	CommandInfo tNewCommandInfo;
	tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
	tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
	tNewCommandInfo.iUnitID = -1;
	if ((m_fElapsedTime += _fDeltaTime) >= 0.2f) {
		// 적을 탐색하고, 적을 발견했다면 이 적을 집중 공격하라
		m_rOwner.DetectUnitsAround();
		if (m_rOwner.GetTargetEnemy()) {
			tNewCommandInfo.eCommand = COMMANDER::COMMAND_SATURATION;
			tNewCommandInfo.pTarget = m_rOwner.GetTargetEnemy();
		}
	}
	m_rOwner.SetCommandInfo(tNewCommandInfo);

	if (m_fElapsedTime >= m_fKeepTime) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Standing_WavingFlag::LateUpdate(void)
{
}

void CAI_ComState_Standing_WavingFlag::OnExited(void)
{
	// 일정 시간 공격불가
	m_rOwner.SetPossibleInvade(false);
	m_rOwner.SetCommandInfo(CommandInfo());	// 더미 커맨드 인포 세팅
}
