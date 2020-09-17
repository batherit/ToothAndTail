#include "stdafx.h"
#include "CAI_ComState_Run.h"
#include "CAI_ComState_Idle.h"
#include "CAI_ComState_Standing_Action.h"
#include "CAI_ComState_Running_GatheringForDefense.h"
#include "CAI_ComState_Standing_WavingFlag.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CWindmill.h"
#include "CFarmland.h"
#include "CPig.h"

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
	m_rOwner.UpdateAnim(_fDeltaTime);

	auto& vecMyWindmills = m_rOwner.GetMyWindmills();
	if (vecMyWindmills.empty()) {
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
		return 1;
	}

	D3DXVECTOR3 vGoalPos;
	CWindmill* pWindmillToAttack = m_rOwner.GetWindmillToAttack();
	if (!pWindmillToAttack || !pWindmillToAttack->IsAttackedRecently()) {
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


	if (!m_rOwner.MoveAlongPath(_fDeltaTime)) {
		if(m_bIsActivating) m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_Action(m_rGameWorld, m_rOwner));
		else m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
	}
		 
	return 0;
}

void CAI_ComState_Run::LateUpdate(void)
{
}

void CAI_ComState_Run::OnExited(void)
{
}
