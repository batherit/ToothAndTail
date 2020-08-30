#include "stdafx.h"
#include "CPigState_Patrol.h"
#include "CPigState_Cropping.h"
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
