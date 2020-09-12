#include "stdafx.h"
#include "CComState_Running_Jump.h"
#include "CComState_Run.h"
#include "CComState_Idle.h"
#include "CCommander.h"
#include "CStateMgr.h"



CComState_Running_Jump::CComState_Running_Jump(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CComState_Running_Jump::~CComState_Running_Jump()
{
}

void CComState_Running_Jump::OnLoaded(void)
{
	AnimInfo stAnimInfo(0, 8, 42, 5, 0.4f, 1, false);
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_rOwner.SetSpeed(COMMANDER_SPEED);
}

int CComState_Running_Jump::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (!m_rOwner.IsAnimPlaying())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Run(m_rGameWorld, m_rOwner));
	}
	else {
		if (!m_rOwner.IsAnimPlaying())
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Idle(m_rGameWorld, m_rOwner));
	}

	if (!m_bIsBuilded && m_rOwner.IsBuilding()) {
		// TODO : 땅굴 설치 로직을 작성합니다.
		m_bIsBuilded = true;
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Running_Jump::LateUpdate(void)
{
}

void CComState_Running_Jump::OnExited(void)
{
}
