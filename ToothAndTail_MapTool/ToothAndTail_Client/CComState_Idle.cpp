#include "stdafx.h"
#include "CComState_Idle.h"
#include "CComState_Run.h"
#include "CComState_Standing_WaveFlag.h"
#include "CComState_Standing_Build.h"
#include "CCommander.h"
#include "CStateMgr.h"


CComState_Idle::CComState_Idle(CGameWorld & _rGameWorld, CCommander & _rOwner)
	:
	CState(_rGameWorld, _rOwner)
{
	m_wstrBuildText[0] = L"승리는 우리들의 것이니,,,!!";
	m_wstrBuildText[1] = L"적들은 무엇을 하고 있을지,,,";
	m_wstrBuildText[2] = L"가만히 있을 때가 아니다!";
}

CComState_Idle::~CComState_Idle()
{
}

void CComState_Idle::OnLoaded(void)
{
	m_rOwner.SetSpeed(0.f);
	AnimInfo stAnimInfo(0, 8, 0, 10, 1.f, 0, false); // 무한 애님 테스트
	m_rOwner.SetNewAnimInfo(stAnimInfo);
	m_fShoutOutTime = GetNumberMinBetweenMax(5.f, 10.f);
}

int CComState_Idle::Update(float _fDeltaTime)
{
	float fNewToX = 0.f;
	float fNewToY = 0.f;
	//CCommander::E_FLAG_TYPE eFlagType = CCommander::FLAG_TYPE_NONE;

	if ((m_fShoutOutTime -= _fDeltaTime) <= 0.f) {
		m_rOwner.ShoutOut(-150.f, -120.f, m_wstrBuildText[rand() % 3]);
		m_fShoutOutTime = GetNumberMinBetweenMax(5.f, 10.f);
	}

	if (m_rOwner.IsMoving(fNewToX, fNewToY)) {
		m_rOwner.SetToXY(fNewToX, fNewToY);
		m_rOwner.MoveByDeltaTime(_fDeltaTime);
		if (m_rOwner.GetToX() != 0.f || m_rOwner.GetToY() != 0.f)
			m_rOwner.GetStateMgr()->SetNextState(new CComState_Run(m_rGameWorld, m_rOwner));
	}
	else if(m_rOwner.IsWavingFlag(/*eFlagType*/)){
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_WaveFlag(m_rGameWorld, m_rOwner));
	}
	else if (m_rOwner.IsActivating()) {
		m_rOwner.GetStateMgr()->SetNextState(new CComState_Standing_Build(m_rGameWorld, m_rOwner));
	}

	return m_rOwner.UpdateAnim(_fDeltaTime);
}

void CComState_Idle::LateUpdate(void)
{
}

void CComState_Idle::OnExited(void)
{
}
