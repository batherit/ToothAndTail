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
	AnimInfo stAnimInfo(0, 8, 10, 10, 1.f, 0, false); // ���� �ִ� �׽�Ʈ
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_fKeepTime = GetNumberMinBetweenMax(4.f, 5.f);
}

int CAI_ComState_Standing_WavingFlag::Update(float _fDeltaTime)
{
	m_rOwner.UpdateAnim(_fDeltaTime);

	auto& vecMyWindmills = m_rOwner.GetMyWindmills();
	if (vecMyWindmills.empty()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
		return 1;
	}

	// �ֺ� ���ֵ��� �˻�
	CommandInfo tNewCommandInfo;
	tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
	tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
	tNewCommandInfo.iUnitID = -1;
	if ((m_fElapsedTime += _fDeltaTime) >= 0.2f) {
		// ���� Ž���ϰ�, ���� �߰��ߴٸ� �� ���� ���� �����϶�
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

	if ((m_fTickTime += _fDeltaTime) >= 1.0f) {
		if (m_bIsGathering) {
			// ���� ��� ���� �ִϸ��̼�
			AnimInfo stAnimInfo(0, 8, 10, 10, 1.f, 0, false); // ���� �ִ� �׽�Ʈ
			m_rOwner.SetNewAnimInfo(stAnimInfo);

			CommandInfo tNewCommandInfo;
			tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
			tNewCommandInfo.iUnitID = -1;
			tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
			m_rOwner.SetCommandInfo(tNewCommandInfo);
			m_bIsGathering = false;
		}
		else {
			// ���̵� �ִϸ��̼�
			AnimInfo stAnimInfo(0, 8, 0, 10, 1.f, 0, false); // ���� �ִ� �׽�Ʈ
			m_rOwner.SetNewAnimInfo(stAnimInfo);
			m_rOwner.SetCommandInfo(CommandInfo());
			m_bIsGathering = true;
		}
		m_fTickTime = 0.f;
	}

	return 0;
}

void CAI_ComState_Standing_WavingFlag::LateUpdate(void)
{
}

void CAI_ComState_Standing_WavingFlag::OnExited(void)
{
	// ���� �ð� ���ݺҰ�
	m_rOwner.SetPossibleInvade(false);
	m_rOwner.SetCommandInfo(CommandInfo());	// ���� Ŀ�ǵ� ���� ����
}
