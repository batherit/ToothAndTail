#include "stdafx.h"
#include "CAI_ComState_Idle.h"
#include "CAI_ComState_Run.h"
#include "CAI_ComState_Standing_Build.h"
#include "CAI_ComState_Running_Gathering.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CWindmill.h"
#include "CFarmland.h"

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
}

int CAI_ComState_Idle::Update(float _fDeltaTime)
{
	D3DXVECTOR3 vGoalPos;

	if (m_rOwner.GetTotalUnitsNum() >= AI_ATTACK_UNITS_NUM) {
		// 병력이 충분한 경우
		if (m_rOwner.GetTargetWindmill() && (&m_rOwner == m_rOwner.GetTargetWindmill()->GetCommander())) {
			// 자신의 제분소에서 가만히 있는 상태인 경우,
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_PLAYER);	// 플레이어 제분소를 찾는다.
			if (!vecWindmills.empty()) {
				// 플레이어 제분소로 병력을 이동시킨다.
				int iIndex = rand() % vecWindmills.size();
				vGoalPos = vecWindmills[iIndex]->GetXY();
				vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
				if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
					// TODO : CAI_ComState_Running_WaveFlag를 세팅하면 될 것 같다. 지금은 Run으로 세팅
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
				int iIndex = rand() % vecWindmills.size();
				vGoalPos = vecWindmills[iIndex]->GetXY();
				vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
				if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
				}
			}
			else {
				// 자신의 제분소가 없다면, 점령할 제분소를 찾는다.
				vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_UNOCCUPIED);
				if (!vecWindmills.empty()) {
					// 점령할 제분소를 향해 달린다.
					int iIndex = rand() % vecWindmills.size();
					vGoalPos = vecWindmills[iIndex]->GetXY();
					vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
					}
				}
				else {
					// 점령할 제분소도 없다면 아무곳이나 간다.
					vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
					if (!vecWindmills.empty()) {
						// 선택한 제분소를 향해 달린다.
						int iIndex = rand() % vecWindmills.size();
						vGoalPos = vecWindmills[iIndex]->GetXY();
						vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
						}
					}
				}
			}
		}
	}
	else{
		if (m_rOwner.GetMoney() >= AI_BUILD_MIN_COST) {
			// 병력은 없지만 자본은 충분한 경우,
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
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
					}
					/*else {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_Build(m_rGameWorld, m_rOwner));
					}*/
				}
				else {
					if (vecWindmills[iIndex]->DetectEmptyLot(vGoalPos)) {
						// TODO1_2 : 점령할 농장이 없다면, 제분소 주변 공터를 찾아 점령(Active)한다.
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
						}
						/*else {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_Build(m_rGameWorld, m_rOwner));
						}*/
					}
					else {
						// TODO1_3 : 점령할 공터가 없다면, 아무 제분소 주변을 간다.
						vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
						if (!vecWindmills.empty()) {
							// 선택한 제분소를 향해 달린다.
							int iIndex = rand() % vecWindmills.size();
							vGoalPos = vecWindmills[iIndex]->GetXY();
							vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
							if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
								m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
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
					vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
					if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
						m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
					}
				}
				else {
					// 점령할 제분소도 없다면 아무곳이나 간다.
					vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
					if (!vecWindmills.empty()) {
						// 선택한 제분소를 향해 달린다.
						int iIndex = rand() % vecWindmills.size();
						vGoalPos = vecWindmills[iIndex]->GetXY();
						vGoalPos.x += TILE_WIDTH * BASE_SCALE * 1.1f;	// 위치 보정
						if (m_rOwner.GeneratePathToGoal(vGoalPos, vecWindmills[iIndex])) {
							m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
						}
					}
				}
			}
		}
		else {
			// 병력도 자본도 충분하지 못한 상황이라면, 그냥 아무데나 돌아다닌다.
			auto& vecWindmills = m_rOwner.ExtractWindmills(WINDMILL::TYPE_RANDOM);
			CWindmill* pWindmill = vecWindmills[rand() % vecWindmills.size()];
			if (pWindmill->DetectEmptyLot(vGoalPos)) {
				// 아무 제분소를 향해 달린다.
				if (m_rOwner.GeneratePathToGoal(vGoalPos, pWindmill)) {
					m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
				}
			}
		}
	}

	// 아무것도 해당이 안되면 그냥 가만히 있는다.

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Idle::LateUpdate(void)
{
}

void CAI_ComState_Idle::OnExited(void)
{
}
