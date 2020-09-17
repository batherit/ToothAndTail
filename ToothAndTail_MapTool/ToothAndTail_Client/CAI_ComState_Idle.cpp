#include "stdafx.h"
#include "CAI_ComState_Idle.h"
#include "CAI_ComState_Run.h"
#include "CAI_ComState_Standing_Action.h"
#include "CAI_ComState_Running_Gathering.h"
#include "CAI_ComState_Running_GatheringForDefense.h"
#include "CAI_ComState_Standing_WavingFlag.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CWindmill.h"
#include "CFarmland.h"
#include "CPig.h"

CAI_ComState_Idle::CAI_ComState_Idle(CGameWorld & _rGameWorld, CCommanderAI & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CAI_ComState_Idle::~CAI_ComState_Idle()
{
}

void CAI_ComState_Idle::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	AnimInfo stAnimInfo(0, 8, 0, 10, 1.f, 0, false); // ���� �ִ� �׽�Ʈ
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_fKeepTime = GetNumberMinBetweenMax(1.f, 2.f);
}

int CAI_ComState_Idle::Update(float _fDeltaTime)
{
	m_rOwner.UpdateAnim(_fDeltaTime);

	auto& vecMyWindmills = m_rOwner.GetMyWindmills();
	
	if (vecMyWindmills.empty()) return 1;

	D3DXVECTOR3 vGoalPos;
	for (auto& pMyWindmill : vecMyWindmills) {
		if (pMyWindmill->IsAttackedRecently()) {
			// �÷��̾� ���мҷ� ������ �̵���Ų��.
			vGoalPos = pMyWindmill->GetXY();
			vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
			if (m_rOwner.GeneratePathToGoal(vGoalPos, pMyWindmill)) {
				// ���мҸ� �����Ϸ� ���� ���� �ƴϹǷ� nullptr�� ����.
				m_rOwner.SetWindmillToAttack(nullptr);
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
						// ���мҸ� �����Ϸ� ���� ���� �ƴϹǷ� nullptr�� ����.
						m_rOwner.SetWindmillToAttack(nullptr);
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

	if (m_rOwner.GetTotalUnitsNum() >= AI_ATTACK_UNITS_NUM && m_rOwner.IsPossibleInvade()) {
		// ������ ����ϰ�, ���� ������ ���¶��,
		if (m_rOwner.GetTargetWindmill() && (&m_rOwner == m_rOwner.GetTargetWindmill()->GetCommander())) {
			// �ڽ��� ���мҿ��� ������ �ִ� ������ ���,
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_OTHER);	// �÷��̾� ���мҸ� ã�´�.
			if (!vecWindmills.empty()) {
				// �÷��̾� ���мҷ� ������ �̵���Ų��.
				int iIndex = rand() % vecWindmills.size();
				vGoalPos = vecWindmills[iIndex]->GetXY();
				vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
				if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
					// ���� Ÿ���� �Ǵ� ���мҸ� �����صд�.
					m_rOwner.SetWindmillToAttack(vecWindmills[iIndex]);
					// �޸��鼭 ���� ������ ���·� �����Ѵ�.
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Running_Gathering(m_rGameWorld, m_rOwner));
				}
			}
			// �÷��̾� ���мҰ� ���ٸ� ������ �ִ´�. => ���ӿ���
		}
		else {
			// �ڽ��� ���мҿ� ���� ���� ���
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_OWN);
			if (!vecWindmills.empty()) {
				// �ڽ��� ���мҸ� ���� �޸���.
				CWindmill* pWindmill = vecWindmills[rand() % vecWindmills.size()];
				if (m_rOwner.GeneratePathToGoal(pWindmill->GetFarmlands()[rand() % 8]->GetXY(), pWindmill)) {
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
				}
			}
			else {
				// �ڽ��� ���мҰ� ���ٸ�, ������ ���мҸ� ã�´�.
				vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_UNOCCUPIED);
				if (!vecWindmills.empty()) {
					// ������ ���мҸ� ���� �޸���.
					int iIndex = rand() % vecWindmills.size();
					vGoalPos = vecWindmills[iIndex]->GetXY();
					vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
					}
				}
				else {
					// ������ ���мҵ� ���ٸ� �ƹ����̳� ����.
					vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
					if (!vecWindmills.empty()) {
						// ������ ���мҸ� ���� �޸���.
						int iIndex = rand() % vecWindmills.size();
						vGoalPos = vecWindmills[iIndex]->GetXY();
						vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
						}
					}
				}
			}
		}
	}
	else{
		if (m_rOwner.GetMoney() >= AI_BUILD_MIN_COST) {
			// ������ ���ų� ���� ��Ȳ�� ���� ����ġ ������, �ں��� ����� ���,
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_OWN);
			if (!vecWindmills.empty()) {
				// �ڱ� ���мҸ� ã��, ���м� �ֺ� ������ ������ ã�� ����(Active)�Ѵ�. 
				int iIndex = rand() % vecWindmills.size();
				CFarmland* pFarmlandToOccupy = nullptr;
				for (auto& pFarmland : vecWindmills[iIndex]->GetFarmlands()) {
					if (pFarmland->CanOccupy()) {
						pFarmlandToOccupy = pFarmland;
						break;
					}
				}
				if (pFarmlandToOccupy) {
					vGoalPos = pFarmlandToOccupy->GetXY();
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
					}
				}
				else {
					if (vecWindmills[iIndex]->DetectEmptyLot(vGoalPos) && m_rOwner.GetInstallableTunnelNum() > m_rOwner.GetTotalTunnelsNum()) {
						// TODO1_2 : ������ ������ ���ٸ�, ���м� �ֺ� ���͸� ã�� ����(Active)�Ѵ�.
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
						}

					}
					else {
						// TODO1_3 : ������ ���Ͱ� ���ٸ�, �ƹ� ���м� �ֺ��� ����.
						vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
						if (!vecWindmills.empty()) {
							// ������ ���мҸ� ���� �޸���.
							int iIndex = rand() % vecWindmills.size();
							vGoalPos = vecWindmills[iIndex]->GetXY();
							vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
							if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
								m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
							}
						}
					}
				}
			}
			else {
				// �ڽ��� ���мҰ� ���ٸ�, ������ ���мҸ� ã�´�.
				vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_UNOCCUPIED);
				if (!vecWindmills.empty()) {
					// ������ ���мҸ� ���� �޸���.
					int iIndex = rand() % vecWindmills.size();
					vGoalPos = vecWindmills[iIndex]->GetXY();
					vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
					}
				}
				else {
					// ������ ���мҵ� ���ٸ� �ƹ����̳� ����.
					vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
					if (!vecWindmills.empty()) {
						// ������ ���мҸ� ���� �޸���.
						int iIndex = rand() % vecWindmills.size();
						vGoalPos = vecWindmills[iIndex]->GetXY();
						vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// ��ġ ����
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
						}
					}
				}
			}
		}
		else {
			// ���µ� �ں��� ������� ���� ��Ȳ�̶��, �׳� �ƹ����� ���ƴٴѴ�.
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
			CWindmill* pWindmill = vecWindmills[rand() % vecWindmills.size()];
			pWindmill->GetFarmlands()[rand() % 8]->GetXY();
			if (m_rOwner.GeneratePathToGoal(pWindmill->GetFarmlands()[rand() % 8]->GetXY(), pWindmill)) {
				m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
			}
		}
	}

	// �ƹ��͵� �ش��� �ȵǸ�, ���� �ð����� �ִٰ� �ƹ� ���мҷ� ����.
	if ((m_fElapsedTime += _fDeltaTime) >= m_fKeepTime) {
		auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
		CWindmill* pWindmill = vecWindmills[rand() % vecWindmills.size()];
		pWindmill->GetFarmlands()[rand() % 8]->GetXY();
		if (m_rOwner.GeneratePathToGoal(pWindmill->GetFarmlands()[rand() % 8]->GetXY(), pWindmill)) {
			m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
		}
	}

	return 0;
}

void CAI_ComState_Idle::LateUpdate(void)
{
}

void CAI_ComState_Idle::OnExited(void)
{
}
