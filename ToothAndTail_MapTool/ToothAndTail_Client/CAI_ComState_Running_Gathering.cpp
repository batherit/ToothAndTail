#include "stdafx.h"
#include "CAI_ComState_Running_Gathering.h"
#include "CAI_ComState_Running_GatheringForDefense.h"
#include "CAI_ComState_Standing_WavingFlag.h"
#include "CAI_ComState_Idle.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CWindmill.h"
#include "CFarmland.h"
#include "CPig.h"

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

	// ��ɾ� �ۼ�
	CommandInfo tNewCommandInfo;
	tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
	tNewCommandInfo.iUnitID = -1;
	tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
	m_rOwner.SetCommandInfo(tNewCommandInfo);
	m_bIsGathering = true;
	m_fRepeatTime = 1.f;
	m_fTickTime = 0.f;
}

int CAI_ComState_Running_Gathering::Update(float _fDeltaTime)
{
	m_rOwner.UpdateAnim(_fDeltaTime);

	auto& vecMyWindmills = m_rOwner.GetMyWindmills();
	if (vecMyWindmills.empty()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
		return 1;
	}

	D3DXVECTOR3 vGoalPos;
	CWindmill* pWindmillToAttack = m_rOwner.GetWindmillToAttack();
	if(!pWindmillToAttack || !pWindmillToAttack->IsAttackedRecently()) {
		// �����Ϸ��� ���мҰ� ���ų�, �����Ϸ��� ���мҰ� ���ݴ����� �ʰ� �ִٸ�, �Ʒ��� ������ �����Ѵ�.
		for (auto& pMyWindmill : vecMyWindmills) {
			if (pMyWindmill->IsAttackedRecently()) {
				// �÷��̾� ���мҷ� ������ �̵���Ų��.
				vGoalPos = pMyWindmill->GetXY();
				vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
				if (m_rOwner.GeneratePathToGoal(vGoalPos, pMyWindmill)) {
					// TODO : CAI_ComState_Running_WaveFlag�� �����ϸ� �� �� ����. ������ Run���� ����
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Running_GatheringForDefense(m_rGameWorld, m_rOwner));
				}
				else {
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_WavingFlag(m_rGameWorld, m_rOwner));
				}
				return 0;
			}
			else {
				CPig* pPig = nullptr;
				for (auto& pFarmland : pMyWindmill->GetFarmlands()) {
					pPig = pFarmland->GetPig();
					if (pPig && pPig->IsAttackedRecently()) {
						if (m_rOwner.GeneratePathToGoal(pFarmland->GetXY(), pMyWindmill)) {
							// TODO : CAI_ComState_Running_WaveFlag�� �����ϸ� �� �� ����. ������ Run���� ����
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Running_GatheringForDefense(m_rGameWorld, m_rOwner));
						}
						else {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_WavingFlag(m_rGameWorld, m_rOwner));
						}
						return 0;
					}
				}
			}
		}
	}

	m_rOwner.GetCurrentCommandInfo().vTargetPos = m_rOwner.GetXY();

	if ((m_fTickTime += _fDeltaTime) >= m_fRepeatTime) {
		if (m_bIsGathering) {
			// �׳� �޸���
			AnimInfo stAnimInfo(0, 8, 22, 10, 1.0f, 0, false);
			m_rOwner.SetNewAnimInfo(stAnimInfo);
			m_rOwner.SetCommandInfo(CommandInfo());
			m_fRepeatTime = 3.f;
			m_bIsGathering = false;
		}
		else {
			// �޸��鼭 ��� �����
			AnimInfo stAnimInfo(0, 8, 32, 10, 1.f, 0, false);
			m_rOwner.SetNewAnimInfo(stAnimInfo);

			CommandInfo tNewCommandInfo;
			tNewCommandInfo.eCommand = COMMANDER::COMMAND_GATHERING;
			tNewCommandInfo.iUnitID = -1;
			tNewCommandInfo.vTargetPos = m_rOwner.GetXY();
			m_fRepeatTime = 1.f;
			m_rOwner.SetCommandInfo(tNewCommandInfo);

			m_bIsGathering = true;
		}
		m_fTickTime = 0.f;
	}

	if (!m_rOwner.MoveAlongPath(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_WavingFlag(m_rGameWorld, m_rOwner));
	}

	return 0;
}

void CAI_ComState_Running_Gathering::LateUpdate(void)
{
}

void CAI_ComState_Running_Gathering::OnExited(void)
{
	m_rOwner.SetCommandInfo(CommandInfo());	// ���� Ŀ�ǵ� ���� ����
}
