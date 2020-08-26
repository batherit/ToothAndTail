#include "stdafx.h"
#include "CComState_Standing_WaveFlag.h"
#include "CComState_Standing_Build.h"
#include "CComState_Idle.h"
#include "CComState_Run.h"
#include "CCommander.h"
#include "CStateMgr.h"


CComState_Standing_WaveFlag::CComState_Standing_WaveFlag(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
}

CComState_Standing_WaveFlag::~CComState_Standing_WaveFlag()
{
}

void CComState_Standing_WaveFlag::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	AnimInfo stAnimInfo(0, 8, 10, 10, 1.f, 0, false); // 무한 애님 테스트
	m_rOwner.SetNewAnimInfo(stAnimInfo);
}

int CComState_Standing_WaveFlag::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if (m_rOwner.IsMoveKeyPressed(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Run(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsBuildKeyPressed())
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_Build(m_rGameWorld, m_rOwner));
	else if(!m_rOwner.IsFlagKeyPressed(eFlagType)) 
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Idle(m_rGameWorld, m_rOwner));

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Standing_WaveFlag::LateUpdate(void)
{
}

void CComState_Standing_WaveFlag::OnExited(void)
{
}
