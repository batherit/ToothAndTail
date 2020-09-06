#include "stdafx.h"
#include "CPigState_Cropping.h"
#include "CPigState_Patrol.h"
#include "CPigState_Attack.h"
#include "CPig.h"
#include "CStateMgr.h"


CPigState_Cropping::CPigState_Cropping(CGameWorld & _rGameWorld, CPig & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CPigState_Cropping::~CPigState_Cropping()
{
}

void CPigState_Cropping::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 9, 16, 2.f, 1, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CPigState_Cropping::Update(float _fDeltaTime)
{
	m_rOwner.DetectEnemyAround();
	if (m_rOwner.GetTargetEnemy()) {
		// 주변에 적을 감지했다면, 공격 상태로 전환한다.
		m_rOwner.GetStateMgr()->SetNextState(new CPigState_Attack(m_rGameWorld, m_rOwner));
	}

	if (1 == m_rOwner.UpdateAnim(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPigState_Patrol(m_rGameWorld, m_rOwner));
	}
	return 0;
}

void CPigState_Cropping::LateUpdate(void)
{
}

void CPigState_Cropping::OnExited(void)
{
}
