#include "stdafx.h"
#include "CSquirrelState_Idle.h"
#include "CSquirrel.h"

CSquirrelState_Idle::CSquirrelState_Idle(CGameWorld & _rGameWorld, CSquirrel & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CSquirrelState_Idle::~CSquirrelState_Idle()
{
}

void CSquirrelState_Idle::OnLoaded(void)
{
	// ������ StartPatrol���� ������ �ٲ۴�,,,
	m_rOwner.SetNewAnimInfo(AnimInfo(0, 8, 0, 4, 1.f, 0, false));
	m_rOwner.SetSpeed(0.f);
}

int CSquirrelState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CSquirrelState_Idle::LateUpdate(void)
{
}

void CSquirrelState_Idle::OnExited(void)
{
}