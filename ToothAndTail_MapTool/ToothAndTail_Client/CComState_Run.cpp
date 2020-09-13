#include "stdafx.h"
#include "CComState_Run.h"
#include "CComState_Idle.h"
#include "CComState_Standing_Build.h"
#include "CComState_Standing_WaveFlag.h"
#include "CComState_Running_Build.h"
#include "CComState_Running_WaveFlag.h"
#include "CCommander.h"
#include "CStateMgr.h"


CComState_Run::CComState_Run(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CComState_Run::~CComState_Run()
{
}

void CComState_Run::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 22, 10, 1.0f, 0, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);
}

int CComState_Run::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	//CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (m_rOwner.IsActivating())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Running_Build(m_rGameWorld, m_rOwner));
		else if (m_rOwner.IsWavingFlag(/*eFlagType*/))
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Running_WaveFlag(m_rGameWorld, m_rOwner));

	}
	else {
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Idle(m_rGameWorld, m_rOwner));
	}

	m_rOwner.UpdateAnim(_fDeltaTime);

	return 0;
}

void CComState_Run::LateUpdate(void)
{
}

void CComState_Run::OnExited(void)
{
}
