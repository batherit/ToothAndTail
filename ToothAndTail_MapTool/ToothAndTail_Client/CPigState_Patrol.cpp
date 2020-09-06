#include "stdafx.h"
#include "CPigState_Patrol.h"
#include "CPigState_Cropping.h"
#include "CPigState_Attack.h"
#include "CPig.h"
#include "CStateMgr.h"


CPigState_Patrol::CPigState_Patrol(CGameWorld & _rGameWorld, CPig & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CPigState_Patrol::~CPigState_Patrol()
{
}

void CPigState_Patrol::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 0, 9, 1.f, 0, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);

	m_rOwner.StartPatrol();
}

int CPigState_Patrol::Update(float _fDeltaTime)
{
	m_rOwner.DetectEnemyAround();
	if (m_rOwner.GetTargetEnemy()) {
		// 주변에 적을 감지했다면, 공격 상태로 전환한다.
		m_rOwner.GetStateMgr()->SetNextState(new CPigState_Attack(m_rGameWorld, m_rOwner));
	}

	if (1 == m_rOwner.UpdatePatrol(_fDeltaTime)) {
		m_rOwner.GetStateMgr()->SetNextState(new CPigState_Cropping(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CPigState_Patrol::LateUpdate(void)
{
}

void CPigState_Patrol::OnExited(void)
{
}
