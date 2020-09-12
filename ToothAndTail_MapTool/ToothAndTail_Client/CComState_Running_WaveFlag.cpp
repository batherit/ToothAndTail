#include "stdafx.h"
#include "CComState_Running_WaveFlag.h"
#include "CComState_Running_Build.h"
#include "CComState_Running_Jump.h"
#include "CComState_Standing_WaveFlag.h"
#include "CComState_Standing_Build.h"
#include "CComState_Idle.h"
#include "CComState_Run.h"
#include "CCommander.h"
#include "CStateMgr.h"


CComState_Running_WaveFlag::CComState_Running_WaveFlag(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CComState_Running_WaveFlag::~CComState_Running_WaveFlag()
{
}

void CComState_Running_WaveFlag::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 32, 10, 1.f, 0, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);
}

int CComState_Running_WaveFlag::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (m_rOwner.IsBuilding())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Running_Build(m_rGameWorld, m_rOwner));
		else if (!m_rOwner.IsFlagKeyPressed(eFlagType)) 
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Running_Jump(m_rGameWorld, m_rOwner));
	}
	/*else if (m_rOwner.IsBuildKeyPressed()){
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_Build(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsFlagKeyPressed(eFlagType)) {
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_WaveFlag(m_rGameWorld, m_rOwner));
	}*/
	else m_rOwner.GetStateMgr()->SetNextState(new CComState_Idle(m_rGameWorld, m_rOwner));

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Running_WaveFlag::LateUpdate(void)
{
}

void CComState_Running_WaveFlag::OnExited(void)
{
}
