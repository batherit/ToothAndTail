#include "stdafx.h"
#include "CAI_ComState_Idle.h"
#include "CAI_ComState_Run.h"
#include "CCommanderAI.h"
#include "CStateMgr.h"

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
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.DetectPlaceToGo()) {
		// 방문할 곳을 찾았다면, 그곳으로 향한다.
		m_rOwner.GetStateMgr()->SetNextState(new CAI_ComState_Run(m_rGameWorld, m_rOwner));
	}
	/*if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (m_rOwner.GetToX() != 0.f || m_rOwner.GetToY() != 0.f)
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Run(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsFlagKeyPressed(eFlagType)) {
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_WaveFlag(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsBuilding()) {
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_Build(m_rGameWorld, m_rOwner));
	}*/

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CAI_ComState_Idle::LateUpdate(void)
{
}

void CAI_ComState_Idle::OnExited(void)
{
}
