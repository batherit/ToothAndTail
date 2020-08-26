#include "stdafx.h"
#include "CComState_Idle.h"
#include "CCommander.h"


CComState_Idle::CComState_Idle(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CComState_Idle::~CComState_Idle()
{
}

void CComState_Idle::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	AnimInfo stAnimInfo(0, 8, 0, 10, 1.f, 0, false); // 무한 애님 테스트
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CComState_Idle::Update(float _fDeltaTime)
{
	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Idle::LateUpdate(void)
{
}

void CComState_Idle::OnExited(void)
{
}
