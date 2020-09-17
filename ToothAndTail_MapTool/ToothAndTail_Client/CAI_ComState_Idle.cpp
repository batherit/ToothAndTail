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
	AnimInfo stAnimInfo(0, 8, 0, 10, 1.f, 0, false); // 무한 애님 테스트
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
			// 플레이어 제분소로 병력을 이동시킨다.
			vGoalPos = pMyWindmill->GetXY();
			vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
			if (m_rOwner.GeneratePathToGoal(vGoalPos, pMyWindmill)) {
				// 제분소를 공격하러 가는 것이 아니므로 nullptr로 설정.
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
						// 제분소를 공격하러 가는 것이 아니므로 nullptr로 설정.
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
		// 병력이 충분하고, 공격 가능한 상태라면,
		if (m_rOwner.GetTargetWindmill() && (&m_rOwner == m_rOwner.GetTargetWindmill()->GetCommander())) {
			// 자신의 제분소에서 가만히 있는 상태인 경우,
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_OTHER);	// 플레이어 제분소를 찾는다.
			if (!vecWindmills.empty()) {
				// 플레이어 제분소로 병력을 이동시킨다.
				int iIndex = rand() % vecWindmills.size();
				vGoalPos = vecWindmills[iIndex]->GetXY();
				vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
				if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
					// 공격 타겟이 되는 제분소를 저장해둔다.
					m_rOwner.SetWindmillToAttack(vecWindmills[iIndex]);
					// 달리면서 병력 모으는 상태로 설정한다.
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Running_Gathering(m_rGameWorld, m_rOwner));
				}
			}
			// 플레이어 제분소가 없다면 가만히 있는다. => 게임오버
		}
		else {
			// 자신의 제분소에 있지 않은 경우
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_OWN);
			if (!vecWindmills.empty()) {
				// 자신의 제분소를 향해 달린다.
				CWindmill* pWindmill = vecWindmills[rand() % vecWindmills.size()];
				if (m_rOwner.GeneratePathToGoal(pWindmill->GetFarmlands()[rand() % 8]->GetXY(), pWindmill)) {
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
				}
			}
			else {
				// 자신의 제분소가 없다면, 점령할 제분소를 찾는다.
				vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_UNOCCUPIED);
				if (!vecWindmills.empty()) {
					// 점령할 제분소를 향해 달린다.
					int iIndex = rand() % vecWindmills.size();
					vGoalPos = vecWindmills[iIndex]->GetXY();
					vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
					}
				}
				else {
					// 점령할 제분소도 없다면 아무곳이나 간다.
					vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
					if (!vecWindmills.empty()) {
						// 선택한 제분소를 향해 달린다.
						int iIndex = rand() % vecWindmills.size();
						vGoalPos = vecWindmills[iIndex]->GetXY();
						vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
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
			// 병력은 없거나 공격 상황이 되지 여건치 않지만, 자본은 충분한 경우,
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_OWN);
			if (!vecWindmills.empty()) {
				// 자기 제분소를 찾고, 제분소 주변 점령할 농장을 찾아 점령(Active)한다. 
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
						// TODO1_2 : 점령할 농장이 없다면, 제분소 주변 공터를 찾아 점령(Active)한다.
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
						}

					}
					else {
						// TODO1_3 : 점령할 공터가 없다면, 아무 제분소 주변을 간다.
						vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
						if (!vecWindmills.empty()) {
							// 선택한 제분소를 향해 달린다.
							int iIndex = rand() % vecWindmills.size();
							vGoalPos = vecWindmills[iIndex]->GetXY();
							vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
							if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
								m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
							}
						}
					}
				}
			}
			else {
				// 자신의 제분소가 없다면, 점령할 제분소를 찾는다.
				vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_UNOCCUPIED);
				if (!vecWindmills.empty()) {
					// 점령할 제분소를 향해 달린다.
					int iIndex = rand() % vecWindmills.size();
					vGoalPos = vecWindmills[iIndex]->GetXY();
					vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, true));
					}
				}
				else {
					// 점령할 제분소도 없다면 아무곳이나 간다.
					vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
					if (!vecWindmills.empty()) {
						// 선택한 제분소를 향해 달린다.
						int iIndex = rand() % vecWindmills.size();
						vGoalPos = vecWindmills[iIndex]->GetXY();
						vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
						}
					}
				}
			}
		}
		else {
			// 병력도 자본도 충분하지 못한 상황이라면, 그냥 아무데나 돌아다닌다.
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
			CWindmill* pWindmill = vecWindmills[rand() % vecWindmills.size()];
			pWindmill->GetFarmlands()[rand() % 8]->GetXY();
			if (m_rOwner.GeneratePathToGoal(pWindmill->GetFarmlands()[rand() % 8]->GetXY(), pWindmill)) {
				m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner, false));
			}
		}
	}

	// 아무것도 해당이 안되면, 일정 시간동안 있다가 아무 제분소로 간다.
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
