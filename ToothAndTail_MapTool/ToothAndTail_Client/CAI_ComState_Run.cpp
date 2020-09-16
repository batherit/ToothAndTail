#include "stdafx.h"
#include "CAI_ComState_Run.h"
#include "CAI_ComState_Idle.h"
#include "CAI_ComState_Standing_Action.h"
#include "CAI_ComState_Running_GatheringForDefense.h"
#include "CAI_ComState_Standing_WavingFlag.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"
#include "CWindmill.h"

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
	auto& vecMyWindmills = m_rOwner.GetMyWindmills();

	if (vecMyWindmills.empty()) return 1;

	D3DXVECTOR3 vGoalPos;
	for (auto& pMyWindmill : vecMyWindmills) {
		if (pMyWindmill->IsAttackedRecently()) {
			// 플레이어 제분소로 병력을 이동시킨다.
			vGoalPos = pMyWindmill->GetXY();
			vGoalPos.y += TILE_HEIGHT * BASE_SCALE * 1.2f;	// 위치 보정
			if (m_rOwner.GeneratePathToGoal(vGoalPos, pMyWindmill)) {
				// TODO : CAI_ComState_Running_WaveFlag를 세팅하면 될 것 같다. 지금은 Run으로 세팅
				m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Running_GatheringForDefense(m_rGameWorld, m_rOwner));
			}
			else {
				m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_WavingFlag(m_rGameWorld, m_rOwner));
			}
			return 0;
		}
	}

	if (!m_rOwner.MoveAlongPath(_fDeltaTime)) {
		if(m_bIsActivating) m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Standing_Action(m_rGameWorld, m_rOwner));
		else m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Idle(m_rGameWorld, m_rOwner));
	}
		 
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Run::LateUpdate(void)
{
}

void CAI_ComState_Run::OnExited(void)
{
}
